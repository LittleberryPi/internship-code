/**
* INIT/LOAD macros
**/
// Load r_low from memory to dest register
#define LOAD_RLOW(dest)\
    " ld1h	{ "dest".h }, p0/z, [%[r], x8, lsl #1]		                                  \n\
      inch x8																			  \n\ "
// Load r_low from memory into to dest register without inch at the end
#define LOAD_RLOW_LAST(dest)\
    " ld1h	{ "dest".h }, p0/z, [%[r], x8, lsl #1]		                                  \n\ "
// Load r_up from memory to dest
#define LOAD_RUP(dest)\
    " ld1h	{ "dest".h }, p0/z, [x10, x8, lsl #1]		                                  \n\
      inch x8																		      \n\ "
// Load r_up from memory to dest register without inch at the end
#define LOAD_RUP_LAST(dest)\
    " ld1h	{ "dest".h }, p0/z, [x10, x8, lsl #1]                                         \n\ "

// Load r2_low from memory to dest register
#define LOAD_R2LOW(dest)\
    " ld1h	{ "dest".h }, p0/z, [%[r2], x8, lsl #1]		                                  \n\
      inch x8																			  \n\ "
// Load r2_low from memory to dest register without inch at the end
#define LOAD_R2LOW_LAST(dest)\
    " ld1h	{ "dest".h }, p0/z, [%[r2], x8, lsl #1]		                                  \n\ "

// VL=128-bit > src/dest registers for NTT to hold throughout the levels
#define LOAD_R_128\
    " "LOAD_RUP("Z9")"                                                                    \n\
      "LOAD_RUP("Z10")"                                                                   \n\
      "LOAD_RUP("Z11")"                                                                   \n\
      "LOAD_RUP("Z16")"                                                                   \n\
      "LOAD_RUP("Z17")"                                                                   \n\
      "LOAD_RUP("Z18")"                                                                   \n\
      "LOAD_RUP("Z19")"                                                                   \n\
      "LOAD_RUP_LAST("Z20")"                                                              \n\
      mov x8, xzr																		  \n\
      "LOAD_RLOW("Z0")"                                                                   \n\
      "LOAD_RLOW("Z1")"                                                                   \n\
      "LOAD_RLOW("Z2")"                                                                   \n\
      "LOAD_RLOW("Z3")"                                                                   \n\
      "LOAD_RLOW("Z4")"                                                                   \n\
      "LOAD_RLOW("Z5")"                                                                   \n\
      "LOAD_RLOW("Z6")"                                                                   \n\
      "LOAD_RLOW_LAST("Z7")"                                                              \n\ "

// VL=128-bit > load elements from temp array r2 to registers
#define LOAD_R2_128\
    " "LOAD_RUP("Z9")"                                                                    \n\
      "LOAD_RUP("Z10")"                                                                   \n\
      "LOAD_RUP("Z11")"                                                                   \n\
      "LOAD_RUP("Z16")"                                                                   \n\
      "LOAD_RUP("Z17")"                                                                   \n\
      "LOAD_RUP("Z18")"                                                                   \n\
      "LOAD_RUP("Z19")"                                                                   \n\
      "LOAD_RUP_LAST("Z20")"                                                              \n\
      mov x8, #64																		  \n\
      "LOAD_R2LOW("Z0")"                                                                  \n\
      "LOAD_R2LOW("Z1")"                                                                  \n\
      "LOAD_R2LOW("Z2")"                                                                  \n\
      "LOAD_R2LOW("Z3")"                                                                  \n\
      "LOAD_R2LOW("Z4")"                                                                  \n\
      "LOAD_R2LOW("Z5")"                                                                  \n\
      "LOAD_R2LOW("Z6")"                                                                  \n\
      "LOAD_R2LOW_LAST("Z7")"                                                             \n\ "

// VL=256-bit > src/dest registers for NTT to hold throughout the levels
#define LOAD_R_256\
    " "LOAD_RUP("Z9")"                                                                    \n\
      "LOAD_RUP("Z10")"                                                                   \n\
      "LOAD_RUP("Z11")"                                                                   \n\
      "LOAD_RUP("Z16")"                                                                   \n\
      "LOAD_RUP("Z17")"                                                                   \n\
      "LOAD_RUP("Z18")"                                                                   \n\
      "LOAD_RUP("Z19")"                                                                   \n\
      "LOAD_RUP_LAST("Z20")"                                                              \n\
      mov x8, xzr																		  \n\
      "LOAD_RLOW("Z0")"                                                                   \n\
      "LOAD_RLOW("Z1")"                                                                   \n\
      "LOAD_RLOW("Z2")"                                                                   \n\
      "LOAD_RLOW("Z3")"                                                                   \n\
      "LOAD_RLOW("Z4")"                                                                   \n\
      "LOAD_RLOW("Z5")"                                                                   \n\
      "LOAD_RLOW("Z6")"                                                                   \n\
      "LOAD_RLOW_LAST("Z7")"                                                              \n\ "

// VL=512-bit > src/dest registers for NTT to hold throughout the levels
#define LOAD_R_512\
    " "LOAD_RUP("Z4")"                                                                    \n\
      "LOAD_RUP("Z5")"                                                                    \n\
      "LOAD_RUP("Z6")"                                                                    \n\
      "LOAD_RUP_LAST("Z7")"                                                               \n\
      mov x8, xzr																		  \n\
      "LOAD_RLOW("Z0")"                                                                   \n\
      "LOAD_RLOW("Z1")"                                                                   \n\
      "LOAD_RLOW("Z2")"                                                                   \n\
      "LOAD_RLOW_LAST("Z3")"                                                              \n\ "

// VL=1024-bit > src/dest registers for NTT to hold throughout the levels
#define LOAD_R_1024\
    " "LOAD_RUP("Z2")"                                                                    \n\
      "LOAD_RUP_LAST("Z3")"                                                               \n\
      mov x8, xzr																		  \n\
      "LOAD_RLOW("Z0")"                                                                   \n\
      "LOAD_RLOW_LAST("Z1")"                                                              \n\ "

// VL=248-bit > src/dest registers for NTT to hold throughout the levels
#define LOAD_R_2048\
    " "LOAD_RUP_LAST("Z1")"                                                               \n\
      "LOAD_RLOW_LAST("Z0")"                                                              \n\ "

// Initialize all the variables needed by putting them in registers for ZIP_ZETAS version
// load: the macro for loading the elements of r
#define INIT(load)\
	  "	mov	x8, xzr									//x8=0                                \n\
      mov	x13, xzr								//x13=zetas_level index               \n\
      add	x10, %[r], #256          			    //x10=*r+256                          \n\
      ptrue	p0.h                                                                          \n\
      mov	z30.s, %w[qinv]							//z0=QINV                             \n\
	  mov	z31.s, %w[kyber_q]					    //z1=KYBER_Q                          \n\
	  "load"                                                                              \n\
      mov x8, xzr                                                                         \n\ "

// Initialize all the variables needed by putting them in registers and load first part of zetas
// for ZIP_ZETAS version
// load: the macro for loading the elements of r
#define INIT_R_AND_ZETAS(load)\
	  "	mov	x8, xzr							//x8=0                                        \n\
      mov	x13, xzr						//x13=0                                       \n\
      ptrue	p0.h                                                                          \n\
      ld1h	{ z29.h }, p0/z, [%[zetas], x13, lsl #1]		//z29=zetas                   \n\
      add	x10, %[r], #256          			            //x10=*r+256                  \n\
      mov	z30.s, %w[qinv]									//z0=QINV                     \n\
	  mov	z31.s, %w[kyber_q]								//z1=KYBER_Q                  \n\
	  "load"                                                                              \n\
      mov x8, xzr                                                                         \n\ "
