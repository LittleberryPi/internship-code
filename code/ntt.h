#ifndef NTT_H
#define NTT_H

#include <stdint.h>

/************************** Macros for inline assembly ***************************/
// Zipping macros
#define ZIP1(dest, src1, src2)\
		" zip1 "dest".h, "src1".h, "src2".h \n\ "
#define ZIP2(dest, src1, src2)\
		" zip2 "dest".h, "src1".h, "src2".h \n\ "
// Combination of zip1 and zip2, zip src1 and src2 and put the results in dest1 and dest2
#define ZIP12(dest1, dest2, src1, src2)\
    " zip1 "dest1".h, "src1".h, "src2".h     \n\
      zip2 "dest2".h, "src1".h, "src2".h     \n\ "

// Store from src register to the temporary array (dest) with predicate p0
#define STORE_TEMP(src, dest)\
    " st1h "src".h, p0, [%["dest"], x14, lsl #1] \n\
      inch x14                                   \n\ "
// Store from src register to the temporary array (dest) without inch at the end with predicate p0
#define STORE_TEMP_LAST(src, dest)\
    " st1h "src".h, p0, [%["dest"], x14, lsl #1]  \n\ "

// Store from src register to r with predicate p0
#define STORE(src)\
    " st1h "src".h, p0, [%[r], x8, lsl #1]   \n\
      inch x8                                \n\ "
// Store to r without inch at the end with predicate p0
#define STORE_LAST(src)\
    " st1h "src".h, p0, [%[r], x8, lsl #1]  \n\ "

// Store from src register to r with predicate p1
#define INVSTORE(src)\
    " st1h "src".h, p1, [%[r], x8, lsl #1]   \n\
      inch x8                                \n\ "
// Store to r without inch at the end with predicate p1
#define INVSTORE_LAST(src)\
    " st1h "src".h, p1, [%[r], x8, lsl #1]   \n\ "

// Store from src register to the temporary array (dest) with predicate p1
#define INVSTORE_TEMP(src, dest)\
    " st1h "src".h, p1, [%["dest"], x14, lsl #1] \n\
      inch x14                                   \n\ "
// Store to the temporary array (dest) without inch at the end with predicate p1
#define INVSTORE_TEMP_LAST(src, dest)\
    " st1h "src".h, p1, [%["dest"], x14, lsl #1]  \n\ "

extern int16_t zetas[];
extern int16_t zetas_inv[];

void (*ntt_asm)(int16_t*, int16_t*);
void (*invntt_asm)(int16_t*);

void ntt(int16_t *poly);
void invntt(int16_t *poly);
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

void invntt_level_c(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]);
void invntt_level_asm_auto(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]);
void invntt_asm_128(int16_t r[256]);
void invntt_asm_256(int16_t r[256]);
void invntt_asm_512(int16_t r[256]);
void invntt_asm_1024(int16_t r[256]);
void invntt_asm_2048(int16_t r[256]);
void ntt_level_c(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]);
void ntt_level_asm_auto(int16_t p_r[256], int16_t p_r2[256], int16_t zetas_level[128]);
void ntt_asm_128(int16_t r[256], int16_t zetas_level[128]);
void ntt_asm_256(int16_t r[256], int16_t zetas_level[128]);
void ntt_asm_512(int16_t r[256], int16_t zetas_level[128]);
void ntt_asm_1024(int16_t r[256], int16_t zetas_level[128]);
void ntt_asm_2048(int16_t r[256], int16_t zetas_level[128]);

#endif
