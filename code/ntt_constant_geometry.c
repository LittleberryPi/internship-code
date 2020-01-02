#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "reduce.h"
#include "params.h"
#include "ntt.h"
#include "m5ops.h"
#include "main.h"
#include "zetas.h"
#include "ntt_init.h"
#include "ntt_iter.h"
#include "ntt_finish.h"
#include "invntt_init.h"
#include "invntt_iter.h"
#include "invntt_finish.h"
#include "util.h"

/* Code to generate zetas and zetas_inv used in the number-theoretic transform:

#define KYBER_ROOT_OF_UNITY 17

static const uint16_t tree[128] = {
0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127};


static int16_t fqmul(int16_t a, int16_t b) {
    return montgomery_reduce((int32_t)a*b);
}

void init_ntt() {
    unsigned int i, j, k;
    int16_t tmp[128];

    tmp[0] = MONT;
    for(i = 1; i < 128; ++i)
        tmp[i] = fqmul(tmp[i-1], KYBER_ROOT_OF_UNITY*MONT % KYBER_Q);

    for(i = 0; i < 128; ++i)
        zetas[i] = tmp[tree[i]];

    k = 0;
    for(i = 64; i >= 1; i >>= 1)
        for(j = i; j < 2*i; ++j)
            zetas_inv[k++] = -tmp[128 - tree[j]];

    zetas_inv[127] = MONT * (MONT * (KYBER_Q - 1) * ((KYBER_Q - 1)/128) % KYBER_Q) % KYBER_Q;
}

*/

int16_t zetas[128] = {
    2285, 2571, 2970, 1812, 1493, 1422, 287, 202, 3158, 622, 1577, 182, 962, 2127, 1855, 1468,
    573, 2004, 264, 383, 2500, 1458, 1727, 3199, 2648, 1017, 732, 608, 1787, 411, 3124, 1758,
    1223, 652, 2777, 1015, 2036, 1491, 3047, 1785, 516, 3321, 3009, 2663, 1711, 2167, 126, 1469,
    2476, 3239, 3058, 830, 107, 1908, 3082, 2378, 2931, 961, 1821, 2604, 448, 2264, 677, 2054,
    2226, 430, 555, 843, 2078, 871, 1550, 105, 422, 587, 177, 3094, 3038, 2869, 1574, 1653,
    3083, 778, 1159, 3182, 2552, 1483, 2727, 1119, 1739, 644, 2457, 349, 418, 329, 3173, 3254,
    817, 1097, 603, 610, 1322, 2044, 1864, 384, 2114, 3193, 1218, 1994, 2455, 220, 2142, 1670,
    2144, 1799, 2051, 794, 1819, 2475, 2459, 478, 3221, 3021, 996, 991, 958, 1869, 1522, 1628};

int16_t zetas_inv[128] = {
    1701, 1807, 1460, 2371, 2338, 2333, 308, 108, 2851, 870, 854, 1510, 2535, 1278, 1530, 1185,
    1659, 1187, 3109, 874, 1335, 2111, 136, 1215, 2945, 1465, 1285, 2007, 2719, 2726, 2232, 2512,
    75, 156, 3000, 2911, 2980, 872, 2685, 1590, 2210, 602, 1846, 777, 147, 2170, 2551, 246,
    1676, 1755, 460, 291, 235, 3152, 2742, 2907, 3224, 1779, 2458, 1251, 2486, 2774, 2899, 1103,
    1275, 2652, 1065, 2881, 725, 1508, 2368, 398, 951, 247, 1421, 3222, 2499, 271, 90, 853,
    1860, 3203, 1162, 1618, 666, 320, 8, 2813, 1544, 282, 1838, 1293, 2314, 552, 2677, 2106,
    1571, 205, 2918, 1542, 2721, 2597, 2312, 681, 130, 1602, 1871, 829, 2946, 3065, 1325, 2756,
    1861, 1474, 1202, 2367, 3147, 1752, 2707, 171, 3127, 3042, 1907, 1836, 1517, 359, 758, 1441};

// For the NTT constant geometry algorithm we have a different output order that needs to be
// switched to the order of the in place algorithm
uint8_t bit_reversed_order[256] = {
  0,128,1,129,2,130,3,131,4,132,5,133,6,134,7,135,8,136,9,137,10,138,11,139,12,140,13,141,14,
  142,15,143,16,144,17,145,18,146,19,147,20,148,21,149,22,150,23,151,24,152,25,153,26,154,27,
  155,28,156,29,157,30,158,31,159,32,160,33,161,34,162,35,163,36,164,37,165,38,166,39,167,40,
  168,41,169,42,170,43,171,44,172,45,173,46,174,47,175,48,176,49,177,50,178,51,179,52,180,53,
  181,54,182,55,183,56,184,57,185,58,186,59,187,60,188,61,189,62,190,63,191,64,192,65,193,66,
  194,67,195,68,196,69,197,70,198,71,199,72,200,73,201,74,202,75,203,76,204,77,205,78,206,79,
  207,80,208,81,209,82,210,83,211,84,212,85,213,86,214,87,215,88,216,89,217,90,218,91,219,92,
  220,93,221,94,222,95,223,96,224,97,225,98,226,99,227,100,228,101,229,102,230,103,231,104,232,
  105,233,106,234,107,235,108,236,109,237,110,238,111,239,112,240,113,241,114,242,115,243,116,
  244,117,245,118,246,119,247,120,248,121,249,122,250,123,251,124,252,125,253,126,254,127,255};

  // For the INVNTT constant geometry algorithm we have a different input order that needs to be
  // switched to the order of the in place algorithm
uint8_t invntt_order[256] =   {
  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,
  66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,
  122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,
  170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,
  216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,1,3,5,7,9,11,
  13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63,65,67,69,71,73,
  75,77,79,81,83,85,87,89,91,93,95,97,99,101,103,105,107,109,111,113,115,117,119,121,123,125,127,
  129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,171,173,175,
  177,179,181,183,185,187,189,191,193,195,197,199,201,203,205,207,209,211,213,215,217,219,221,223,
  225,227,229,231,233,235,237,239,241,243,245,247,249,251,253,255};

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/
static int16_t fqmul(int16_t a, int16_t b) {
    return montgomery_reduce((int32_t)a*b);
}

/*************************************************
* Name:        ntt_level_in_place_c
*
* Description: Inplace number-theoretic transform (NTT) in Rq
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void ntt_level_in_place_c(int16_t r[256]) {
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  for(len = 128; len >= 2; len >>= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k++];
      for(j = start; j < start + len; ++j) {
        t = fqmul(zeta, r[j + len]);
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
}

/*************************************************
* Name:        ntt_level_constant_c
*
* Description: One level of the constant geometry NTT
*
* Arguments:   - int16_t *p_r: pointer to pointer to input vector of elements of Zq
*              - int16_t *p_r2: pointer to pointer to output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for NTT level
**************************************************/
void ntt_level_constant_c(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]) {
    unsigned int j;
    unsigned int const len = 128;
    uint32_t multiplied, t;
    int16_t u;
    int output_index;
    output_index = 0;

    for(j = 0; j < len; j++) {
        multiplied = (int32_t) zetas_level[j] * p_r[j + len]; //remove int32_t

        // montgomery_reduce
        u = multiplied * QINV;
        t = (int32_t)u * KYBER_Q;
        t = multiplied - t;
        t >>= 16;

        p_r2[output_index + 1] = p_r[j] - t;
        p_r2[output_index] = p_r[j] + t;
        output_index += 2;
    }
}


/*************************************************
* Name:        ntt_level_asm_auto
*
* Description: One level of the constant geometry NTT using inline autogenerated assembly
*
* Arguments:   - int16_t *p_r: pointer to pointer to input vector of elements of Zq
*              - int16_t *p_r2: pointer to pointer to output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for NTT level
**************************************************/
void ntt_level_asm_auto(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]) {
    unsigned long len = 128;

    asm (
        " mov	x8, xzr																//x8=0                                \n\
        add	x10, %[p_r], #256          			      //x10=*p_r+256                        \n\
        ptrue	p0.s                                                                      \n\
        whilelo	p1.h, xzr, %[len]                                                       \n\
        mov	z0.s, %w[qinv]												//z0=QINV                             \n\
        mov	z1.s, %w[kyber_q]											//z1=KYBER_Q                          \n\
        1:                                                                                \n\
        ld1h	{ z2.h }, p1/z, [%[zetas_level], x8, lsl #1]		//z2=zetas_level          \n\
        ld1h	{ z3.h }, p1/z, [x10, x8, lsl #1]		//z3=p_r[up]                          \n\
        lsl	x11, x8, #33													//p_r2 (output_index)                 \n\
        add	x11, %[p_r2], x11, asr #31						//p_r2 (output_index)                 \n\
        sunpklo	z4.s, z2.h												//z4=lower of zetas_level             \n\
        sunpkhi	z2.s, z2.h												//z2=upper of zetas_level             \n\
        sunpklo	z5.s, z3.h												//z5=2/4 of *p_r                      \n\
        sunpkhi	z3.s, z3.h												//z3=3/4 of *p_r                      \n\
        movprfx	z6, z3														//z6=3/4 of *p_r                      \n\
        mul	z6.s, p0/m, z6.s, z2.s								//z6=p_r[3/4] * zetas_level[up]       \n\
        movprfx	z7, z5														//z7=lower half of *p_r               \n\
        mul	z7.s, p0/m, z7.s, z4.s								//z7=p_r[2/4] * zetas_level[low]      \n\
        mul	z7.s, p0/m, z7.s, z0.s								//z7=multiplied[2/4] * QINV           \n\
        mul	z6.s, p0/m, z6.s, z0.s								//z6=multiplied[3/4] * QINV           \n\
        asr	z6.s, z6.s, #16												//z6=t>>=16                           \n\
        asr	z7.s, z7.s, #16												//z7=t>>=16                           \n\
        mul	z7.s, p0/m, z7.s, z1.s								//z7=u[2/4] * KYBER_Q                 \n\
        mul	z6.s, p0/m, z6.s, z1.s								//z6=u[3/4] * KYBER_Q                 \n\
        mad	z2.s, p0/m, z3.s, z6.s								//z2=p_r[up] * zeta[up] + t[up]       \n\
        movprfx	z3, z7														//z3=z7                               \n\
        mla	z3.s, p0/m, z5.s, z4.s								//z3=t[low] + p_r[low] * zeta[low]    \n\
        ld1h	{ z4.h }, p1/z, [%[p_r], x8, lsl #1]//z4=*p_r                             \n\
        uzp2	z2.h, z3.h, z2.h										//z2=zipped t[low] and t[up]          \n\
        inch	x8																	//x8 + VL                             \n\
        sub	z6.h, z4.h, z2.h											//z6=p_r - t                          \n\
        add	z5.h, z4.h, z2.h											//z5=p_r + t                          \n\
        st2h	{ z5.h, z6.h }, p1, [x11]						//z5 -> p_r2[output_index]            \n\
        whilelo	p1.h, x8, %[len]									//while x8 < len                      \n\
        b.mi	1b"
        : /**no output**/
        : [p_r] "r" (p_r), [p_r2] "r" (p_r2), [zetas_level] "r" (zetas_level), [len] "r" (len),
        [qinv] "r" (-218038272), [kyber_q] "r" (-3329)
        : "memory", "x0", "x1", "x2", "x8", "x10", "x11", "z0", "z1", "z2", "z3", "z4",
        "z5", "z6", "z7", "p0", "p1"
    );
}



/*************************************************
* Name:        ntt_asm_128
*
* Description: The constant geometry NTT using inline assembly for vector length 128
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for an NTT level
**************************************************/
void ntt_asm_128(int16_t r[256], int16_t zetas_level[128]) {
    int16_t r2[256];
    // Choose between a version where zetas is stored in memory and loaded in every vector register computation
    // (LOAD_ZETAS) or a version where zetas is zipped and put in the right registers (ZIP_ZETAS)
    #ifdef LOAD_ZETAS
        unsigned long len = 64;

        asm (
            // Initialize by loading elements of r into registers
            INIT(LOAD_R_128)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            // Initialize by loading elements of r to registers
            ITER128_EVEN_LEVEL
            ITER128_ODD_LEVEL
            ITER128_EVEN_LEVEL
            ITER128_ODD_LEVEL
            ITER128_EVEN_LEVEL
            ITER128_ODD_LEVEL
            ITER128_EVEN_LEVEL
            // Finish by zipping (to get elements of r in bitreversed order) and store to r
            //Store the elements in registers to r array in memory
            ZIP128
            "mov x8, xzr                                         \n\ "
            STORE128
            //Store the elements from temp array (r2) to r array in memory
            "mov x8, xzr                                         \n\
            add x10, %[r2], #384                                 \n\ "
            LOAD_R2_128
            ZIP128
            "mov x8, #128                                        \n\ "
            STORE128
            : /**no output**/
            : [r] "r" (r), [r2] "r" (r2), [zetas_level] "r" (zetas_level), [len] "r" (len),
            [qinv] "r" (-218038272), [kyber_q] "r" (-3329)
            : "memory", "x8", "x9", "x10", "x11", "x13", "x14", "z0", "z1", "z2", "z3", "z4",
            "z5", "z6", "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17",
            "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z30", "z31", "p0"
        );
    #else // ZIP_ZETAS
        #ifndef ZIP_ZETAS
            printf("ERROR: Expected ZIP_ZETAS to be defined");
            exit(1);
        #endif
        UNUSED(zetas_level);  // Avoids warning about unused parameter

        asm (
            // Initialize by loading elements of r and zetas into registers
            INIT_R_AND_ZETAS(LOAD_R_128)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER128_ZIP_ZETAS_EVEN_LEVEL(GATHER_ZETAS_LVL6, "Z8", "Z8", "Z8", "Z8")
            ITER128_ZIP_ZETAS_ODD_LEVEL(GATHER_ZETAS_LVL5, "Z8", "Z8", "Z8", "Z8")
            ITER128_ZIP_ZETAS_EVEN_LEVEL(GATHER_ZETAS_LVL4, "Z8", "Z8", "Z8", "Z8")
            ITER128_ZIP_ZETAS_ODD_LEVEL(GATHER128_ZETAS_LVL3, "Z8", "Z8", "Z8", "Z8")
            ITER128_ZIP_ZETAS_EVEN_LEVEL(GATHER128_ZETAS_LVL2, "Z8", "Z29", "Z8", "Z29")
            ITER128_ZIP_ZETAS_ODD_LEVEL(GATHER128_ZETAS_LVL1, "Z8", "Z27", "Z28", "Z29")
            ITER128_ZIP_ZETAS_LAST_LEVEL(GATHER128_ZETAS_LVL0, "Z8", "Z27", "Z28", "Z29")
            // Finish by zipping (to get elements of r in bitreversed order) and store to r
            //Store the elements in registers to r array in memory
            ZIP128
            "mov x8, xzr                                         \n\ "
            STORE128
            //Store the elements from temp array (r2) to r array in memory
            "mov x8, xzr                                         \n\
            add x10, %[r2], #384                                \n\ "
            LOAD_R2_128
            ZIP128
            "mov x8, #128                                        \n\ "
            STORE128
          : /**no output**/
          : [r] "r" (r), [r2] "r" (r2), [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [zetas] "r" (zetas)
          : "memory", "x8", "x9", "x10", "x11", "x13", "x14", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
          "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19",
          "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31", "p0"
        );
    #endif
}

/*************************************************
* Name:        ntt_asm_256
*
* Description: The constant geometry NTT using inline assembly for vector length 256
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for an NTT level
**************************************************/
void ntt_asm_256(int16_t r[256], int16_t zetas_level[128]) {
    // Choose between a version where zetas is stored in memory and loaded in every vector register computation
    // (LOAD_ZETAS) or a version where zetas is zipped and put in the right registers (ZIP_ZETAS)
    #ifdef LOAD_ZETAS
        asm (
            // Initialize by loading elements of r into registers
            INIT(LOAD_R_256)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            ITER256_LOAD_ZETAS
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP256
            "mov x8, xzr \n\ "
            STORE256
            : /**no output**/
            : [r] "r" (r), [zetas_level] "r" (zetas_level), [qinv] "r" (-218038272),
            [kyber_q] "r" (-3329)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19",
            "z20", "z21", "z22", "z23", "z24", "z30", "z31", "p0"
        );
    #else // ZIP_ZETAS
        #ifndef ZIP_ZETAS
            printf("ERROR: Expected ZIP_ZETAS to be defined");
            exit(1);
        #endif
        UNUSED(zetas_level);  // Avoids warning about unused parameter

        asm (
            // Initialize by loading elements of r and zetas into registers
            INIT_R_AND_ZETAS(LOAD_R_256)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER256_ZIP_ZETAS(GATHER_ZETAS_LVL6, "Z8", "Z8", "Z8", "Z8")
            ITER256_ZIP_ZETAS(GATHER_ZETAS_LVL5, "Z8", "Z8", "Z8", "Z8")
            ITER256_ZIP_ZETAS(GATHER_ZETAS_LVL4, "Z8", "Z8", "Z8", "Z8")
            ITER256_ZIP_ZETAS(GATHER_ZETAS_LVL3, "Z8", "Z8", "Z8", "Z8")
            ITER256_ZIP_ZETAS(GATHER256_ZETAS_LVL2, "Z8", "Z8", "Z8", "Z8")
            ITER256_ZIP_ZETAS(GATHER256_ZETAS_LVL1, "Z8", "Z29", "Z8", "Z29")
            ITER256_ZIP_ZETAS(GATHER256_ZETAS_LVL0, "Z8", "Z25", "Z26", "Z29")
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP256
            "mov x8, xzr \n\ "
            STORE256
            : /**no output**/
            : [r] "r" (r), [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [zetas] "r" (zetas)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19",
            "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z29", "z30", "z31", "p0"
        );
    #endif
}

/*************************************************
* Name:        ntt_asm_512
*
* Description: The constant geometry NTT using inline assembly for vector length 512
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for an NTT level
**************************************************/
void ntt_asm_512(int16_t r[256], int16_t zetas_level[128]) {
    // Choose between a version where zetas is stored in memory and loaded in every vector register computation
    // (LOAD_ZETAS) or a version where zetas is zipped and put in the right registers (ZIP_ZETAS)
    #ifdef LOAD_ZETAS
        asm (
            // Initialize by loading elements of r into registers
            INIT(LOAD_R_512)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            ITER512_LOAD_ZETAS
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP512
            "mov x8, xzr \n\ "
            STORE512
            : /**no output**/
            : [r] "r" (r), [zetas_level] "r" (zetas_level), [qinv] "r" (-218038272),
            [kyber_q] "r" (-3329)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18",
            "z21", "z22", "z23", "z24", "z30", "z31", "p0"
        );
    #else // ZIP_ZETAS
        #ifndef ZIP_ZETAS
            printf("ERROR: Expected ZIP_ZETAS to be defined");
            exit(1);
        #endif
        UNUSED(zetas_level);  // Avoids warning about unused parameter

        asm (
            // Initialize by loading elements of r and zetas into registers
            INIT_R_AND_ZETAS(LOAD_R_512)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL6, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL5, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL4, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL3, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER512_ZETAS_LVL2, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL1, "Z8", "Z8")
            ITER512_ZIP_ZETAS(GATHER_ZETAS_LVL0, "Z8", "Z29")
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP512
            "mov x8, xzr \n\ "
            STORE512
            // "mov z8.h, #1 \n\
            // mov z9.h, #2 \n\
            // mov z10.d, z8.d \n\
            // mul z10.h, p0/m, z10.h, z9.h "
            : /**no output**/
            : [r] "r" (r), [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [zetas] "r" (zetas)
            : "memory", "x8", "x9", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18",
            "z21", "z22", "z23", "z24", "z30", "z31", "p0"
        );
    #endif
}

/*************************************************
* Name:        ntt_asm_1024
*
* Description: The constant geometry NTT using inline assembly for vector length 1024
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for an NTT level
**************************************************/
void ntt_asm_1024(int16_t r[256], int16_t zetas_level[128]) {
    // Choose between a version where zetas is stored in memory and loaded in every vector register computation
    // (LOAD_ZETAS) or a version where zetas is zipped and put in the right registers (ZIP_ZETAS)
    #ifdef LOAD_ZETAS
        asm (
            // Initialize by loading elements of r into registers
            INIT(LOAD_R_1024)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            ITER1024_LOAD_ZETAS
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP1024
            "mov x8, xzr \n\ "
            STORE1024
            : /**no output**/
            : [r] "r" (r), [zetas_level] "r" (zetas_level), [qinv] "r" (-218038272),
            [kyber_q] "r" (-3329)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z12", "z13", "z14", "z15",
            "z22", "z30", "z31", "p0"
        );
    #else // ZIP_ZETAS
        #ifndef ZIP_ZETAS
            printf("ERROR: Expected ZIP_ZETAS to be defined");
            exit(1);
        #endif
        UNUSED(zetas_level);  // Avoids warning about unused parameter

        asm (
            // Initialize by loading elements of r and zetas into registers
            INIT_R_AND_ZETAS(LOAD_R_1024)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER1024_ZIP_ZETAS(GATHER_ZETAS_LVL6, "Z8")
            ITER1024_ZIP_ZETAS(GATHER_ZETAS_LVL5, "Z8")
            ITER1024_ZIP_ZETAS(GATHER_ZETAS_LVL4, "Z8")
            ITER1024_ZIP_ZETAS(GATHER_ZETAS_LVL3, "Z8")
            ITER1024_ZIP_ZETAS(GATHER_ZETAS_LVL2, "Z8")
            ITER1024_ZIP_ZETAS(GATHER1024_ZETAS_LVL1, "Z8")
            ITER1024_ZIP_ZETAS(GATHER1024_ZETAS_LVL0, "Z8")
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP1024
            "mov x8, xzr \n\ "
            STORE1024
            : /**no output**/
            : [r] "r" (r), [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [zetas] "r" (zetas)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z10", "z12", "z13", "z14", "z15", "z22", "z29", "z30", "z31", "p0"
        );
    #endif
}

/*************************************************
* Name:        ntt_asm_2048
*
* Description: Theconstant geometry NTT using inline assembly for vector length 2048
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for an NTT level
**************************************************/
void ntt_asm_2048(int16_t r[256], int16_t zetas_level[128]) {
    // Choose between a version where zetas is stored in memory and loaded in every vector register computation
    // (LOAD_ZETAS) or a version where zetas is zipped and put in the right registers (ZIP_ZETAS)
    #ifdef LOAD_ZETAS
        asm (
            // Initialize by loading elements of r into registers
            INIT(LOAD_R_2048)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            ITER2048_LOAD_ZETAS
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP2048
            "mov x8, xzr \n\ "
            STORE2048
            : /**no output**/
            : [r] "r" (r), [zetas_level] "r" (zetas_level), [qinv] "r" (-218038272),
            [kyber_q] "r" (-3329)
            : "memory", "x8", "x10", "x11", "x13", "z0", "z1", "z8", "z12", "z13", "z14",
            "z15", "z30", "z31", "p0"
        );
    #else // ZIP_ZETAS
        #ifndef ZIP_ZETAS
            printf("ERROR: Expected ZIP_ZETAS to be defined");
            exit(1);
        #endif
        UNUSED(zetas_level);  // Avoids warning about unused parameter

        asm (
            // Initialize by loading elements of r and zetas into registers
            INIT_R_AND_ZETAS(LOAD_R_2048)
            // Iterate (levels 6-0)
            // Do a whole NTT by computing r_low and r_up (with ITER) and put them in registers (with ZIP)
            ITER2048_ZIP_ZETAS(GATHER_ZETAS_LVL6, "Z8")
            ITER2048_ZIP_ZETAS(GATHER_ZETAS_LVL5, "Z8")
            ITER2048_ZIP_ZETAS(GATHER_ZETAS_LVL4, "Z8")
            ITER2048_ZIP_ZETAS(GATHER_ZETAS_LVL3, "Z8")
            ITER2048_ZIP_ZETAS(GATHER_ZETAS_LVL2, "Z8")
            ITER2048_ZIP_ZETAS(GATHER2048_ZETAS_LVL1, "Z8")
            ITER2048_ZIP_ZETAS(GATHER2048_ZETAS_LVL0, "Z8")
            // Finish by zipping (to get elements in bitreversed order) and store to r
            ZIP2048
            "mov x8, xzr \n\ "
            STORE2048
            // STORE("Z29")
            : /**no output**/
            : [r] "r" (r), [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [zetas] "r" (zetas)
            : "memory", "x8", "x10", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
            "z7", "z8", "z9", "z12", "z13", "z14", "z15", "z29", "z30", "z31", "p0"
        );
    #endif
}

/*************************************************
* Name:        ntt
*
* Description: Constant geometry number-theoretic transform (NTT) in Rq
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void ntt(int16_t r[256]) {
    #ifdef GEM5_TEST_NTT
        m5_reset_stats(0,0);
    #endif

    #ifdef NTT_INPLACE
        ntt_level_in_place_c(r);
    #else
        int16_t *zetas_level;
        zetas_level = zetas_levels;

        #ifndef NTT_ASM_HAND
            unsigned int i;
            int16_t r2[256], *tmp, *p_r, *p_r2;
            p_r = r;
            p_r2 = r2;
        #endif

        #ifdef NTT_ASM_HAND
            (*ntt_asm)(r, zetas_level);
        #else
            // Levels 6-1
            for (i = 0; i < 6; i++) {
                #ifdef NTT_CONSTANT
                    ntt_level_constant_c(p_r, p_r2, zetas_level);
                #elif NTT_ASM_AUTO
                    ntt_level_asm_auto(p_r, p_r2, zetas_level);
                #endif
                zetas_level += 128;

                // swap(r, r2);
                tmp = p_r;
                p_r = p_r2;
                p_r2 = tmp;
            }
        #endif

        // Level 0
        #ifdef NTT_CONSTANT
            ntt_level_constant_c(p_r, p_r2, zetas_level);
        #elif NTT_ASM_AUTO
            ntt_level_asm_auto(p_r, p_r2, zetas_level);
        #endif
        #ifndef NTT_ASM_HAND
            // Shuffle r into the right order
            for (i = 0; i < KYBER_N; i++) {
                p_r[i] = p_r2[bit_reversed_order[i]];
            }
        #endif
    #endif

    #ifdef GEM5_TEST_NTT
        m5_dump_stats(0,0);
    #endif
}

/*************************************************
* Name:        invntt_in_place_c
*
* Description: Inplace inverse number-theoretic transform in Rq
*              input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void invntt_in_place_c(int16_t r[256]) {
  unsigned int start, len, j, k;
  int16_t t, zeta;

  k = 0;
  for(len = 2; len <= 128; len <<= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas_inv[k++];
      for(j = start; j < start + len; ++j) {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = t - r[j + len];
        r[j + len] = fqmul(zeta, r[j + len]);
      }
    }
  }

  for(j = 0; j < 256; ++j)
    r[j] = fqmul(r[j], zetas_inv[127]);
}


/*************************************************
* Name:        invntt_level_constant_c
*
* Description: One level of the INVNTT
*
* Arguments:   - int16_t *p_r: pointer to pointer to input vector of elements of Zq
*              - int16_t *p_r2: pointer to pointer to output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for NTT level
**************************************************/
void invntt_level_constant_c(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]) {
    unsigned int len, i;
    int16_t t, zeta, a;
    int32_t tmp_reduce, multiplied;
    int output_index;
    const int32_t v = (1U << 26)/KYBER_Q + 1;
    output_index = 0;
    len = 128;

    for(i = 0; i < 256; i+=2) {
        zeta = zetas_level[output_index];
        t = p_r2[i];

        // compute r_low
        // barrett_reduce
        a = t + p_r2[i+1];
        tmp_reduce = v * a;
        tmp_reduce >>= 26;
        tmp_reduce *= KYBER_Q;
        p_r[output_index] = a - tmp_reduce; //r_low

        // compute r_up
        p_r[output_index + len] = t - p_r2[i + 1];

        // montgomery_reduce
        multiplied = (int32_t) zeta * p_r[output_index + len];
        a = multiplied * QINV;
        tmp_reduce = (int32_t)a * KYBER_Q;
        tmp_reduce = multiplied - tmp_reduce;
        tmp_reduce >>= 16;

        p_r[output_index + len] = tmp_reduce; //r_up

        output_index++;
    }
}

/*************************************************
* Name:        invntt_level_asm_auto
*
* Description: One level of the constant geometry INVNTT using inline autogenerated assembly
*
* Arguments:   - int16_t *p_r: pointer to pointer to input vector of elements of Zq
*              - int16_t *p_r2: pointer to pointer to output vector of elements of Zq
*              - int16_t *zetas_level: pointer to index in zetas_levels for INVNTT level
**************************************************/
void invntt_level_asm_auto(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]) {
    unsigned long len = 128;

    asm (
      " mov	x8, xzr                                                                \n\
    	add	x10, %[p_r], #256                                                      \n\
    	ptrue	p0.s                                                               \n\
    	ptrue	p1.h                                                               \n\
    	whilelo	p2.h, xzr, %[len]                                                  \n\
    	mov	z0.s, %w[v]                                                            \n\
    	mov	z1.h, %w[kyber_q]                                                      \n\
    	mov	z2.s, %w[qinv]                                                         \n\
    	mov	z3.s, %w[kyber_q]                                                      \n\
    1:                                                                             \n\
    	lsl	x11, x8, #33                                                           \n\
    	add	x11, %[p_r2], x11, asr #31                                             \n\
    	ld1h	{ z4.h }, p2/z, [%[zetas_level], x8, lsl #1]                       \n\
    	ld2h	{ z5.h, z6.h }, p2/z, [x11]                                        \n\
    	sunpklo	z7.s, z4.h                                                         \n\
    	add	z16.h, z6.h, z5.h                                                      \n\
    	sub	z5.h, z5.h, z6.h                                                       \n\
    	sunpkhi	z6.s, z16.h                                                        \n\
    	sunpklo	z17.s, z16.h                                                       \n\
    	mul	z17.s, p0/m, z17.s, z0.s                                               \n\
    	mul	z6.s, p0/m, z6.s, z0.s                                                 \n\
    	asr	z6.s, z6.s, #26                                                        \n\
    	asr	z17.s, z17.s, #26                                                      \n\
    	uzp1	z6.h, z17.h, z6.h                                                  \n\
    	sunpkhi	z4.s, z4.h                                                         \n\
    	sunpklo	z17.s, z5.h                                                        \n\
    	sunpkhi	z5.s, z5.h                                                         \n\
    	mad	z6.h, p1/m, z1.h, z16.h                                                \n\
    	st1h	{ z6.h }, p2, [%[p_r], x8, lsl #1]                                 \n\
    	movprfx	z6, z5                                                             \n\
    	mul	z6.s, p0/m, z6.s, z4.s                                                 \n\
    	movprfx	z16, z17                                                           \n\
    	mul	z16.s, p0/m, z16.s, z7.s                                               \n\
    	mul	z16.s, p0/m, z16.s, z2.s                                               \n\
    	mul	z6.s, p0/m, z6.s, z2.s                                                 \n\
    	asr	z6.s, z6.s, #16                                                        \n\
    	asr	z16.s, z16.s, #16                                                      \n\
    	mul	z16.s, p0/m, z16.s, z3.s                                               \n\
    	mul	z6.s, p0/m, z6.s, z3.s                                                 \n\
    	mad	z4.s, p0/m, z5.s, z6.s                                                 \n\
    	movprfx	z5, z16                                                            \n\
    	mla	z5.s, p0/m, z17.s, z7.s                                                \n\
    	uzp2	z4.h, z5.h, z4.h                                                   \n\
    	st1h	{ z4.h }, p2, [x10, x8, lsl #1]                                    \n\
    	inch	x8                                                                 \n\
    	whilelo	p2.h, x8, %[len]                                                   \n\
    	b.mi	1b"
        : /**no output**/
        : [p_r] "r" (p_r), [p_r2] "r" (p_r2), [zetas_level] "r" (zetas_level), [len] "r" (len),
        [qinv] "r" (-218038272), [kyber_q] "r" (-3329), [v] "r" (20159)
        : "memory", "x8", "x9", "x10", "x11", "x12", "x13", "z0", "z1", "z2", "z3", "z4",
        "z5", "z6", "z7", "z16", "z17", "p0", "p1", "p2"
    );
}

/*************************************************
* Name:        invntt_asm_128
*
* Description: The constant geometry INVNTT using inline assembly for vector length 128
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
**************************************************/
void invntt_asm_128(int16_t r[256]) {
    int16_t r2[256];

    asm (
        // Initialize by loading elements of r into registers by ordering them into the right order
        INVINIT_R_AND_ZETAS(INVLOAD_R_128, INVORDER128_LEVEL6)

        // Do a whole NTT by computing r_low and r_up (with INVITER) and put them in registers (r2_low)
        // or memory (r2_up) (with UZP2)
        // Level 6 is different so it can put elements of r directly into the right registers without using
        // extra mov's in ITER as for INVORDER128
        // Level 6
        // Iterate by loading from registers and storing to registers (r2_low) / memory (r2_up)
        INVGATHER128_ZETAS_LVL6
        INVITER128_FROM_REG_LVL6("r2", "Z27", "Z8", "Z25", "Z26")
        // Iterate by loading from memory and storing to registers (r2_low) / memory (r2_up)
        " inch x13 \n\
        ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1] \n\ "
        INVGATHER128_ZETAS_LVL6
        INVITER128_FROM_MEM_LVL6("r", "r2", "Z27", "Z27", "Z8", "Z8", "Z25", "Z25", "Z26", "Z26")
        // Level 5-0
        INVITER128_ODD_LEVEL(INVGATHER128_ZETAS_LVL5, "Z27", "Z8", "Z25", "Z26")
        INVITER128_EVEN_LEVEL(INVGATHER128_ZETAS_LVL4, "Z27", "Z8")
        INVITER128_ODD_LEVEL(INVGATHER128_ZETAS_LVL3, "Z8", "Z8", "Z8", "Z8")
        INVITER128_EVEN_LEVEL(INVGATHER128_ZETAS_LVL2, "Z8", "Z8")
        INVITER128_ODD_LEVEL(INVGATHER128_ZETAS_LVL1, "Z8", "Z8", "Z8", "Z8")
        INVITER128_EVEN_LEVEL(INVGATHER128_ZETAS_LVL0, "Z8", "Z8")

        // Finish by doing montgomery_reduce(r*zetas_inv[127]) and storing the registers in array r in
        // memory at the right indices
        // montgomery_reduce(r*zetas_inv[127])
        "mov z8.s, %w[z127] \n\ "
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z21")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z22")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z23")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z24")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z9")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z10")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z11")
        MUL_MONT_ZETA("Z7")
        MUL_MONT_ZETA("Z16")
        //Store the elements in registers to r array in memory
        "mov x8, xzr \n\ "
        INVSTORE128_FROM_REG
        //Store the elements from temp array (r2) to r array in memory
        "mov x8, xzr                                        \n\
        add x10, %[r2], #256                                \n\ "
        INVLOAD_R2_128
        // montgomery_reduce(r*zetas_inv[127])
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z21")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z22")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z23")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z24")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z9")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z10")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z11")
        MUL_MONT_ZETA("Z7")
        MUL_MONT_ZETA("Z16")
        "mov x8, #128                                       \n\ "
        INVSTORE128_FROM_MEM
        : /**no output**/
        : [r] "r" (r), [r2] "r" (r2), [qinv] "r" (-218038272),
        [kyber_q] "r" (-3329), [zetas] "r" (zetas), [v] "r" (20159),
        [zetas_inv] "r" (zetas_inv), [z127] "r" (1441)
        : "memory", "x8", "x10", "x11", "x13", "x14", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
        "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19",
        "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31", "p0", "p1"
    );
}

/*************************************************
* Name:        invntt_asm_256
*
* Description: The constant geometry INVNTT using inline assembly for vector length 256
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
**************************************************/
void invntt_asm_256(int16_t r[256]) {

    asm (
        // Initialize by loading elements of r into registers by ordering them into the right order
        INVINIT_R_AND_ZETAS(INVLOAD_R_256, INVORDER256_LEVEL6)

        // Do a whole NTT by computing r_low and r_up (with INVITER) and put them in registers (with UZP2)
        // Level 6 is different so it can put elements of r directly into the right registers without using
        // extra mov's in ITER as for INVORDER256
        // Level 6
        INVGATHER256_ZETAS_LVL6
        INVITER("Z1", "Z0", "Z17", "Z18", "Z0", "Z27")
        UZP2("Z21", "Z17", "Z18")
        INVITER("Z3", "Z2", "Z17", "Z18", "Z1", "Z8")
        UZP2("Z22", "Z17", "Z18")
        INVITER("Z5", "Z4", "Z17", "Z18", "Z2", "Z25")
        UZP2("Z23", "Z17", "Z18")
        INVITER("Z7", "Z6", "Z17", "Z18", "Z3", "Z26")
        UZP2("Z24", "Z17", "Z18")
        INVITER("Z10", "Z9", "Z17", "Z18", "Z4", "Z27")
        UZP2("Z9", "Z17", "Z18")
        INVITER("Z12", "Z11", "Z17", "Z18", "Z5", "Z8")
        UZP2("Z10", "Z17", "Z18")
        INVITER("Z14", "Z13", "Z17", "Z18", "Z6", "Z25")
        UZP2("Z11", "Z17", "Z18")
        INVITER("Z16", "Z15", "Z17", "Z18", "Z7", "Z26")
        UZP2("Z16", "Z17", "Z18")
        // Levels 5-0
        INVITER256(INVGATHER256_ZETAS_LVL5, "Z8", "Z27")
        INVITER256(INVGATHER256_ZETAS_LVL4, "Z8", "Z8")
        INVITER256(INVGATHER256_ZETAS_LVL3, "Z8", "Z8")
        INVITER256(INVGATHER256_ZETAS_LVL2, "Z8", "Z8")
        INVITER256(INVGATHER256_ZETAS_LVL1, "Z8", "Z8")
        INVITER256(INVGATHER256_ZETAS_LVL0, "Z8", "Z8")

        // Finish by doing montgomery_reduce(r*zetas_inv[127]) and storing the registers in array r in
        // memory at the right indices
        // montgomery_reduce(r*zetas_inv[127])
        "mov z8.s, %w[z127] \n\ "
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z21")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z22")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z23")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z24")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z9")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z10")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z11")
        MUL_MONT_ZETA("Z7")
        MUL_MONT_ZETA("Z16")
        "mov x8, xzr \n\ "
        INVSTORE256
        : /**no output**/
        : [r] "r" (r), [zetas] "r" (zetas), [qinv] "r" (-218038272),
        [kyber_q] "r" (-3329), [v] "r" (20159),
        [zetas_inv] "r" (zetas_inv), [z127] "r" (1441)
        : "memory", "x8", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
        "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19",
        "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31", "p0", "p1"
    );
}

/*************************************************
* Name:        invntt_asm_512
*
* Description: The constant geometry INVNTT using inline assembly for vector length 512
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
**************************************************/
void invntt_asm_512(int16_t r[256]) {

    asm (
        // Initialize by loading elements of r into registers by ordering them into the right order
        INVINIT_R_AND_ZETAS(INVLOAD_R_512, INVORDER512_LEVEL6)

        // Do a whole NTT by computing r_low and r_up (with INVITER) and put them in registers (with UZP2)
        // Level 6 is different so it can put elements of r directly into the right registers without using
        // extra mov's in ITER as for INVORDER512
        // Level 6
        INVGATHER_ZETAS_LVL6
        INVITER("Z1", "Z0", "Z21", "Z22", "Z0", "Z27")
        INVITER("Z3", "Z2", "Z23", "Z24", "Z1", "Z8")
        INVITER("Z5", "Z4", "Z25", "Z26", "Z2", "Z27")
        UZP2("Z4", "Z21", "Z22")
        UZP2("Z5", "Z23", "Z24")
        INVITER("Z7", "Z6", "Z21", "Z22", "Z3", "Z8")
        UZP2("Z6", "Z25", "Z26")
        UZP2("Z7", "Z21", "Z22")
        // Levels 5-0
        INVITER512(INVGATHER_ZETAS_LVL5)
        INVITER512(INVGATHER_ZETAS_LVL4)
        INVITER512(INVGATHER_ZETAS_LVL3)
        INVITER512(INVGATHER_ZETAS_LVL2)
        INVITER512(INVGATHER_ZETAS_LVL1)
        INVITER512(INVGATHER_ZETAS_LVL0)

        // Finish by doing montgomery_reduce(r*zetas_inv[127]) and storing the registers in array r in
        // memory at the right indices
        // montgomery_reduce(r*zetas_inv[127])
        "mov z8.s, %w[z127] \n\ "
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z7")
        "mov x8, xzr \n\ "
        INVSTORE512
        : /**no output**/
        : [r] "r" (r), [zetas] "r" (zetas), [qinv] "r" (-218038272),
        [kyber_q] "r" (-3329), [v] "r" (20159),
        [zetas_inv] "r" (zetas_inv), [z127] "r" (1441)
        : "memory", "x8", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
        "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14", "z15", "z17", "z18", "z19",
        "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31", "p0", "p1"
    );
}

/*************************************************
* Name:        invntt_asm_1024
*
* Description: The constant geometry INVNTT using inline assembly for vector length 1024
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
**************************************************/
void invntt_asm_1024(int16_t r[256]) {

    asm (
        // Initialize by loading elements of r into registers by ordering them into the right order
        INVINIT_R_AND_ZETAS(INVLOAD_R_1024, INVORDER1024_LEVEL6)

        // Do a whole NTT by computing r_low and r_up (with INVITER) and put them in registers (with UZP2)
        // Level 6 is different so it can put elements of r directly into the right registers without using
        // extra mov's in ITER as for INVORDER1024
        // Level 6
        INVITER("Z1", "Z0", "Z21", "Z22", "Z0", "Z27")
        INVITER("Z3", "Z2", "Z23", "Z24", "Z1", "Z27")
        UZP2("Z2", "Z21", "Z22")
        UZP2("Z3", "Z23", "Z24")
        // Levels 5-0
        INVITER1024(INVGATHER1024_ZETAS_LVL5)
        INVITER1024(INVGATHER1024_ZETAS_LVL4)
        INVITER1024(INVGATHER1024_ZETAS_LVL3)
        INVITER1024(INVGATHER1024_ZETAS_LVL2)
        INVITER1024(INVGATHER1024_ZETAS_LVL1)
        INVITER1024(INVGATHER1024_ZETAS_LVL0)

        // Finish by doing montgomery_reduce(r*zetas_inv[127]) and storing the registers in array r in
        // memory at the right indices
        // montgomery_reduce(r*zetas_inv[127])
        "mov z8.s, %w[z127] \n\ "
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z7")
        "mov x8, xzr \n\ "
        INVSTORE1024
        : /**no output**/
        : [r] "r" (r), [zetas] "r" (zetas), [qinv] "r" (-218038272),
        [kyber_q] "r" (-3329), [zetas_inv_levels] "r" (zetas_inv_levels), [v] "r" (20159),
        [zetas_inv] "r" (zetas_inv), [z127] "r" (1441)
        : "memory", "x8", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
        "z7", "z8", "z9", "z10", "z11", "z12", "z17", "z18", "z19",
        "z20", "z21", "z22", "z23", "z24", "z27", "z28", "z29", "z30", "z31", "p0", "p1"
    );
}

/*************************************************
* Name:        invntt_asm_2048
*
* Description: The constant geometry INVNTT using inline assembly for vector length 1024
*
* Arguments:   - int16_t *r: pointer to pointer to input/output vector of elements of Zq
**************************************************/
void invntt_asm_2048(int16_t r[256]) {

    asm (
        // Initialize by loading elements of r into registers by ordering them into the right order
        INVINIT_R(INVLOAD_R_2048, INVORDER2048_LEVEL6)

        // Do a whole NTT by computing r_low and r_up (with INVITER) and put them in registers (with UZP2)
        // Level 6 is different so it can put elements of r directly into the right registers without using
        // extra mov's in ITER as for INVORDER2048
        // Level 6
        INVGATHER2048_ZETAS_LVL6
        INVITER_LOAD_ZETAS("Z1", "Z0", "Z21", "Z22", "Z0")
        UZP2("Z1", "Z21", "Z22")
        // Levels 5-0
        INVITER2048
        INVITER2048
        INVITER2048
        INVITER2048
        INVITER2048
        INVITER2048

        // Finish by doing montgomery_reduce(r*zetas_inv[127]) and storing the registers in array r in
        // memory at the right indices
        // montgomery_reduce(r*zetas_inv[127])
        "mov z8.s, %w[z127] \n\ "
        MUL_MONT_ZETA("Z0")
        MUL_MONT_ZETA("Z1")
        MUL_MONT_ZETA("Z2")
        MUL_MONT_ZETA("Z3")
        MUL_MONT_ZETA("Z4")
        MUL_MONT_ZETA("Z5")
        MUL_MONT_ZETA("Z6")
        MUL_MONT_ZETA("Z7")
        "mov x8, xzr \n\ "
        INVSTORE2048
        : /**no output**/
        : [r] "r" (r), [zetas] "r" (zetas), [qinv] "r" (-218038272),
        [kyber_q] "r" (-3329), [zetas_inv_levels] "r" (zetas_inv_levels), [v] "r" (20159),
        [zetas_inv] "r" (zetas_inv), [z127] "r" (1441)
        : "memory", "x8", "x13", "z0", "z1", "z2", "z3", "z4", "z5", "z6",
        "z7", "z8", "z9", "z12", "z17", "z18", "z19",
        "z20", "z21", "z22", "z27", "z28", "z29", "z30", "z31", "p0", "p1"
    );
}

/*************************************************
* Name:        invntt
*
* Description: Inverse number-theoretic transform in Rq
*              input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
    #ifdef GEM5_TEST_INVNTT
        m5_reset_stats(0,0);
    #endif

    #ifdef INVNTT_INPLACE
        invntt_in_place_c(r);
    #else
        #ifdef INVNTT_ASM_HAND
            (*invntt_asm)(r);
        #else
            unsigned int i;
            int16_t r2[256], *zetas_level, *tmp, *p_r, *p_r2;
            zetas_level = zetas_inv_levels;
            p_r = r;
            p_r2 = r2;

            // Shuffle r into the right order
            for (i = 0; i < 256; i++) {
                p_r2[i] = p_r[invntt_order[i]];
            }

            // Levels 6-1
            for (int level = 0; level < 6; level++) {
                #ifdef INVNTT_CONSTANT
                    invntt_level_constant_c(p_r, p_r2, zetas_level);
                #elif INVNTT_ASM_AUTO
                    invntt_level_asm_auto(p_r, p_r2, zetas_level);
                #endif
                zetas_level += 128;

                // swap(r, r2);
                tmp = p_r;
                p_r = p_r2;
                p_r2 = tmp;
            }

            // Level 0
            #ifdef INVNTT_CONSTANT
                invntt_level_constant_c(p_r, p_r2, zetas_level);
            #elif INVNTT_ASM_AUTO
                invntt_level_asm_auto(p_r, p_r2, zetas_level);
            #endif

            int32_t multiplied, t;
            int16_t u;
            for(int i = 0; i < 256; ++i) {
                multiplied = r[i] * 1441;
                u = multiplied * QINV;
                t = (int32_t)u * KYBER_Q;
                t = multiplied - t;
                t >>= 16;
                r[i] = t;
            }
        #endif
    #endif

    #ifdef GEM5_TEST_INVNTT
        m5_dump_stats(0,0);
    #endif
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/((X^2-zeta))
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[2]: pointer to the output polynomial
*              - const int16_t a[2]: pointer to the first factor
*              - const int16_t b[2]: pointer to the second factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
#ifndef POLY_BASEMUL_ASM
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta) {
    r[0]  = fqmul(a[1], b[1]);
    r[0]  = fqmul(r[0], zeta);
    r[0] += fqmul(a[0], b[0]);

    r[1]  = fqmul(a[0], b[1]);
    r[1] += fqmul(a[1], b[0]);
}
#endif
