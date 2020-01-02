#include <stdint.h>
#include <stdio.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - unsigned char *r: pointer to output byte array (needs space for KYBER_POLYCOMPRESSEDBYTES bytes)
*              - const poly *a:    pointer to input polynomial
**************************************************/
void poly_compress(unsigned char *r, poly *a)
{
  uint8_t t[8];
  int i,j,k=0;

  poly_csubq(a);

#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 3) + KYBER_Q/2) / KYBER_Q) & 7;

    r[k]   =  t[0]       | (t[1] << 3) | (t[2] << 6);
    r[k+1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[k+2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    k += 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 4) + KYBER_Q/2) / KYBER_Q) & 15;

    r[k]   = t[0] | (t[1] << 4);
    r[k+1] = t[2] | (t[3] << 4);
    r[k+2] = t[4] | (t[5] << 4);
    r[k+3] = t[6] | (t[7] << 4);
    k += 4;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<KYBER_N;i+=8)
  {
    for(j=0;j<8;j++)
      t[j] = ((((uint32_t)a->coeffs[i+j] << 5) + KYBER_Q/2) / KYBER_Q) & 31;

    r[k]   =  t[0]       | (t[1] << 5);
    r[k+1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
    r[k+2] = (t[3] >> 1) | (t[4] << 4);
    r[k+3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
    r[k+4] = (t[6] >> 2) | (t[7] << 3);
    k += 5;
  }
#else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {96, 128, 160}"
#endif
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r:                pointer to output polynomial
*              - const unsigned char *a: pointer to input byte array (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress(poly *r, const unsigned char *a)
{
  int i;
#if (KYBER_POLYCOMPRESSEDBYTES == 96)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] =  (((a[0] & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+1] = ((((a[0] >> 3) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+2] = ((((a[0] >> 6) | ((a[1] << 2) & 4)) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+3] = ((((a[1] >> 1) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+4] = ((((a[1] >> 4) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+5] = ((((a[1] >> 7) | ((a[2] << 1) & 6)) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+6] = ((((a[2] >> 2) & 7) * KYBER_Q) + 4) >> 3;
    r->coeffs[i+7] = ((((a[2] >> 5)) * KYBER_Q) + 4) >> 3;
    a += 3;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 128)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] = (((a[0] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+1] = (((a[0] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+2] = (((a[1] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+3] = (((a[1] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+4] = (((a[2] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+5] = (((a[2] >> 4) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+6] = (((a[3] & 15) * KYBER_Q) + 8) >> 4;
    r->coeffs[i+7] = (((a[3] >> 4) * KYBER_Q) + 8) >> 4;
    a += 4;
  }
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
  for(i=0;i<KYBER_N;i+=8)
  {
    r->coeffs[i+0] =  (((a[0] & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+1] = ((((a[0] >> 5) | ((a[1] & 3) << 3)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+2] = ((((a[1] >> 2) & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+3] = ((((a[1] >> 7) | ((a[2] & 15) << 1)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+4] = ((((a[2] >> 4) | ((a[3] &  1) << 4)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+5] = ((((a[3] >> 1) & 31) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+6] = ((((a[3] >> 6) | ((a[4] &  7) << 2)) * KYBER_Q) + 16) >> 5;
    r->coeffs[i+7] =  (((a[4] >> 3) * KYBER_Q) + 16) >> 5;
    a += 5;
  }
#else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {96, 128, 160}"
#endif
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - unsigned char *r: pointer to output byte array (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a:    pointer to input polynomial
**************************************************/
void poly_tobytes(unsigned char *r, poly *a)
{
  int i;
  uint16_t t0, t1;

  poly_csubq(a);

  for(i=0;i<KYBER_N/2;i++){
    t0 = a->coeffs[2*i];
    t1 = a->coeffs[2*i+1];
    r[3*i] = t0 & 0xff;
    r[3*i+1] = (t0 >> 8) | ((t1 & 0xf) << 4);
    r[3*i+2] = t1 >> 4;
  }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r:                pointer to output polynomial
*              - const unsigned char *a: pointer to input byte array (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(poly *r, const unsigned char *a)
{
  int i;

  for(i=0;i<KYBER_N/2;i++){
    r->coeffs[2*i]   = a[3*i]        | ((uint16_t)a[3*i+1] & 0x0f) << 8;
    r->coeffs[2*i+1] = a[3*i+1] >> 4 | ((uint16_t)a[3*i+2] & 0xff) << 4;
  }
}

/*************************************************
* Name:        poly_getnoise
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA
*
* Arguments:   - poly *r:                   pointer to output polynomial
*              - const unsigned char *seed: pointer to input seed (pointing to array of length KYBER_SYMBYTES bytes)
*              - unsigned char nonce:       one-byte input nonce
**************************************************/
void poly_getnoise(poly *r, const unsigned char *seed, unsigned char nonce)
{
  unsigned char buf[KYBER_ETA*KYBER_N/4];

  prf(buf, KYBER_ETA*KYBER_N/4, seed, nonce);
  cbd(r, buf);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void poly_ntt(poly *r)
{
  ntt(r->coeffs);
  poly_reduce(r);
}

/*************************************************
* Name:        poly_invntt
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt(poly *r)
{
  invntt(r->coeffs);
}

/*************************************************
* Name:        poly_basemul
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_basemul(poly *r, const poly *a, const poly *b)
{
    #ifdef GEM5_TEST_POLY_BASEMUL
        m5_reset_stats(0,0);
    #endif
    // Vectorized version
    #ifdef POLY_BASEMUL_ASM
        int16_t *zeta = &zetas[64];

        asm (
            "mov x8, %[zeta]                                                            \n\
            mov	z0.s, %w[qinv]                                                          \n\
            rdvl	x5, #1                                                              \n\
            mov	z1.s, %w[kyber_q]                                                       \n\
            lsr	x5, x5, #4                                                              \n\
            mov	w9, wzr                                                                 \n\
            mov	x10, xzr                                                                \n\
            add	x12, %[a], #2             // =2                                         \n\
            add	x13, %[b], #2             // =2                                         \n\
            add	x14, %[r], #2             // =2                                         \n\
            add	x15, %[a], #6             // =6                                         \n\
            add	x16, %[b], #6             // =6                                         \n\
            add	x17, %[r], #4             // =4                                         \n\
            add	x18, %[a], #4             // =4                                         \n\
            add	x3, %[b], #4              // =4                                         \n\
            add	x4, %[r], #6              // =6                                         \n\
            ptrue	p0.s                                                                \n\
            pfalse	p3.b                                                                \n\
            whilelo	p1.h, xzr, %[kyber_n_4]                                             \n\
            mov	z2.h, #0                // =0x0                                         \n\
        1:                                // =>This Inner Loop Header: Depth=1          \n\
            ld1h	{ z4.h }, p1/z, [x8, x10, lsl #1]                                   \n\
            add	x7, x8, x10, lsl #1                                                     \n\
            addvl	x7, x9, #1                                                          \n\
            index	z3.s, w9, #4                                                        \n\
            index	z5.s, w7, #4                                                        \n\
            zip2	p2.h, p1.h, p3.h                                                    \n\
            zip1	p1.h, p1.h, p3.h                                                    \n\ "
            BASEMUL_PART1("x12", "x13")           // a[i+1] * b[i+1] && r[i] * zeta
            BASEMUL_PART2("%[r]", "%[a]", "%[b]") // a[i] * b[i]
            BASEMUL_PART3("x14")                  // a[i] * b[i+1] && a[i+1] * b[i]
        	"sub	z4.h, z2.h, z4.h              //-zeta                               \n\ "
            BASEMUL_PART1("x15", "x16")           // a[i+3] * b[i+3] && r[i+2] * zeta
            BASEMUL_PART2("x17", "x18", "x3")     // a[i+2] * b[i+2]
            BASEMUL_PART3("x4")                   // a[i+2] * b[i+3] && a[i+3] * b[i+2]
            "add	w9, w9, w5, lsl #5                                                  \n\
            inch	x10                                                                 \n\
            whilelo	p1.h, x10, %[kyber_n_4]                                             \n\
            b.mi	1b "
          : /**no output**/
          : [r] "r" (&(r->coeffs)), [a] "r" (&(a->coeffs)), [b] "r" (&(b->coeffs)),
          [kyber_q] "r" (-3329), [qinv] "r" (-218038272), [kyber_n_4] "r" (64), [zeta] "r" (zeta)
          : "memory", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
          "x16", "x17", "x18", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10",
          "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23",
          "z24", "z25", "z26", "z29", "z30", "z31", "p0", "p1", "p2", "p3"
        );
    // C reference
    #else
        unsigned int i;

        for(i = 0; i < KYBER_N/4; ++i) {
            basemul(r->coeffs + 4*i, a->coeffs + 4*i, b->coeffs + 4*i, zetas[64 + i]);
            basemul(r->coeffs + 4*i + 2, a->coeffs + 4*i + 2, b->coeffs + 4*i + 2, -zetas[64 + i]);
        }
    #endif

    #ifdef GEM5_TEST_POLY_BASEMUL
        m5_dump_stats(0,0);
    #endif
}

/*************************************************
* Name:        poly_frommont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from Montgomery domain to normal domain
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void poly_frommont(poly *r)
{
  int i;
  const int16_t f = (1ULL << 32) % KYBER_Q;

  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = montgomery_reduce((int32_t)r->coeffs[i]*f);
}

/*************************************************
* Name:        poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *r)
{
    // Vectorized version
    #ifdef POLY_REDUCE_ASM
        asm (
            "mov	x8, xzr                                                            \n\
        	ptrue	p0.s                                                               \n\
        	whilelo	p2.h, xzr, %[kyber_n]                                              \n\
        	mov	z0.s, %w[v]                                                            \n\
        	mov	z1.h, %w[kyber_q]                                                      \n\
        	ptrue	p1.h                                                               \n\
          1:                                // =>This Inner Loop Header: Depth=1       \n\
        	ld1h	{ z2.h }, p2/z, [%[r], x8, lsl #1]                                 \n\
        	sunpkhi	z3.s, z2.h                                                         \n\
        	sunpklo	z4.s, z2.h                                                         \n\
        	mul	z4.s, p0/m, z4.s, z0.s                                                 \n\
        	mul	z3.s, p0/m, z3.s, z0.s                                                 \n\
        	asr	z3.s, z3.s, #26                                                        \n\
        	asr	z4.s, z4.s, #26                                                        \n\
        	uzp1	z3.h, z4.h, z3.h                                                   \n\
        	mla	z2.h, p1/m, z3.h, z1.h                                                 \n\
        	st1h	{ z2.h }, p2, [%[r], x8, lsl #1]                                   \n\
        	inch	x8                                                                 \n\
        	whilelo	p2.h, x8, %[kyber_n]                                               \n\
        	b.mi	1b"
            : /**no output**/
            : [r] "r" (&(r->coeffs[0])), [kyber_q] "r" (-3329), [v] "r" (20159), [kyber_n] "r" (KYBER_N)
            : "memory", "x8", "x9", "x10", "z0", "z1", "z2", "z3", "z4", "p0", "p1", "p2"
        );
    //C reference
    #else
        int i;
        for(i=0;i<KYBER_N;i++)
          r->coeffs[i] = barrett_reduce(r->coeffs[i]);
    #endif
}

/*************************************************
* Name:        poly_csubq
*
* Description: Applies conditional subtraction of q to each coefficient of a polynomial
*              for details of conditional subtraction of q see comments in reduce.c
*
* Arguments:   - poly *r:       pointer to input/output polynomial
**************************************************/
void poly_csubq(poly *r)
{
  int i;

  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = csubq(r->coeffs[i]);
}

/*************************************************
* Name:        poly_add
*
* Description: Add two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_add(poly *r, const poly *a, const poly *b)
{
  int i;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_sub(poly *r, const poly *a, const poly *b)
{
  int i;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r:                  pointer to output polynomial
*              - const unsigned char *msg: pointer to input message
**************************************************/
void poly_frommsg(poly *r, const unsigned char msg[KYBER_SYMBYTES])
{
  int i,j;
  uint16_t mask;

  for(i=0;i<KYBER_SYMBYTES;i++)
  {
    for(j=0;j<8;j++)
    {
      mask = -((msg[i] >> j)&1);
      r->coeffs[8*i+j] = mask & ((KYBER_Q+1)/2);
    }
  }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - unsigned char *msg: pointer to output message
*              - const poly *a:      pointer to input polynomial
**************************************************/
void poly_tomsg(unsigned char msg[KYBER_SYMBYTES], poly *a)
{
  uint16_t t;
  int i,j;

  poly_csubq(a);

  for(i=0;i<KYBER_SYMBYTES;i++)
  {
    msg[i] = 0;
    for(j=0;j<8;j++)
    {
      t = (((a->coeffs[8*i+j] << 1) + KYBER_Q/2) / KYBER_Q) & 1;
      msg[i] |= t << j;
    }
  }
}
