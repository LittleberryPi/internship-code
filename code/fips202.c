/* Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe */

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "fips202.h"
#include "api.h"
#include "main.h"
#include "util.h"
#include "m5ops.h"

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64-offset)))
#define GEN_BITM(offset) (~((1ll<<offset)-1))

#ifdef NO_MOVPRFX
    #define MOVPRFX "//"
#else
    #define MOVPRFX "movprfx"
#endif

const uint8_t idx_state_cols[25] = {0,5,10,15,20,
                                     1,6,11,16,21,
                                     2,7,12,17,22,
                                     3,8,13,18,23,
                                     4,9,14,19,24};

const uint8_t idx_rows[7] = {0,5,10,15,20,0,5};

 const uint8_t idx_theta[25] = {4,4,4,4,4,
                                 0,0,0,0,0,
                                 1,1,1,1,1,
                                 2,2,2,2,2,
                                 3,3,3,3,3};

const uint64_t bitmasks[25] = {
                       0, GEN_BITM(28), GEN_BITM(61), GEN_BITM(23), GEN_BITM(46),
            GEN_BITM(63), GEN_BITM(20), GEN_BITM(54), GEN_BITM(19), GEN_BITM(62),
            GEN_BITM( 2), GEN_BITM(58), GEN_BITM(21), GEN_BITM(49), GEN_BITM(3),
            GEN_BITM(36), GEN_BITM( 9), GEN_BITM(39), GEN_BITM(43), GEN_BITM(8),
            GEN_BITM(37), GEN_BITM(44), GEN_BITM(25), GEN_BITM(56), GEN_BITM(50)};

const uint8_t idx_phi[25] = {0, 15,5, 20,10,
                              6, 21,11,1, 16,
                              12,2, 17,7, 22,
                              18,8, 23,13,3,
                              24,14,4, 19,9};

const uint8_t idx_plus5[25] = {  5,  6,  7,  8,  9,
                                10, 11, 12, 13, 14,
                                15, 16, 17, 18, 19,
                                20, 21, 22, 23, 24,
                                 0,  1,  2,  3,  4};

const uint8_t idx_plus10[25] = {10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19,
                               20, 21, 22, 23, 24,
                                0,  1,  2,  3,  4,
                                5,  6,  7,  8,  9};


/*************************************************
* Name:        load64
*
* Description: Load 8 bytes into uint64_t in little-endian order
*
* Arguments:   - const unsigned char *x: pointer to input byte array
*
* Returns the loaded 64-bit unsigned integer
**************************************************/
static uint64_t load64(const unsigned char *x)
{
  unsigned long long r = 0, i;

  for (i = 0; i < 8; ++i) {
    r |= (unsigned long long)x[i] << 8 * i;
  }
  return r;
}

/*************************************************
* Name:        store64
*
* Description: Store a 64-bit integer to a byte array in little-endian order
*
* Arguments:   - uint8_t *x: pointer to the output byte array
*              - uint64_t u: input 64-bit unsigned integer
**************************************************/
static void store64(uint8_t *x, uint64_t u)
{
  unsigned int i;

  for(i=0; i<8; ++i) {
    x[i] = u;
    u >>= 8;
  }
}

/* Keccak round constants */
static const uint64_t KeccakF_RoundConstants[NROUNDS] =
{
    (uint64_t)0x0000000000000001ULL,
    (uint64_t)0x0000000000008082ULL,
    (uint64_t)0x800000000000808aULL,
    (uint64_t)0x8000000080008000ULL,
    (uint64_t)0x000000000000808bULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008009ULL,
    (uint64_t)0x000000000000008aULL,
    (uint64_t)0x0000000000000088ULL,
    (uint64_t)0x0000000080008009ULL,
    (uint64_t)0x000000008000000aULL,
    (uint64_t)0x000000008000808bULL,
    (uint64_t)0x800000000000008bULL,
    (uint64_t)0x8000000000008089ULL,
    (uint64_t)0x8000000000008003ULL,
    (uint64_t)0x8000000000008002ULL,
    (uint64_t)0x8000000000000080ULL,
    (uint64_t)0x000000000000800aULL,
    (uint64_t)0x800000008000000aULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008080ULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008008ULL
};

/*************************************************
* Name:        KeccakF1600_StatePermute_c
*
* Description: The Keccak F1600 Permutation
*
* Arguments:   - uint64_t * state: pointer to in/output Keccak state
**************************************************/
void KeccakF1600_StatePermute_c(uint64_t * state)
{
    int round;

    uint64_t Aba, Abe, Abi, Abo, Abu;
    uint64_t Aga, Age, Agi, Ago, Agu;
    uint64_t Aka, Ake, Aki, Ako, Aku;
    uint64_t Ama, Ame, Ami, Amo, Amu;
    uint64_t Asa, Ase, Asi, Aso, Asu;
    uint64_t BCa, BCe, BCi, BCo, BCu;
    uint64_t Da, De, Di, Do, Du;
    uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
    uint64_t Ega, Ege, Egi, Ego, Egu;
    uint64_t Eka, Eke, Eki, Eko, Eku;
    uint64_t Ema, Eme, Emi, Emo, Emu;
    uint64_t Esa, Ese, Esi, Eso, Esu;

    //copyFromState(A, state)
    Aba = state[ 0];
    Abe = state[ 1];
    Abi = state[ 2];
    Abo = state[ 3];
    Abu = state[ 4];
    Aga = state[ 5];
    Age = state[ 6];
    Agi = state[ 7];
    Ago = state[ 8];
    Agu = state[ 9];
    Aka = state[10];
    Ake = state[11];
    Aki = state[12];
    Ako = state[13];
    Aku = state[14];
    Ama = state[15];
    Ame = state[16];
    Ami = state[17];
    Amo = state[18];
    Amu = state[19];
    Asa = state[20];
    Ase = state[21];
    Asi = state[22];
    Aso = state[23];
    Asu = state[24];

    for( round = 0; round < NROUNDS; round += 2 )
    {
        //    prepareTheta
        BCa = Aba^Aga^Aka^Ama^Asa;
        BCe = Abe^Age^Ake^Ame^Ase;
        BCi = Abi^Agi^Aki^Ami^Asi;
        BCo = Abo^Ago^Ako^Amo^Aso;
        BCu = Abu^Agu^Aku^Amu^Asu;

        //thetaRhoPiChiIotaPrepareTheta(round  , A, E)
        Da = BCu^ROL(BCe, 1);
        De = BCa^ROL(BCi, 1);
        Di = BCe^ROL(BCo, 1);
        Do = BCi^ROL(BCu, 1);
        Du = BCo^ROL(BCa, 1);

        Aba ^= Da;
        BCa = Aba;
        Age ^= De;
        BCe = ROL(Age, 44);
        Aki ^= Di;
        BCi = ROL(Aki, 43);
        Amo ^= Do;
        BCo = ROL(Amo, 21);
        Asu ^= Du;
        BCu = ROL(Asu, 14);

        Eba =   BCa ^((~BCe)&  BCi );
        Eba ^= (uint64_t)KeccakF_RoundConstants[round];
        Ebe =   BCe ^((~BCi)&  BCo );
        Ebi =   BCi ^((~BCo)&  BCu );
        Ebo =   BCo ^((~BCu)&  BCa );
        Ebu =   BCu ^((~BCa)&  BCe );

        Abo ^= Do;
        BCa = ROL(Abo, 28);
        Agu ^= Du;
        BCe = ROL(Agu, 20);
        Aka ^= Da;
        BCi = ROL(Aka,  3);
        Ame ^= De;
        BCo = ROL(Ame, 45);
        Asi ^= Di;
        BCu = ROL(Asi, 61);

        Ega =   BCa ^((~BCe)&  BCi );
        Ege =   BCe ^((~BCi)&  BCo );
        Egi =   BCi ^((~BCo)&  BCu );
        Ego =   BCo ^((~BCu)&  BCa );
        Egu =   BCu ^((~BCa)&  BCe );

        Abe ^= De;
        BCa = ROL(Abe,  1);
        Agi ^= Di;
        BCe = ROL(Agi,  6);
        Ako ^= Do;
        BCi = ROL(Ako, 25);
        Amu ^= Du;
        BCo = ROL(Amu,  8);
        Asa ^= Da;
        BCu = ROL(Asa, 18);

        Eka =   BCa ^((~BCe)&  BCi );
        Eke =   BCe ^((~BCi)&  BCo );
        Eki =   BCi ^((~BCo)&  BCu );
        Eko =   BCo ^((~BCu)&  BCa );
        Eku =   BCu ^((~BCa)&  BCe );

        Abu ^= Du;
        BCa = ROL(Abu, 27);
        Aga ^= Da;
        BCe = ROL(Aga, 36);
        Ake ^= De;
        BCi = ROL(Ake, 10);
        Ami ^= Di;
        BCo = ROL(Ami, 15);
        Aso ^= Do;
        BCu = ROL(Aso, 56);

        Ema =   BCa ^((~BCe)&  BCi );
        Eme =   BCe ^((~BCi)&  BCo );
        Emi =   BCi ^((~BCo)&  BCu );
        Emo =   BCo ^((~BCu)&  BCa );
        Emu =   BCu ^((~BCa)&  BCe );

        Abi ^= Di;
        BCa = ROL(Abi, 62);
        Ago ^= Do;
        BCe = ROL(Ago, 55);
        Aku ^= Du;
        BCi = ROL(Aku, 39);
        Ama ^= Da;
        BCo = ROL(Ama, 41);
        Ase ^= De;
        BCu = ROL(Ase,  2);

        Esa =   BCa ^((~BCe)&  BCi );
        Ese =   BCe ^((~BCi)&  BCo );
        Esi =   BCi ^((~BCo)&  BCu );
        Eso =   BCo ^((~BCu)&  BCa );
        Esu =   BCu ^((~BCa)&  BCe );

        //    prepareTheta
        BCa = Eba^Ega^Eka^Ema^Esa;
        BCe = Ebe^Ege^Eke^Eme^Ese;
        BCi = Ebi^Egi^Eki^Emi^Esi;
        BCo = Ebo^Ego^Eko^Emo^Eso;
        BCu = Ebu^Egu^Eku^Emu^Esu;

        //thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
        Da = BCu^ROL(BCe, 1);
        De = BCa^ROL(BCi, 1);
        Di = BCe^ROL(BCo, 1);
        Do = BCi^ROL(BCu, 1);
        Du = BCo^ROL(BCa, 1);

        Eba ^= Da;
        BCa = Eba;
        Ege ^= De;
        BCe = ROL(Ege, 44);
        Eki ^= Di;
        BCi = ROL(Eki, 43);
        Emo ^= Do;
        BCo = ROL(Emo, 21);
        Esu ^= Du;
        BCu = ROL(Esu, 14);
        Aba =   BCa ^((~BCe)&  BCi );
        Aba ^= (uint64_t)KeccakF_RoundConstants[round+1];
        Abe =   BCe ^((~BCi)&  BCo );
        Abi =   BCi ^((~BCo)&  BCu );
        Abo =   BCo ^((~BCu)&  BCa );
        Abu =   BCu ^((~BCa)&  BCe );

        Ebo ^= Do;
        BCa = ROL(Ebo, 28);
        Egu ^= Du;
        BCe = ROL(Egu, 20);
        Eka ^= Da;
        BCi = ROL(Eka, 3);
        Eme ^= De;
        BCo = ROL(Eme, 45);
        Esi ^= Di;
        BCu = ROL(Esi, 61);
        Aga =   BCa ^((~BCe)&  BCi );
        Age =   BCe ^((~BCi)&  BCo );
        Agi =   BCi ^((~BCo)&  BCu );
        Ago =   BCo ^((~BCu)&  BCa );
        Agu =   BCu ^((~BCa)&  BCe );

        Ebe ^= De;
        BCa = ROL(Ebe, 1);
        Egi ^= Di;
        BCe = ROL(Egi, 6);
        Eko ^= Do;
        BCi = ROL(Eko, 25);
        Emu ^= Du;
        BCo = ROL(Emu, 8);
        Esa ^= Da;
        BCu = ROL(Esa, 18);
        Aka =   BCa ^((~BCe)&  BCi );
        Ake =   BCe ^((~BCi)&  BCo );
        Aki =   BCi ^((~BCo)&  BCu );
        Ako =   BCo ^((~BCu)&  BCa );
        Aku =   BCu ^((~BCa)&  BCe );

        Ebu ^= Du;
        BCa = ROL(Ebu, 27);
        Ega ^= Da;
        BCe = ROL(Ega, 36);
        Eke ^= De;
        BCi = ROL(Eke, 10);
        Emi ^= Di;
        BCo = ROL(Emi, 15);
        Eso ^= Do;
        BCu = ROL(Eso, 56);
        Ama =   BCa ^((~BCe)&  BCi );
        Ame =   BCe ^((~BCi)&  BCo );
        Ami =   BCi ^((~BCo)&  BCu );
        Amo =   BCo ^((~BCu)&  BCa );
        Amu =   BCu ^((~BCa)&  BCe );

        Ebi ^= Di;
        BCa = ROL(Ebi, 62);
        Ego ^= Do;
        BCe = ROL(Ego, 55);
        Eku ^= Du;
        BCi = ROL(Eku, 39);
        Ema ^= Da;
        BCo = ROL(Ema, 41);
        Ese ^= De;
        BCu = ROL(Ese, 2);
        Asa =   BCa ^((~BCe)&  BCi );
        Ase =   BCe ^((~BCi)&  BCo );
        Asi =   BCi ^((~BCo)&  BCu );
        Aso =   BCo ^((~BCu)&  BCa );
        Asu =   BCu ^((~BCa)&  BCe );
    }

    //copyToState(state, A)
    state[ 0] = Aba;
    state[ 1] = Abe;
    state[ 2] = Abi;
    state[ 3] = Abo;
    state[ 4] = Abu;
    state[ 5] = Aga;
    state[ 6] = Age;
    state[ 7] = Agi;
    state[ 8] = Ago;
    state[ 9] = Agu;
    state[10] = Aka;
    state[11] = Ake;
    state[12] = Aki;
    state[13] = Ako;
    state[14] = Aku;
    state[15] = Ama;
    state[16] = Ame;
    state[17] = Ami;
    state[18] = Amo;
    state[19] = Amu;
    state[20] = Asa;
    state[21] = Ase;
    state[22] = Asi;
    state[23] = Aso;
    state[24] = Asu;

    #undef    round
}



// VL=128-1024 bits, load the state into the registers for KECCAK
#define KECCAK_INIT\
    "pfalse p1.b                                                                   \n\
	ptrue p2.d                                                                     \n\
	pfirst p1.b, p2, p1.b                                                          \n\
	mov x2, #0                                                                     \n\
	ld1d	{ z12.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z13.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z14.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z10.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z11.d }, p1/z, [%[state], x2, lsl #3]                                \n\
                                                                                   \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z7.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z8.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z9.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z5.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z6.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
                                                                                   \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z2.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z3.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z4.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z0.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z1.d }, p1/z, [%[state], x2, lsl #3]                                 \n\
                                                                                   \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z22.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z23.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z24.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z20.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z21.d }, p1/z, [%[state], x2, lsl #3]                                \n\
                                                                                   \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z17.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z18.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z19.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z15.d }, p1/z, [%[state], x2, lsl #3]                                \n\
	add x2, x2, #1                                                                 \n\
	ld1d	{ z16.d }, p1/z, [%[state], x2, lsl #3]                                \n\ "

// VL=128-1024 bits, do one round of KECCAK
#define KECCAK_ITER(ba, ga, ka, ma, sa, be, ge, ke, me, se, bi, gi, ki, mi, si, bo, go, ko, mo, so, bu, gu, ku, mu, su, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7)\
    "//  Theta Calculations                                                         \n\
	"MOVPRFX" "tmp1", "ba"                                                          \n\
	eor3   "tmp1".d,  "tmp1".d, "ga".d, "ka".d                                      \n\
	eor3   "tmp1".d, "tmp1".d, "ma".d, "sa".d                                       \n\
	"MOVPRFX" "tmp2", "be"                                                          \n\
	eor3   "tmp2".d,  "tmp2".d,  "ge".d, "ke".d                                     \n\
	eor3   "tmp2".d,  "tmp2".d,  "me".d, "se".d                                     \n\
	"MOVPRFX" "tmp3", "bi"                                                          \n\
	eor3   "tmp3".d,  "tmp3".d,  "gi".d, "ki".d                                     \n\
	eor3   "tmp3".d,  "tmp3".d,  "mi".d, "si".d                                     \n\
	"MOVPRFX" "tmp4", "bo"                                                          \n\
	eor3   "tmp4".d,  "tmp4".d,  "go".d, "ko".d                                     \n\
	eor3   "tmp4".d,  "tmp4".d,  "mo".d, "so".d                                     \n\
	"MOVPRFX" "tmp5", "bu"                                                          \n\
	eor3   "tmp5".d,  "tmp5".d,  "gu".d, "ku".d                                     \n\
	eor3   "tmp5".d,  "tmp5".d,  "mu".d, "su".d                                     \n\
                                                                                    \n\
	rax1   "tmp6".d,   "tmp5".d,   "tmp2".d                                         \n\
	rax1   "tmp7".d,   "tmp3".d,   "tmp5".d                                         \n\
	rax1   "tmp5".d,   "tmp1".d,   "tmp3".d                                         \n\
	rax1   "tmp3".d,   "tmp4".d,   "tmp1".d                                         \n\
	rax1   "tmp1".d,   "tmp2".d,   "tmp4".d                                         \n\
                                                                                    \n\
	//  Phirho Stage                                                                \n\
	eor    "ba".d,   "ba".d, "tmp6".d                                               \n\
	"MOVPRFX" "tmp2", "mu"                                                          \n\
	xar    "tmp2".d,  "tmp2".d, "tmp3".d,  #56                                      \n\
	"MOVPRFX" "mu", "so"                                                            \n\
	xar    "mu".d,  "mu".d, "tmp7".d,  #8                                           \n\
	"MOVPRFX" "so", "ma"                                                            \n\
	xar    "so".d,  "so".d, "tmp6".d,  #23                                          \n\
	"MOVPRFX" "ma", "bu"                                                            \n\
	xar    "ma".d,  "ma".d, "tmp3".d,  #37                                          \n\
	"MOVPRFX" "bu", "su"                                                            \n\
	xar    "bu".d,  "bu".d, "tmp3".d,  #50                                          \n\
	"MOVPRFX" "su", "se"                                                            \n\
	xar    "su".d,  "su".d, "tmp5".d,  #62                                          \n\
	"MOVPRFX" "se", "go"                                                            \n\
	xar    "se".d,  "se".d,  "tmp7".d,  #9                                          \n\
	"MOVPRFX" "go", "me"                                                            \n\
	xar    "go".d,   "go".d, "tmp5".d,  #19                                         \n\
	"MOVPRFX" "me", "ga"                                                            \n\
	xar    "me".d,  "me".d,  "tmp6".d,  #28                                         \n\
	"MOVPRFX" "ga", "bo"                                                            \n\
	xar    "ga".d,   "ga".d, "tmp7".d,  #36                                         \n\
	"MOVPRFX" "bo", "mo"                                                            \n\
	xar    "bo".d,  "bo".d, "tmp7".d,  #43                                          \n\
	"MOVPRFX" "mo", "mi"                                                            \n\
	xar    "mo".d,  "mo".d, "tmp1".d,  #49                                          \n\
	"MOVPRFX" "mi", "ke"                                                            \n\
	xar    "mi".d,  "mi".d,  "tmp5".d,  #54                                         \n\
	"MOVPRFX" "ke", "gi"                                                            \n\
	xar    "ke".d,   "ke".d,  "tmp1".d,  #58                                        \n\
	"MOVPRFX" "gi", "ka"                                                            \n\
	xar    "gi".d,   "gi".d,  "tmp6".d,  #61                                        \n\
	"MOVPRFX" "ka", "be"                                                            \n\
	xar    "ka".d,   "ka".d, "tmp5".d,  #63                                         \n\
	"MOVPRFX" "be", "ge"                                                            \n\
	xar    "be".d,  "be".d,  "tmp5".d,  #20                                         \n\
	"MOVPRFX" "ge", "gu"                                                            \n\
	xar    "ge".d,   "ge".d,  "tmp3".d,  #44                                        \n\
	"MOVPRFX" "gu", "si"                                                            \n\
	xar    "gu".d,   "gu".d, "tmp1".d,  #3                                          \n\
	"MOVPRFX" "si", "ku"                                                            \n\
	xar    "si".d,  "si".d,  "tmp3".d,  #25                                         \n\
	"MOVPRFX" "ku", "sa"                                                            \n\
	xar    "ku".d,   "ku".d, "tmp6".d,  #46                                         \n\
	"MOVPRFX" "sa", "bi"                                                            \n\
	xar    "sa".d,  "sa".d, "tmp1".d,  #2                                           \n\
	"MOVPRFX" "bi", "ki"                                                            \n\
	xar    "bi".d,  "bi".d,  "tmp1".d,  #21                                         \n\
	"MOVPRFX" "ki", "ko"                                                            \n\
	xar    "ki".d,   "ki".d,  "tmp7".d,  #39                                        \n\
                                                                                    \n\
	// XAR Output:                                                                  \n\
	//                                                                              \n\
	// ba  ka   sa  ga   ma                                                         \n\
	// me  be  ke   se  ge                                                          \n\
	// gi   mi  bi  ki   si                                                         \n\
	// so  go   mo  bo  tmp2                                                        \n\
	// ku   su  gu   mu  bu                                                         \n\
	//                                                                              \n\
	// temp:   tmp1(d), tmp2(d), tmp3(d), tmp4(d), tmp5(d), tmp6(d), tmp7(d)        \n\
                                                                                    \n\
                                                                                    \n\
	// Phi Output:                                                                  \n\
	//                                                                              \n\
	// ba   be   bi   bo   bu                                                       \n\
	// ga   ge   gi   go   gu                                                       \n\
	// ka   ke   ki   tmp2 ku                                                       \n\
	// ma   me   mi   mo   mu                                                       \n\
	// sa   se   si   so   su                                                       \n\
                                                                                    \n\
                                                                                    \n\
	//  Chi transformations                                                         \n\
                                                                                    \n\
	"MOVPRFX" "tmp7", "tmp2"                                                        \n\
	bcax    "tmp7".d, "tmp7".d, "ka".d,  "ku".d                                     \n\
	"MOVPRFX" "tmp3", "ku"                                                          \n\
	bcax    "tmp3".d, "tmp3".d,  "ke".d,  "ka".d                                    \n\
	"MOVPRFX" "tmp4", "ka"                                                          \n\
	bcax    "tmp4".d, "tmp4".d,  "ki".d,  "ke".d                                    \n\
	"MOVPRFX" "tmp5", "ke"                                                          \n\
	bcax    "tmp5".d, "tmp5".d,  "tmp2".d, "ki".d                                   \n\
	"MOVPRFX" "tmp6", "ki"                                                          \n\
	bcax    "tmp6".d, "tmp6".d,  "ku".d,  "tmp2".d                                  \n\
                                                                                    \n\
	"MOVPRFX" "ko", "go"                                                            \n\
	bcax    "ko".d,  "ko".d, "ga".d,  "gu".d                                        \n\
	"MOVPRFX" "ku", "gu"                                                            \n\
	bcax    "ku".d,  "ku".d, "ge".d,  "ga".d                                        \n\
	"MOVPRFX" "ka", "ga"                                                            \n\
	bcax    "ka".d,  "ka".d, "gi".d,  "ge".d                                        \n\
	"MOVPRFX" "ke", "ge"                                                            \n\
	bcax    "ke".d,  "ke".d, "go".d,  "gi".d                                        \n\
	"MOVPRFX" "ki", "gi"                                                            \n\
	bcax    "ki".d,  "ki".d, "gu".d,  "go".d                                        \n\
                                                                                    \n\
	"MOVPRFX" "go", "bo"                                                            \n\
	bcax    "go".d, "go".d, "ba".d,  "bu".d                                         \n\
	"MOVPRFX" "gu", "bu"                                                            \n\
	bcax    "gu".d, "gu".d, "be".d,  "ba".d                                         \n\
	"MOVPRFX" "ga", "ba"                                                            \n\
	bcax    "ga".d, "ga".d, "bi".d,  "be".d                                         \n\
	ld1d	{ "ge".d }, p2/z, [%[consts]]      // 8-byte Folded Reload              \n\
	add     %[consts], %[consts], #8			                                    \n\
	eor "ga".d, "ga".d, "ge".d		   	//xor with KeccakF_RoundConstants[round]    \n\
	"MOVPRFX" "ge", "be"                                                            \n\
	bcax    "ge".d, "ge".d, "bo".d,  "bi".d                                         \n\
	"MOVPRFX" "gi", "bi"                                                            \n\
	bcax    "gi".d, "gi".d, "bu".d,  "bo".d                                         \n\
                                                                                    \n\
	"MOVPRFX" "bo", "so"                                                            \n\
	bcax    "bo".d, "bo".d, "sa".d,  "su".d                                         \n\
	"MOVPRFX" "bu", "su"                                                            \n\
	bcax    "bu".d, "bu".d, "se".d,  "sa".d                                         \n\
	"MOVPRFX" "ba", "sa"                                                            \n\
	bcax    "ba".d, "ba".d, "si".d,  "se".d                                         \n\
	"MOVPRFX" "be", "se"                                                            \n\
	bcax    "be".d, "be".d, "so".d,  "si".d                                         \n\
	"MOVPRFX" "bi", "si"                                                            \n\
	bcax    "bi".d, "bi".d, "su".d,  "so".d                                         \n\
                                                                                    \n\
	"MOVPRFX" "so", "mo"                                                            \n\
	bcax    "so".d, "so".d, "ma".d,  "mu".d                                         \n\
	"MOVPRFX" "su", "mu"                                                            \n\
	bcax    "su".d, "su".d, "me".d,  "ma".d                                         \n\
	"MOVPRFX" "sa", "ma"                                                            \n\
	bcax    "sa".d, "sa".d, "mi".d,  "me".d                                         \n\
	"MOVPRFX" "se", "me"                                                            \n\
	bcax    "se".d, "se".d, "mo".d,  "mi".d                                         \n\
	"MOVPRFX" "si", "mi"                                                            \n\
	bcax    "si".d, "si".d, "mu".d,  "mo".d                                         \n\ "

// VL=128-1024 bits, store the registers back into the state
#define KECCAK_STORE\
    "mov x2, #0                                                                     \n\
	st1d {z12.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z13.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z14.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z10.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z11.d}, p1, [%[state], x2, lsl #3]                                        \n\
                                                                                    \n\
	add x2, x2, #1                                                                  \n\
	st1d {z7.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z8.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z9.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z5.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z6.d}, p1, [%[state], x2, lsl #3]                                         \n\
                                                                                    \n\
	add x2, x2, #1                                                                  \n\
	st1d {z2.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z3.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z4.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z0.d}, p1, [%[state], x2, lsl #3]                                         \n\
	add x2, x2, #1                                                                  \n\
	st1d {z1.d}, p1, [%[state], x2, lsl #3]                                         \n\
                                                                                    \n\
	add x2, x2, #1                                                                  \n\
	st1d {z22.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z23.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z24.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z20.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z21.d}, p1, [%[state], x2, lsl #3]                                        \n\
                                                                                    \n\
	add x2, x2, #1                                                                  \n\
	st1d {z17.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z18.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z19.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z15.d}, p1, [%[state], x2, lsl #3]                                        \n\
	add x2, x2, #1                                                                  \n\
	st1d {z16.d}, p1, [%[state], x2, lsl #3]                                        \n\ "


/*************************************************
* Name:        KeccakF1600_StatePermute_asm
*
* Description: The Keccak F1600 Permutation with SVE2 instrinsics for VL 128-1024 bits
*              //Comment this out if compiling with SVE2 compiler
*
* Arguments:   - uint64_t * state: pointer to in/output Keccak state
**************************************************/
// void KeccakF1600_StatePermute_asm(uint64_t * state)
// {
//     asm (
//         KECCAK_INIT
//         "mov x8, xzr                  \n\
//     	mov x4, #4                    \n\
//         whilelo p0.d, x8, x4          \n\
//     	1:                            \n\ "
//     		KECCAK_ITER("z12", "z7", "z2", "z22", "z17", "z13", "z8", "z3", "z23", "z18", "z14", "z9", "z4", "z24", "z19", "z10", "z5", "z0", "z20", "z15", "z11", "z6", "z1", "z21", "z16", "z25", "z26", "z27", "z28", "z29", "z30", "z31")
//     		KECCAK_ITER("z7", "z2", "z28", "z17", "z12", "z8", "z3", "z29", "z18", "z13", "z9", "z4", "z30", "z19", "z14", "z5", "z0", "z31", "z15", "z10", "z6", "z1", "z27", "z16", "z11", "z25", "z26", "z21", "z22", "z23", "z24", "z20")
//     		KECCAK_ITER("z2", "z28", "z22", "z12", "z7", "z3", "z29", "z23", "z13", "z8", "z4", "z30", "z24", "z14", "z9", "z0", "z31", "z20", "z10", "z5", "z1", "z27", "z21", "z11", "z6", "z25", "z26", "z16", "z17", "z18", "z19", "z15")
//     		KECCAK_ITER("z28", "z22", "z17", "z7", "z2", "z29", "z23", "z18", "z8", "z3", "z30", "z24", "z19", "z9", "z4", "z31", "z20", "z15", "z5", "z0", "z27", "z21", "z16", "z6", "z1", "z25", "z26", "z11", "z12", "z13", "z14", "z10")
//     		KECCAK_ITER("z22", "z17", "z12", "z2", "z28", "z23", "z18", "z13", "z3", "z29", "z24", "z19", "z14", "z4", "z30", "z20", "z15", "z10", "z0", "z31", "z21", "z16", "z11", "z1", "z27", "z25", "z26", "z6", "z7", "z8", "z9", "z5")
//     		KECCAK_ITER("z17", "z12", "z7", "z28", "z22", "z18", "z13", "z8", "z29", "z23", "z19", "z14", "z9", "z30", "z24", "z15", "z10", "z5", "z31", "z20", "z16", "z11", "z6", "z27", "z21", "z25", "z26", "z1", "z2", "z3", "z4", "z0")
//     		"add x8, x8, #1           \n\
//     		whilelo p0.d, x8, x4      \n\
//     		b.mi 1b                   \n\ "
//         KECCAK_STORE
//         : /** no output **/
//         : [state] "r" (state), [consts] "r" (KeccakF_RoundConstants)
//         : "memory", "x4", "x8", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10",
//         "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23",
//         "z24", "z25", "z26", "z30", "z31", "p0", "p1", "p2"
//     );
// }

/*************************************************
* Name:        KeccakF1600_StatePermute_asm_2048
*
* Description: The Keccak F1600 Permutation with SVE2 instrinsics for VL 2048 bits
*              Comment this out if compiling with SVE2 compiler
*
* Arguments:   - uint64_t * state: pointer to in/output Keccak state
**************************************************/
// void KeccakF1600_StatePermute_asm_2048(uint64_t * state)
// {
//
//     asm (
//         "// Load state                                   \n\
//         mov x4, #0                                       \n\
//         mov x5, #25                                      \n\
//         whilelo p0.d, xzr, x5                            \n\
//         mov x5, #7                                       \n\
//         whilelo p1.d, xzr, x5                            \n\
//         mov x5, #1                                       \n\
//         whilelo p2.d, xzr, x5                            \n\
//         ld1b z25.d, p0/z, [%[idx_state_cols]]            \n\
//         ld1b z26.d, p1/z, [%[idx_rows]]                  \n\
//         ld1b z27.d, p0/z, [%[idx_theta]]                 \n\
//         ld1b z28.d, p0/z, [%[idx_phi]]                   \n\
//         ld1b z29.d, p0/z, [%[idx_plus5]]                 \n\
//         ld1b z30.d, p0/z, [%[idx_plus10]]                \n\
//         ld1d z31.d, p0/z, [%[bitmasks]]                  \n\
//         // Load rows                                     \n\
//         ld1d z0.d, p0/z, [%[state], z25.d, sxtw #3]      \n\
//         "MOVPRFX" z1, z26                                \n\
//         add z1.d, z1.d, #1                               \n\
//         "MOVPRFX" z2, z26                                \n\
//         add z2.d, z2.d, #2                               \n\
//         "MOVPRFX" z3, z26                                \n\
//         add z3.d, z3.d, #3                               \n\
//         "MOVPRFX" z4, z26                                \n\
//         add z4.d, z4.d, #4                               \n\
//     .Keccak2048_Loop:                                    \n\
//         // Theta Calculations                            \n\
//         tbl z5.d, z0.d, z26.d                            \n\
//         tbl z6.d, z0.d, z1.d                             \n\
//         tbl z7.d, z0.d, z2.d                             \n\
//         eor3 z7.d, z7.d, z6.d, z5.d                      \n\
//         tbl z5.d, z0.d, z3.d                             \n\
//         tbl z6.d, z0.d, z4.d                             \n\
//         eor3 z7.d, z7.d, z6.d, z5.d                      \n\
//         // RAX                                           \n\
//         "MOVPRFX" z6, z7                                 \n\
//         ext z6.b, z6.b, z7.b, #16                        \n\
//         rax1 z7.d, z7.d, z6.d                            \n\
//         //XAR                                            \n\
//         tbl z7.d, z7.d, z27.d                            \n\
//         eor z0.d, z0.d, z7.d                             \n\
//         bgrp z0.d, z0.d, z31.d                           \n\
//         // PHI                                           \n\
//         tbl z0.d, z0.d, z28.d                            \n\
//         // BCAX                                          \n\
//         tbl z7.d, z0.d, z29.d                            \n\
//         tbl z6.d, z0.d, z30.d                            \n\
//         bcax z0.d, z0.d, z6.d, z7.d                      \n\
//         // XOR with round constant                       \n\
//         ldr x5, [%[consts]], #8                          \n\
//         dup z7.d, x5                                     \n\
//         eor z0.d, p2/m, z0.d, z7.d                       \n\
//         // Check loop condition                          \n\
//         add x4, x4, #1                                   \n\
//         cmp     x4, #24                                  \n\
//         blt     .Keccak2048_Loop                         \n\
//         // Store state to memory                         \n\
//         st1d {z0.d}, p0, [%[state], z25.d, sxtw #3]      \n\
//         "
//         : [state] "+r" (state)
//         : [consts] "r" (KeccakF_RoundConstants),
//         [idx_state_cols] "r" (idx_state_cols),
//         [idx_rows] "r" (idx_rows), [idx_theta] "r" (idx_theta),
//         [idx_plus5] "r" (idx_plus5), [idx_plus10] "r" (idx_plus10),
//         [bitmasks] "r" (bitmasks), [idx_phi] "r" (idx_phi)
//         : "memory", "cc",
//           "x4", "x5",
//           "p0", "p1", "p2",
//           "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7",
//           "z25", "z26", "z27", "z28", "z29", "z30", "z31"
//     );
//
//     // --------------------------------------------------------
//     // TBD:
//     //      - try TBL before LD1 vs. scatter store; if good try same with gather ld
//     // --------------------------------------------------------
// }

/*************************************************
* Name:        KeccakF1600_StatePermute
*
* Description: The Keccak F1600 Permutation
*
* Arguments:   - uint64_t * state: pointer to in/output Keccak state
**************************************************/
void KeccakF1600_StatePermute(uint64_t * state) {
    #ifdef GEM5_TEST_KECCAK
        m5_reset_stats(0,0);
    #endif

    //Comment this out if compiling with SVE2 compiler:
    // #ifdef KECCAK_ASM
    //     (*KeccakF1600_sve2)(state);
    // #else
        KeccakF1600_StatePermute_c(state);
    // #endif

    #ifdef GEM5_TEST_KECCAK
        m5_dump_stats(0,0);
    #endif
}

#include <string.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))


/*************************************************
* Name:        keccak_absorb
*
* Description: Absorb step of Keccak;
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - uint64_t *s:             pointer to (uninitialized) output Keccak state
*              - unsigned int r:          rate in bytes (e.g., 168 for SHAKE128)
*              - const unsigned char *m:  pointer to input to be absorbed into s
*              - unsigned long long mlen: length of input in bytes
*              - unsigned char p:         domain-separation byte for different Keccak-derived functions
**************************************************/
static void keccak_absorb(uint64_t *s,
                          unsigned int r,
                          const unsigned char *m, unsigned long long int mlen,
                          unsigned char p)
{
    unsigned long long i;
    unsigned char t[200];

    // Zero state
    for (i = 0; i < 25; ++i)
        s[i] = 0;

    while (mlen >= r)
    {
        for (i = 0; i < r / 8; ++i)
          s[i] ^= load64(m + 8 * i);

        KeccakF1600_StatePermute(s);

        mlen -= r;
        m += r;
    }

    for (i = 0; i < r; ++i)
        t[i] = 0;

    for (i = 0; i < mlen; ++i)
        t[i] = m[i];
    t[i] = p;
    t[r - 1] |= 128;
    for (i = 0; i < r / 8; ++i)
        s[i] ^= load64(t + 8 * i);
}


/*************************************************
* Name:        keccak_squeezeblocks
*
* Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
*              Modifies the state. Can be called multiple times to keep squeezing,
*              i.e., is incremental.
*
* Arguments:   - unsigned char *h:               pointer to output blocks
*              - unsigned long long int nblocks: number of blocks to be squeezed (written to h)
*              - uint64_t *s:                    pointer to in/output Keccak state
*              - unsigned int r:                 rate in bytes (e.g., 168 for SHAKE128)
**************************************************/
static void keccak_squeezeblocks(unsigned char *h, unsigned long long int nblocks,
                                 uint64_t *s,
                                 unsigned int r)
{
  unsigned int i;
  while(nblocks > 0)
  {
    KeccakF1600_StatePermute(s);
    for(i=0;i<(r>>3);i++)
    {
      store64(h+8*i, s[i]);
    }
    h += r;
    nblocks--;
  }
}


/*************************************************
* Name:        shake128_absorb
*
* Description: Absorb step of the SHAKE128 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - uint64_t *s:                     pointer to (uninitialized) output Keccak state
*              - const unsigned char *input:      pointer to input to be absorbed into s
*              - unsigned long long inputByteLen: length of input in bytes
**************************************************/
void shake128_absorb(uint64_t *s, const unsigned char *input, unsigned int inputByteLen)
{
  keccak_absorb(s, SHAKE128_RATE, input, inputByteLen, 0x1F);
}

/*************************************************
* Name:        shake128_squeezeblocks
*
* Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of SHAKE128_RATE bytes each.
*              Modifies the state. Can be called multiple times to keep squeezing,
*              i.e., is incremental.
*
* Arguments:   - unsigned char *output:      pointer to output blocks
*              - unsigned long long nblocks: number of blocks to be squeezed (written to output)
*              - uint64_t *s:                pointer to in/output Keccak state
**************************************************/
void shake128_squeezeblocks(unsigned char *output, unsigned long long nblocks, uint64_t *s)
{
  keccak_squeezeblocks(output, nblocks, s, SHAKE128_RATE);
}

/*************************************************
* Name:        shake256
*
* Description: SHAKE256 XOF with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output
*              - unsigned long long outlen:  requested output length in bytes
               - const unsigned char *input: pointer to input
               - unsigned long long inlen:   length of input in bytes
**************************************************/
void shake256(unsigned char *output, unsigned long long outlen,
              const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25];
  unsigned char t[SHAKE256_RATE];
  unsigned long long nblocks = outlen/SHAKE256_RATE;
  size_t i;

  /* Absorb input */
  keccak_absorb(s, SHAKE256_RATE, input, inlen, 0x1F);

  /* Squeeze output */
  keccak_squeezeblocks(output, nblocks, s, SHAKE256_RATE);

  output+=nblocks*SHAKE256_RATE;
  outlen-=nblocks*SHAKE256_RATE;

  if(outlen)
  {
    keccak_squeezeblocks(t, 1, s, SHAKE256_RATE);
    for(i=0;i<outlen;i++)
      output[i] = t[i];
  }
}

/*************************************************
* Name:        sha3_256
*
* Description: SHA3-256 with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output (32 bytes)
*              - const unsigned char *input: pointer to input
*              - unsigned long long inlen:   length of input in bytes
**************************************************/
void sha3_256(unsigned char *output, const unsigned char *input,  unsigned long long inlen)
{
    uint64_t s[25];
    unsigned char t[SHA3_256_RATE];
    size_t i;

    /* Absorb input */
    keccak_absorb(s, SHA3_256_RATE, input, inlen, 0x06);

    /* Squeeze output */
    keccak_squeezeblocks(t, 1, s, SHA3_256_RATE);

    for(i=0;i<32;i++)
      output[i] = t[i];
}

/*************************************************
* Name:        sha3_512
*
* Description: SHA3-512 with non-incremental API
*
* Arguments:   - unsigned char *output:      pointer to output (64 bytes)
*              - const unsigned char *input: pointer to input
*              - unsigned long long inlen:   length of input in bytes
**************************************************/
void sha3_512(unsigned char *output, const unsigned char *input,  unsigned long long inlen)
{
  uint64_t s[25];
  unsigned char t[SHA3_512_RATE];
  size_t i;

  /* Absorb input */
  keccak_absorb(s, SHA3_512_RATE, input, inlen, 0x06);

  /* Squeeze output */
  keccak_squeezeblocks(t, 1, s, SHA3_512_RATE);

  for(i=0;i<64;i++)
      output[i] = t[i];
}
