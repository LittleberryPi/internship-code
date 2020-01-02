/**
* ITER macros
**/
// Unzipping macros
#define UZP1(dest, src1, src2)\
		" uzp1 "dest".h, "src1".h, "src2".h \n\ "
#define UZP2(dest, src1, src2)\
		" uzp2 "dest".h, "src1".h, "src2".h \n\ "
#define UZP12(dest1, dest2, src1, src2)\
		" uzp1 "dest1".h, "src1".h, "src2".h \n\
          uzp2 "dest2".h, "src1".h, "src2".h \n\ "

  // VL=128-bit > Level 6: gather the zetas needed for level 6 for ZIP_ZETAS option invntt
  #define INVGATHER128_ZETAS_LVL6\
    " inch x13 																	\n\
  	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z28zetas_inv      \n\
  	inch x13 																	\n\
    ld1h	{ z25.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z25=zetas_inv     \n\
  	inch x13 																	\n\
    ld1h	{ z26.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z26=zetas_inv     \n\ "

// VL=256-bit > Level 6: gather the zetas needed for level 6 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL6\
  " inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z8=zetas_inv      \n\
	inch x13 																	\n\
  	ld1h	{ z25.h }, p1/z, [%[zetas_inv], x13, lsl #1]  	//z25=zetas_inv     \n\
	inch x13 																	\n\
  	ld1h	{ z26.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z26=zetas_inv     \n\ "

// VL=512-bit > Level 6: gather the zetas needed for level 6 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL6\
    " inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv     \n\ "

// VL=2048-bit > Level 6: gather the zetas needed for level 6 for ZIP_ZETAS option invntt
#define INVGATHER2048_ZETAS_LVL6\
    " movprfx z8, z27 															\n\
	ext z8.b, z8.b, z27.b, #128									  				\n\
	ext z8.b, z8.b, z27.b, #128									  				\n\ "

// VL=128-bit > Level 5: gather the zetas needed for level 5 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL5\
	" inch x13 																	\n\
	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z27=zetas_inv     \n\
	inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z8=zetas_inv      \n\
	inch x13 																	\n\
	ld1h	{ z25.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z25=zetas_inv     \n\
	inch x13 																	\n\
	ld1h	{ z26.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z26=zetas_inv     \n\ "

// VL=256-bit > Level 5: gather the zetas needed for level 5 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL5\
    " inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv     \n\
	inch x13 																	\n\
	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv \n\ "

// VL=512-bit > Level 5: gather the zetas needed for level 5 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL5\
    " inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv     \n\ "

// VL=1024-bit > Level 5: gather the zetas needed for level 5 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL5\
    " inch x13 																	\n\
	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv \n\
	movprfx z8, z27 															\n\
	ext z8.b, z8.b, z27.b, #64									  				\n\
	ext z8.b, z8.b, z27.b, #64													\n\ "

// VL=128-bit > Level 4: gather the zetas needed for level 4 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL4\
	" inch x13 																	\n\
	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]	//z27=zetas_inv     \n\
	inch x13 																	\n\
	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z8=zetas_inv      \n\ "

// VL=256-bit > Level 4: gather the zetas needed for level 4 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL4\
  " inch x13 																	\n\
  	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv     \n\ "

// VL=512-bit > Level 4: gather the zetas needed for level 4 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL4\
    " inch x13 																		  \n\
	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv       \n\
	dup z8.q, z27.q[0]                                                                \n\
	dup z12.q, z27.q[1]                                                               \n\
	zip1 z8.d, z8.d, z12.d                                                            \n\
	dup z12.q, z8.q[0]                                                                \n\
	dup z8.q, z8.q[1]                                                                 \n\
	zip1 z8.d, z12.d, z8.d                                                            \n\ "

// VL=1024-bit > Level 4: gather the zetas needed for level 4 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL4\
    " movprfx z12, z27                        	  \n\
      ext z12.b, z12.b, z27.b, #64                \n\
	  movprfx z8, z27                         	  \n\
      ext z8.b, z8.b, z27.b, #96                  \n\
      ext z8.b, z8.b, z12.b, #32                  \n\
      ext z8.b, z8.b, z12.b, #32                  \n\
      ext z8.b, z8.b, z12.b, #32                  \n\ "

// VL=128-bit > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL3\
  " inch x13 																	      \n\
  	ld1h	{ z8.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z8=zetas_inv            \n\ "

// VL=256-bit > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL3\
  " inch x13 																		  \n\
  	ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv       \n\
  	dup z8.q, z27.q[0]                                                                \n\ "

// VL=512-bit > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL3\
  " dup z8.q, z27.q[2]                                                                \n\ "

// VL=1024-bit > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL3\
    " movprfx z8, z27													 			  \n\
	  ext z8.b, z8.b, z27.b, #96                                                      \n\
	  dup z8.q, z8.q[0] 															  \n\ "

// VL=128-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL2\
	"  inch x13 																	  \n\
       ld1h	{ z27.h }, p1/z, [%[zetas_inv], x13, lsl #1]		//z29=zetas_inv       \n\
       dup z8.d, z27.d[0]                                                             \n\ "

// VL=256-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL2\
  " dup z8.d, z27.d[2]                                                                \n\ "

// VL=512-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL2\
    " dup z8.d, z27.d[6]                                                              \n\ "

// VL=1024-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL2\
    " movprfx z8, z27													 			  \n\
	  ext z8.b, z8.b, z27.b, #112                                                     \n\
	  dup z8.d, z8.d[0] 															  \n\ "

// VL=128-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL1\
  " dup z8.s, z27.s[2]                                                                \n\ "

// VL=256-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL1\
  " dup z8.s, z27.s[6]                                                                \n\ "

// VL=512-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL1\
    " dup z8.s, z27.s[14]                                                             \n\ "

// VL=1024-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL1\
    " movprfx z8, z27													 			  \n\
	  ext z8.b, z8.b, z27.b, #120                                                     \n\
	  dup z8.s, z8.s[0] 															  \n\ "

// VL=128-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option invntt
#define INVGATHER128_ZETAS_LVL0\
  " dup z8.h, z27.h[6]                                                                \n\ "

// VL=256-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option invntt
#define INVGATHER256_ZETAS_LVL0\
  " dup z8.h, z27.h[14]                                                               \n\ "

// VL=512-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option invntt
#define INVGATHER_ZETAS_LVL0\
    " dup z8.h, z27.h[30]                                                             \n\ "

// VL=1024-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option invntt
#define INVGATHER1024_ZETAS_LVL0\
    " movprfx z8, z27													 			  \n\
	  ext z8.b, z8.b, z27.b, #124                                                     \n\
	  dup z8.h, z8.h[0] 															  \n\ "

// VL=256-bit > Initialize the zetas and then do one level of the NTT
// init_zetas: the macro that initializes the zetas in the registers
// zeta1: the first zeta src register
// zeta2: the second zeta src register
#define INVITER256(init_zetas, zeta1, zeta2)\
	  " "init_zetas"														          \n\
		"INVORDER256"                                                                 \n\
		"INVITER("Z1", "Z0", "Z17", "Z18", "Z0", zeta1)"                              \n\
        "UZP2("Z21", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z3", "Z2", "Z17", "Z18", "Z1", zeta2)"                         	  \n\
        "UZP2("Z22", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z5", "Z4", "Z17", "Z18", "Z2", zeta1)"                        	  \n\
        "UZP2("Z23", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z7", "Z6", "Z17", "Z18", "Z3", zeta2)"                        	  \n\
        "UZP2("Z24", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z10", "Z9", "Z17", "Z18", "Z4", zeta1)"                       	  \n\
        "UZP2("Z9", "Z17", "Z18")"                                              	  \n\
        "INVITER("Z12", "Z11", "Z17", "Z18", "Z5", zeta2)"                       	  \n\
        "UZP2("Z10", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z14", "Z13", "Z17", "Z18", "Z6", zeta1)"                      	  \n\
        "UZP2("Z11", "Z17", "Z18")"                                             	  \n\
        "INVITER("Z16", "Z15", "Z17", "Z18", "Z7", zeta2)"                      	  \n\
        "UZP2("Z16", "Z17", "Z18")" 											      \n\	"

// VL=512-bit > Initialize the zetas and then do one level of the NTT
// init_zetas: the macro that initializes the zetas in the registers
#define INVITER512(init_zetas)\
	  " "init_zetas"																  \n\
		"INVORDER512"                                                                 \n\
		"INVITER("Z1", "Z0", "Z21", "Z22", "Z0", "Z8")"                               \n\
		"INVITER("Z3", "Z2", "Z23", "Z24", "Z1", "Z8")"                               \n\
		"INVITER("Z5", "Z4", "Z25", "Z26", "Z2", "Z8")"                               \n\
		"UZP2("Z4", "Z21", "Z22")"                                                    \n\
		"UZP2("Z5", "Z23", "Z24")"                                                    \n\
		"INVITER("Z7", "Z6", "Z21", "Z22", "Z3", "Z8")"                               \n\
		"UZP2("Z6", "Z25", "Z26")"                                                    \n\
		"UZP2("Z7", "Z21", "Z22")"                                                    \n\ "

// VL=1024-bit > Initialize the zetas and then do one level of the NTT
// init_zetas: the macro that initializes the zetas in the registers
#define INVITER1024(init_zetas)\
	  " "init_zetas"																  \n\
		"INVORDER1024"                                                                \n\
		"INVITER("Z1", "Z0", "Z21", "Z22", "Z0", "Z8")"                               \n\
		"INVITER("Z3", "Z2", "Z23", "Z24", "Z1", "Z8")"                               \n\
		"UZP2("Z2", "Z21", "Z22")"                                                    \n\
		"UZP2("Z3", "Z23", "Z24")"                                                    \n\ "

// VL=2048-bit > Initialize the zetas and then do one level of the NTT
#define INVITER2048\
	  " "INVORDER2048"                                                                \n\
		"INVITER_LOAD_ZETAS("Z1", "Z0", "Z21", "Z22", "Z0")"                          \n\
		"UZP2("Z1", "Z21", "Z22")"                                                    \n\ "

// VL=128-bit INVNTT level 6,
// where zetas are loaded from memory and zipped into the right registers. We get r from the registers
// We store half the results in memory and keep half of it in registers
// dest: the destination array to store the results in memory
// zeta1: the first src zeta register
// zeta2: the second src zeta register
// zeta3: the third src zeta register
// zeta4: the fourth src zeta register
#define INVITER128_FROM_REG_LVL6(dest, zeta1, zeta2, zeta3, zeta4)\
	" mov x14, #128                                                                \n\
	"INVITER("Z1", "Z0", "Z17", "Z18", "Z0", zeta1)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z3", "Z2", "Z17", "Z18", "Z1", zeta2)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z5", "Z4", "Z17", "Z18", "Z2", zeta3)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z7", "Z6", "Z17", "Z18", "Z3", zeta4)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	mov x14, #192                                                                  \n\
	"INVITER("Z10", "Z9", "Z17", "Z18", "Z4", zeta1)"                              \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z12", "Z11", "Z17", "Z18", "Z5", zeta2)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z14", "Z13", "Z17", "Z18", "Z6", zeta3)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z16", "Z15", "Z17", "Z18", "Z7", zeta4)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\ "

// VL=128-bit > INVNTT level 6,
// where zetas are loaded from memory and zipped into the right registers. We get r from the memory
// We store half the results in memory and keep half of it in registers
// src: the source array to load r from
// dest: the destination array to store the results in memory
// zeta1...zeta8: src registers for zeta
#define INVITER128_FROM_MEM_LVL6(src, dest, zeta1, zeta2, zeta3, zeta4, zeta5, zeta6, zeta7, zeta8)\
	" mov x11, #128 															   \n\
	mov x14, #160                                                                  \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"UZP12("Z18", "Z14", "Z12", "Z14")"                                            \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z21", zeta1)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"UZP12("Z18", "Z14", "Z13", "Z15")"                                            \n\
	mov x14, #224                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z9", zeta2)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"UZP12("Z18", "Z14", "Z12", "Z14")"                                            \n\
	mov x14, #168                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z22", zeta3)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"UZP12("Z18", "Z14", "Z13", "Z15")"                                            \n\
	mov x14, #232                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z10", zeta4)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"UZP12("Z18", "Z14", "Z12", "Z14")"                                            \n\
	mov x14, #176                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z23", zeta5)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"UZP12("Z18", "Z14", "Z13", "Z15")"                                            \n\
	mov x14, #240                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z11", zeta6)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"UZP12("Z18", "Z14", "Z12", "Z14")"                                            \n\
	mov x14, #184                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z24", zeta7)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	"UZP12("Z18", "Z14", "Z13", "Z15")"                                            \n\
	mov x14, #248                                                                  \n\
	"INVITER("Z14", "Z18", "Z17", "Z18", "Z16", zeta8)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\
	mov x8, xzr                                                                    \n\
	"INVSTORE("Z21")" 															   \n\ "

// VL=128-bit > Do one even level of the INVNTT
#define INVITER128_EVEN_LEVEL(init_zetas, zeta1, zeta2)\
	" "init_zetas"																	\n\
	"INVORDER128"																	\n\
	"INVITER128_FROM_REG("r2", zeta1, zeta2, zeta1, zeta2)"				 			\n\
	"INVITER128_FROM_MEM("r", "r2", zeta1, zeta2, zeta1, zeta2, zeta1, zeta2, zeta1, zeta2)" \n\ "

// VL=128-bit > Do one odd level of the INVNTT
#define INVITER128_ODD_LEVEL(init_zetas, zeta1, zeta2, zeta3, zeta4)\
	" "init_zetas"																	\n\
	"INVORDER128"																	\n\
	"INVITER128_FROM_REG("r", zeta1, zeta2, zeta3, zeta4)"				 			\n\
	"INVITER128_FROM_MEM("r2", "r", zeta1, zeta2, zeta3, zeta4, zeta1, zeta2, zeta3, zeta4)" \n\ "


// VL=128-bit > One vector register's worth of butterfly operations for INVNTT
// where zetas are loaded from memory and zipped into the right registers. We get r from the registers
// We store half the results in memory and keep half of it in registers
// dest: the destination array to store the results in memory
// zeta1: the first src zeta register
// zeta2: the second src zeta register
// zeta3: the third src zeta register
// zeta4: the fourth src zeta register
#define INVITER128_FROM_REG(dest, zeta1, zeta2, zeta3, zeta4)\
	" mov x14, #128                                                                \n\
	"INVITER("Z1", "Z0", "Z17", "Z18", "Z0", zeta1)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z3", "Z2", "Z17", "Z18", "Z1", zeta2)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z5", "Z4", "Z17", "Z18", "Z2", zeta3)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z7", "Z6", "Z17", "Z18", "Z3", zeta4)"                               \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                              	   \n\
	"INVITER("Z10", "Z9", "Z17", "Z18", "Z21", zeta1)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z12", "Z11", "Z17", "Z18", "Z22", zeta2)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z14", "Z13", "Z17", "Z18", "Z23", zeta3)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z16", "Z15", "Z17", "Z18", "Z24", zeta4)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\ "

// VL=128-bit > One vector register's worth of butterfly operations for INVNTT
// where zetas are loaded from memory and zipped into the right registers. We get r from the memory
// We store half the results in memory and keep half of it in registers
// src: the source array to load r from
// dest: the destination array to store the results in memory
// zeta1...zeta8: src registers for zeta
#define INVITER128_FROM_MEM(src, dest, zeta1, zeta2, zeta3, zeta4, zeta5, zeta6, zeta7, zeta8)\
	" mov x11, #128 															   \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"INVITER("Z13", "Z12", "Z17", "Z18", "Z4", zeta1)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z15", "Z14", "Z17", "Z18", "Z5", zeta2)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"INVITER("Z13", "Z12", "Z17", "Z18", "Z6", zeta3)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z15", "Z14", "Z17", "Z18", "Z7", zeta4)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"INVITER("Z13", "Z12", "Z17", "Z18", "Z9", zeta5)"                             \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z15", "Z14", "Z17", "Z18", "Z10", zeta6)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"PREPARE_ITER(src, "Z13", "Z12", "Z15", "Z14")"                                \n\
	"INVITER("Z13", "Z12", "Z17", "Z18", "Z11", zeta7)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP("Z17", dest)"                                                   \n\
	"INVITER("Z15", "Z14", "Z17", "Z18", "Z16", zeta8)"                            \n\
	"UZP2("Z17", "Z17", "Z18")"                                                    \n\
	"INVSTORE_TEMP_LAST("Z17", dest)"                                              \n\ "

// VL=256-bit, 512-bit, 1024-bit, 2048-bit > One vector register's worth of butterfly operations for a NTT level,
// where zetas are loaded from memory and zipped into the right registers
// r_up: src register of r_up
// r_low: src register of r_low
// r2_up1: dest register of r_up(lower half), later to be uzp2'ed with r2_up2
// r2_up2: dest register of r_up(upper half), later to be uzp2'ed with r2_up1
// r2_low: dest register or r_low
// zeta: zeta register
#define INVITER(r_up, r_low, r2_up1, r2_up2, r2_low, zeta)\
        "add	z17.h, "r_low".h, "r_up".h                            //a = r_low + r_up                    \n\
        sub	z18.h, "r_low".h, "r_up".h                                //sub=r_low-r_up                      \n\
        sunpkhi	z19.s, z17.h                                          //a_up                                \n\
        sunpklo	"r2_low".s, z17.h                                     //a_low                               \n\
        mul	"r2_low".s, p0/m, "r2_low".s, z28.s                       //tmp_reduce_low=v*a                  \n\
        mul	z19.s, p0/m, z19.s, z28.s                                 //tmp_reduce_up=v*a_up                \n\
        asr	"r2_low".s, "r2_low".s, #26                               //tmp_reduce_low >> 26                \n\
        asr	z19.s, z19.s, #26                                         //tmp_reduce_up >> 26                 \n\
        uzp1	"r2_low".h, "r2_low".h, z19.h                         //tmp_reduce                          \n\
        mad	"r2_low".h, p1/m, z29.h, z17.h                            //a+tmp_reduce*kyber_q                \n\
        sunpklo	z17.s, z18.h                                          //sub_low                             \n\
        sunpkhi	z18.s, z18.h                                          //sub_up                              \n\
        sunpkhi	z20.s, "zeta".h                                       //zeta_up                             \n\
        mul	z20.s, p0/m, z20.s, z18.s                                 //multiplied_up=sub_up*zeta_up        \n\
        sunpklo	z19.s, "zeta".h                                       //zeta_low                            \n\
        mul	z19.s, p0/m, z19.s, z17.s                                 //multiplied_low=sub_low*zeta_low     \n\
        movprfx z17, z19                                                                                    \n\
        mul	z17.s, p0/m, z17.s, z30.s                                 //a_low=multiplied_low*qinv           \n\
        movprfx z18, z20                                                                                    \n\
        mul	z18.s, p0/m, z18.s, z30.s                                 //a_up=multiplied_up*qinv             \n\
        asr	z18.s, z18.s, #16                                         //a_up >> 16                          \n\
        asr	z17.s, z17.s, #16                                         //a_low >> 16                         \n\
        mul	z17.s, p0/m, z17.s, z31.s                                 //tmp_reduce_low=a_low*kyber_q        \n\
        mul	z18.s, p0/m, z18.s, z31.s                                 //tmp_reduce_up=a_up*kyber_q          \n\
        add "r2_up1".s, z17.s, z19.s                                                                        \n\
        add "r2_up2".s, z18.s, z20.s                                                                        \n\ "

// VL=2048-bit > One vector register's worth of butterfly operations for a NTT level,
// where zetas are loaded from memory
// r_up: src register of r_up
// r_low: src register of r_low
// r2_up1: dest register of r_up(lower half), later to be uzp2'ed with r2_up2
// r2_up2: dest register of r_up(upper half), later to be uzp2'ed with r2_up1
// r2_low: dest register or r_low
#define INVITER_LOAD_ZETAS(r_up, r_low, r2_up1, r2_up2, r2_low)\
        "ld1h	{ z8.h }, p1/z, [%[zetas_inv_levels], x13, lsl #1]    //load in zetas                       \n\
        add	z17.h, "r_low".h, "r_up".h                                //a = r_low + r_up                    \n\
        sub	z18.h, "r_low".h, "r_up".h                                //sub=r_low-r_up                      \n\
        sunpkhi	z19.s, z17.h                                          //a_up                                \n\
        sunpklo	"r2_low".s, z17.h                                     //a_low                               \n\
        mul	"r2_low".s, p0/m, "r2_low".s, z28.s                       //tmp_reduce_low=v*a                  \n\
        mul	z19.s, p0/m, z19.s, z28.s                                 //tmp_reduce_up=v*a_up                \n\
        asr	"r2_low".s, "r2_low".s, #26                               //tmp_reduce_low >> 26                \n\
        asr	z19.s, z19.s, #26                                         //tmp_reduce_up >> 26                 \n\
        uzp1	"r2_low".h, "r2_low".h, z19.h                         //tmp_reduce                          \n\
        mad	"r2_low".h, p1/m, z29.h, z17.h                            //a+tmp_reduce*kyber_q                \n\
        sunpklo	z17.s, z18.h                                          //sub_low                             \n\
        sunpkhi	z18.s, z18.h                                          //sub_up                              \n\
        sunpkhi	z20.s, z8.h                                           //zeta_up                             \n\
        mul	z20.s, p0/m, z20.s, z18.s                                 //multiplied_up=sub_up*zeta_up        \n\
        sunpklo	z19.s, z8.h                                           //zeta_low                            \n\
        mul	z19.s, p0/m, z19.s, z17.s                                 //multiplied_low=sub_low*zeta_low     \n\
        movprfx z17, z19                                                                                    \n\
        mul	z17.s, p0/m, z17.s, z30.s                                 //a_low=multiplied_low*qinv           \n\
        movprfx z18, z20                                                                                    \n\
        mul	z18.s, p0/m, z18.s, z30.s                                 //a_up=multiplied_up*qinv             \n\
        asr	z18.s, z18.s, #16                                         //a_up >> 16                          \n\
        asr	z17.s, z17.s, #16                                         //a_low >> 16                         \n\
        mul	z17.s, p0/m, z17.s, z31.s                                 //tmp_reduce_low=a_low*kyber_q        \n\
        mul	z18.s, p0/m, z18.s, z31.s                                 //tmp_reduce_up=a_up*kyber_q          \n\
        add "r2_up1".s, z17.s, z19.s                                                                        \n\
        add "r2_up2".s, z18.s, z20.s                                                                        \n\
        inch	x13                                                                                         \n\ "

// Prepare the register for the iteration by loading in the even elements of r into r_low and r1_low
// and the odd elements into r_up and r1_up
#define PREPARE_ITER(src, r_up, r_low, r1_up, r1_low)\
		"ld2h	{ "r_low".h, "r_up".h }, p1/z, [%["src"], x11, lsl #1]                                      \n\
		inch x11                                                                                            \n\
		inch x11                                                                                            \n\
		ld2h	{ "r1_low".h, "r1_up".h }, p1/z, [%["src"], x11, lsl #1]                                    \n\
		inch x11                                                                                            \n\
		inch x11                                                                                            \n\ "
