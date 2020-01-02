/**
* INIT/LOAD macros
**/
// Contiguous load parts of r_low and r_up from memory to dest registers, we do this to get r into
// the right order in the registers
#define INVLOAD_RLOW(dest, dest2)\
    " ld2h	{ "dest".h, "dest2".h }, p1/z, [%[r], x8, lsl #1]		                      \n\
      inch x8																			  \n\
	  inch x8																			  \n\ "
// Contiguous load parts of r_low and r_up from memory into to dest registers without inch at the end
// we do this to get r into the right order in the registers
#define INVLOAD_RLOW_LAST(dest, dest2)\
    " ld2h	{ "dest".h, "dest2".h }, p1/z, [%[r], x8, lsl #1]		                      \n\ "
// Load r_up from memory to dest
#define INVLOAD_RUP(dest)\
    " ld1h	{ "dest".h }, p1/z, [x10, x8, lsl #1]		                                  \n\
      inch x8																		      \n\ "
// Load r_up from memory to dest register without inch at the end
#define INVLOAD_RUP_LAST(dest)\
    " ld1h	{ "dest".h }, p1/z, [x10, x8, lsl #1]                                         \n\ "

// VL=128-bit > load in array r for INVNTT
#define INVLOAD_R_128\
    " "INVLOAD_RLOW("Z0", "Z1")"                                                          \n\
      "INVLOAD_RLOW("Z2", "Z3")"                                                          \n\
      "INVLOAD_RLOW("Z4", "Z5")"                                                          \n\
      "INVLOAD_RLOW("Z6", "Z7")"                                                          \n\
      "INVLOAD_RLOW("Z9", "Z10")"                                                         \n\
      "INVLOAD_RLOW("Z11", "Z12")"                                                        \n\
      "INVLOAD_RLOW("Z13", "Z14")"                                                        \n\
      "INVLOAD_RLOW_LAST("Z15", "Z16")"                                                   \n\ "

// VL=128-bit > load elements (r2_up) from temp array r2 to registers
#define INVLOAD_R2_128\
    " "INVLOAD_RUP("Z0")"                                                                 \n\
    "INVLOAD_RUP("Z1")"                                                                   \n\
    "INVLOAD_RUP("Z2")"                                                                   \n\
    "INVLOAD_RUP("Z3")"                                                                   \n\
    "INVLOAD_RUP("Z4")"                                                                   \n\
    "INVLOAD_RUP("Z5")"                                                                   \n\
    "INVLOAD_RUP("Z6")"                                                                   \n\
    "INVLOAD_RUP("Z7")"                                                                   \n\
    "INVLOAD_RUP("Z21")"                                                                  \n\
    "INVLOAD_RUP("Z22")"                                                                  \n\
    "INVLOAD_RUP("Z23")"                                                                  \n\
    "INVLOAD_RUP("Z24")"                                                                  \n\
    "INVLOAD_RUP("Z9")"                                                                   \n\
    "INVLOAD_RUP("Z10")"                                                                  \n\
    "INVLOAD_RUP("Z11")"                                                                  \n\
    "INVLOAD_RUP_LAST("Z16")"                                                             \n\ "

// VL=256-bit > load in array r for INVNTT
#define INVLOAD_R_256\
    " "INVLOAD_RLOW("Z0", "Z1")"                                                          \n\
      "INVLOAD_RLOW("Z2", "Z3")"                                                          \n\
      "INVLOAD_RLOW("Z4", "Z5")"                                                          \n\
      "INVLOAD_RLOW("Z6", "Z7")"                                                          \n\
      "INVLOAD_RLOW("Z9", "Z10")"                                                         \n\
      "INVLOAD_RLOW("Z11", "Z12")"                                                        \n\
      "INVLOAD_RLOW("Z13", "Z14")"                                                        \n\
      "INVLOAD_RLOW_LAST("Z15", "Z16")"                                                   \n\ "

// VL=512-bit > load in array r for INVNTT
#define INVLOAD_R_512\
    " "INVLOAD_RLOW("Z8", "Z9")"                                                          \n\
      "INVLOAD_RLOW("Z10", "Z11")"                                                        \n\
      "INVLOAD_RLOW("Z12", "Z13")"                                                        \n\
      "INVLOAD_RLOW_LAST("Z14", "Z15")"                                                   \n\ "

// VL=1024-bit > load in array r for INVNTT
#define INVLOAD_R_1024\
  " "INVLOAD_RLOW("Z8", "Z9")"                                                            \n\
    "INVLOAD_RLOW_LAST("Z10", "Z11")"                                                     \n\ "

// VL=2048-bit > load in array r for INVNTT
#define INVLOAD_R_2048\
  " "INVLOAD_RLOW_LAST("Z8", "Z9")"                                                       \n\ "

// VL=128-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
// TODO REMOVE MOVS
#define INVORDER128_LEVEL6\
  " "UZP12("Z17", "Z18", "Z0", "Z2")"         \n\
    mov z0.d, z17.d                           \n\
    "UZP12("Z17", "Z19", "Z1", "Z3")"         \n\
    mov z1.d, z18.d                           \n\
    "UZP12("Z18", "Z20", "Z9", "Z11")"        \n\
    mov z9.d, z17.d                           \n\
    "UZP12("Z17", "Z3", "Z4", "Z6")"          \n\
    mov z4.d, z18.d                           \n\
    mov z2.d, z17.d                           \n\
    "UZP12("Z6", "Z17", "Z13", "Z15")"        \n\
    "UZP12("Z13", "Z18", "Z10", "Z12")"       \n\
    mov z10.d, z19.d                          \n\
    "UZP12("Z15", "Z16", "Z14", "Z16")"       \n\
    mov z14.d, z18.d                          \n\
    "UZP12("Z11", "Z12", "Z5", "Z7")"         \n\
    mov z5.d, z20.d                           \n\
    mov z7.d, z17.d                           \n\ "

// VL=256-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
// TODO REMOVE MOVS
#define INVORDER256_LEVEL6\
 " "UZP12("Z17", "Z18", "Z0", "Z2")"          \n\
    mov z0.d, z17.d                           \n\
    "UZP12("Z17", "Z19", "Z1", "Z3")"         \n\
    mov z1.d, z18.d                           \n\
    "UZP12("Z18", "Z20", "Z9", "Z11")"        \n\
    mov z9.d, z17.d                           \n\
	"UZP12("Z17", "Z3", "Z4", "Z6")"          \n\
    mov z4.d, z18.d                           \n\
    mov z2.d, z17.d                           \n\
    "UZP12("Z6", "Z17", "Z13", "Z15")"        \n\
    "UZP12("Z13", "Z18", "Z10", "Z12")"       \n\
    mov z10.d, z19.d                          \n\
    "UZP12("Z15", "Z16", "Z14", "Z16")"       \n\
    mov z14.d, z18.d                          \n\
    "UZP12("Z11", "Z12", "Z5", "Z7")"         \n\
    mov z5.d, z20.d                           \n\
    mov z7.d, z17.d                           \n\ "

// VL=512-bit > get the registers ready for level 6 by putting the r_low elements in the same
// register and the r_up elements in the same register
#define INVORDER512_LEVEL6\
  " "UZP12("Z0", "Z1", "Z8", "Z10")"          \n\
	"UZP12("Z2", "Z3", "Z12", "Z14")"         \n\
	"UZP12("Z4", "Z5", "Z9", "Z11")"          \n\
	"UZP12("Z6", "Z7", "Z13", "Z15")"         \n\ "

// VL=1024-bit > get the registers ready for level 6 by putting the r_low elements in the same
// register and the r_up elements in the same register
#define INVORDER1024_LEVEL6\
  " "UZP12("Z0", "Z1", "Z8", "Z10")"          \n\
	"UZP12("Z2", "Z3", "Z9", "Z11")"          \n\ "

// VL=2048-bit > get the registers ready for level 6 by putting the r_low elements in the same
// register and the r_up elements in the same register
#define INVORDER2048_LEVEL6\
  " "UZP12("Z0", "Z1", "Z8", "Z9")"           \n\ "

// VL=128-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
#define INVORDER128\
 " "UZP12("Z17", "Z1", "Z0", "Z1")"           \n\
    mov z0.d, z17.d                           \n\
    "UZP12("Z17", "Z3", "Z2", "Z3")"          \n\
    mov z2.d, z17.d                           \n\
    "UZP12("Z13", "Z14", "Z9", "Z10")"        \n\
    "UZP12("Z9", "Z10", "Z4", "Z5")"          \n\
    "UZP12("Z4", "Z5", "Z21", "Z22")"         \n\
    "UZP12("Z15", "Z16", "Z11", "Z16")"       \n\
    "UZP12("Z11", "Z12", "Z6", "Z7")"         \n\
    "UZP12("Z6", "Z7", "Z23", "Z24")"         \n\ "

// VL=256-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
#define INVORDER256\
  " "UZP12("Z17", "Z1", "Z0", "Z1")"          \n\
    mov z0.d, z17.d                           \n\
	"UZP12("Z17", "Z3", "Z2", "Z3")"          \n\
    mov z2.d, z17.d                           \n\
	"UZP12("Z17", "Z5", "Z4", "Z5")"          \n\
    mov z4.d, z17.d                           \n\
	"UZP12("Z17", "Z7", "Z6", "Z7")"          \n\
    mov z6.d, z17.d                           \n\
  	"UZP12("Z13", "Z14", "Z9", "Z10")"        \n\
    "UZP12("Z9", "Z10", "Z21", "Z22")"        \n\
  	"UZP12("Z15", "Z16", "Z11", "Z16")"       \n\
    "UZP12("Z11", "Z12", "Z23", "Z24")"       \n\ "

// VL=512-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
#define INVORDER512\
 " "UZP12("Z12", "Z1", "Z0", "Z1")"           \n\
    mov z0.d, z12.d                           \n\
	"UZP12("Z12", "Z3", "Z2", "Z3")"          \n\
    mov z2.d, z12.d                           \n\
	"UZP12("Z12", "Z5", "Z4", "Z5")"          \n\
    mov z4.d, z12.d                           \n\
	"UZP12("Z12", "Z7", "Z6", "Z7")"          \n\
    mov z6.d, z12.d                           \n\ "

// VL=1024-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
#define INVORDER1024\
  " "UZP12("Z12", "Z1", "Z0", "Z1")"          \n\
    mov z0.d, z12.d                           \n\
	"UZP12("Z12", "Z3", "Z2", "Z3")"          \n\
    mov z2.d, z12.d                           \n\ "

// VL=2048-bit > get the registers for level 5,4,3,2,1 or 0 ready by putting the r_low elements in
// the same register and the r_up elements in the same register
#define INVORDER2048\
  " "UZP12("Z12", "Z1", "Z0", "Z1")"          \n\
    mov z0.d, z12.d                           \n\ "


// Initialize all the variables needed by putting them in registers and load first part of zetas
// for ZIP_ZETAS version
// load: the macro for loading the elements of r
#define INVINIT_R_AND_ZETAS(load, order)\
	  "	mov	x8, xzr							//x8=0                              \n\
      mov	x13, xzr						//x13=0                             \n\
      ptrue	p0.s                                                                \n\
      ptrue	p1.h                                                                \n\
      ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv \n\
      mov	z28.s, %w[v]                                                        \n\
      mov	z29.h, %w[kyber_q]                                                  \n\
      mov	z30.s, %w[qinv]                                                     \n\
      mov	z31.s, %w[kyber_q]                                                  \n\
	  "load"                                                                    \n\
	  "order"                                                                   \n\
      mov x8, xzr                                                               \n\ "

// Initialize all the variables needed by putting them in registers and load first part of zetas
// for LOAD_ZETAS version
// load: the macro for loading the elements of r
#define INVINIT_R(load, order)\
	  "	mov	x8, xzr							//x8=0                              \n\
    mov	x13, xzr							//x13=0                             \n\
    ptrue	p0.s                                                                \n\
    ptrue	p1.h                                                                \n\
    mov	z28.s, %w[v]                                                            \n\
    mov	z29.h, %w[kyber_q]                                                      \n\
    mov	z30.s, %w[qinv]                                                         \n\
    mov	z31.s, %w[kyber_q]                                                      \n\
	  "load"                                                                    \n\
	  "order"                                                                   \n\
    mov x8, xzr                                                                 \n\ "
