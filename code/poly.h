#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct{
  int16_t coeffs[KYBER_N];
} poly;

void poly_compress(unsigned char *r, poly *a);
void poly_decompress(poly *r, const unsigned char *a);

void poly_tobytes(unsigned char *r, poly *a);
void poly_frombytes(poly *r, const unsigned char *a);

void poly_frommsg(poly *r, const unsigned char msg[KYBER_SYMBYTES]);
void poly_tomsg(unsigned char msg[KYBER_SYMBYTES], poly *r);

void poly_getnoise(poly *r,const unsigned char *seed, unsigned char nonce);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_basemul(poly *r, const poly *a, const poly *b);
void poly_frommont(poly *r);

void poly_reduce(poly *r);
void poly_csubq(poly *r);

void poly_add(poly *r, const poly *a, const poly *b);
void poly_sub(poly *r, const poly *a, const poly *b);

#define BASEMUL_PART1(src1, src2)\
    "ld1sh	{ z6.s }, p2/z, ["src1", z5.s, sxtw #1]                            \n\
    ld1sh	{ z7.s }, p1/z, ["src1", z3.s, sxtw #1]                            \n\
    ld1sh	{ z16.s }, p2/z, ["src2", z5.s, sxtw #1]                           \n\
    ld1sh	{ z17.s }, p1/z, ["src2", z3.s, sxtw #1]                           \n\
    uzp1	z6.h, z7.h, z6.h                                                   \n\
    uzp1	z7.h, z17.h, z16.h                                                 \n\
    sunpklo	z18.s, z4.h                                                        \n\
    sunpkhi	z19.s, z4.h                                                        \n\
    sunpklo	z16.s, z6.h                                                        \n\
    sunpkhi	z6.s, z6.h                                                         \n\
    sunpklo	z17.s, z7.h                                                        \n\
    sunpkhi	z7.s, z7.h                                                         \n\
    movprfx	z20, z7                                                            \n\
    mul	z20.s, p0/m, z20.s, z6.s                                               \n\
    movprfx	z21, z17                                                           \n\
    mul	z21.s, p0/m, z21.s, z16.s                                              \n\
    mul	z21.s, p0/m, z21.s, z0.s                                               \n\
    mul	z20.s, p0/m, z20.s, z0.s                                               \n\
    asr	z20.s, z20.s, #16                                                      \n\
    asr	z21.s, z21.s, #16                                                      \n\
    mul	z21.s, p0/m, z21.s, z1.s                                               \n\
    mul	z20.s, p0/m, z20.s, z1.s                                               \n\
    mla	z20.s, p0/m, z7.s, z6.s                                                \n\
    mla	z21.s, p0/m, z17.s, z16.s                                              \n\
    asr	z21.s, z21.s, #16                                                      \n\
    asr	z20.s, z20.s, #16                                                      \n\
    movprfx	z22, z20                                                           \n\
    mul	z22.s, p0/m, z22.s, z19.s                                              \n\
    movprfx	z23, z21                                                           \n\
    mul	z23.s, p0/m, z23.s, z18.s                                              \n\
    mul	z23.s, p0/m, z23.s, z0.s                                               \n\
    mul	z22.s, p0/m, z22.s, z0.s                                               \n\
    asr	z22.s, z22.s, #16                                                      \n\
    asr	z23.s, z23.s, #16                                                      \n\
    mul	z23.s, p0/m, z23.s, z1.s                                               \n\
    mul	z22.s, p0/m, z22.s, z1.s                                               \n\
    mad	z19.s, p0/m, z20.s, z22.s                                              \n\
    mad	z18.s, p0/m, z21.s, z23.s                                              \n\
    uzp2	z18.h, z18.h, z19.h                                                \n\ "

#define BASEMUL_PART2(dest, src1, src2)\
    "ld1sh	{ z20.s }, p2/z, ["src1", z5.s, sxtw #1]                           \n\
    ld1sh	{ z21.s }, p1/z, ["src1", z3.s, sxtw #1]                           \n\
    ld1sh	{ z22.s }, p2/z, ["src2", z5.s, sxtw #1]                           \n\
    ld1sh	{ z23.s }, p1/z, ["src2", z3.s, sxtw #1]                           \n\
    uzp1	z19.h, z21.h, z20.h                                                \n\
    uzp1	z20.h, z23.h, z22.h                                                \n\
    uunpkhi	z24.s, z18.h                                                       \n\
    sunpklo	z21.s, z19.h                                                       \n\
    sunpkhi	z19.s, z19.h                                                       \n\
    sunpklo	z22.s, z20.h                                                       \n\
    sunpkhi	z20.s, z20.h                                                       \n\
    movprfx	z23, z20                                                           \n\
    mul	z23.s, p0/m, z23.s, z19.s                                              \n\
    movprfx	z24, z22                                                           \n\
    mul	z24.s, p0/m, z24.s, z21.s                                              \n\
    mul	z24.s, p0/m, z24.s, z0.s                                               \n\
    mul	z23.s, p0/m, z23.s, z0.s                                               \n\
    asr	z23.s, z23.s, #16                                                      \n\
    asr	z24.s, z24.s, #16                                                      \n\
    mul	z24.s, p0/m, z24.s, z1.s                                               \n\
    mul	z23.s, p0/m, z23.s, z1.s                                               \n\
    mla	z23.s, p0/m, z20.s, z19.s                                              \n\
    mla	z24.s, p0/m, z22.s, z21.s                                              \n\
    uzp2	z23.h, z24.h, z23.h                                                \n\
    add	z18.h, z18.h, z23.h                                                    \n\
    uunpklo	z23.s, z18.h                                                       \n\
    uunpkhi	z18.s, z18.h                                                       \n\
    st1h	{ z23.s }, p1, ["dest", z3.s, sxtw #1]                             \n\
    st1h	{ z18.s }, p2, ["dest", z5.s, sxtw #1]                             \n\ "

#define BASEMUL_PART3(dest)\
    "movprfx	z18, z7                                                        \n\
    mul	z18.s, p0/m, z18.s, z19.s                                              \n\
    movprfx	z23, z17                                                           \n\
    mul	z23.s, p0/m, z23.s, z21.s                                              \n\
    mul	z23.s, p0/m, z23.s, z0.s                                               \n\
    mul	z18.s, p0/m, z18.s, z0.s                                               \n\
    asr	z18.s, z18.s, #16                                                      \n\
    asr	z23.s, z23.s, #16                                                      \n\
    mul	z23.s, p0/m, z23.s, z1.s                                               \n\
    mul	z18.s, p0/m, z18.s, z1.s                                               \n\
    mad	z7.s, p0/m, z19.s, z18.s                                               \n\
    mad	z17.s, p0/m, z21.s, z23.s                                              \n\
    uzp2	z7.h, z17.h, z7.h                                                  \n\
    uunpklo	z17.s, z7.h                                                        \n\
    uunpkhi	z18.s, z7.h                                                        \n\
    movprfx	z17, z20                                                           \n\
    mul	z17.s, p0/m, z17.s, z6.s                                               \n\
    movprfx	z18, z22                                                           \n\
    mul	z18.s, p0/m, z18.s, z16.s                                              \n\
    mul	z18.s, p0/m, z18.s, z0.s                                               \n\
    mul	z17.s, p0/m, z17.s, z0.s                                               \n\
    asr	z17.s, z17.s, #16                                                      \n\
    asr	z18.s, z18.s, #16                                                      \n\
    mul	z18.s, p0/m, z18.s, z1.s                                               \n\
    mul	z17.s, p0/m, z17.s, z1.s                                               \n\
    mad	z6.s, p0/m, z20.s, z17.s                                               \n\
    mad	z16.s, p0/m, z22.s, z18.s                                              \n\
    uzp2	z6.h, z16.h, z6.h                                                  \n\
    add	z6.h, z6.h, z7.h                                                       \n\
    uunpklo	z7.s, z6.h                                                         \n\
    uunpkhi	z6.s, z6.h                                                         \n\
    st1h	{ z7.s }, p1, ["dest", z3.s, sxtw #1]                              \n\
    st1h	{ z6.s }, p2, ["dest", z5.s, sxtw #1]                              \n\ "

#endif
