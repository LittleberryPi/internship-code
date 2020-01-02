/**
* ITER macros
**/
// Register macros for dest registers while computing r_low and r_up
#define PART0_R2_UP "Z9"
#define PART0_R2_LOW "Z0"
#define PART1_R2_UP "Z22"
#define PART1_R2_LOW "Z10"
#define PART2_R2_UP "Z21"
#define PART2_R2_LOW "Z11"
#define PART3_R2_UP "Z24"
#define PART3_R2_LOW "Z23"
#define PART4_R2_UP "Z17"
#define PART4_R2_LOW "Z16"
#define PART5_R2_UP "Z18"
#define PART5_R2_LOW "Z17"
#define PART6_R2_UP "Z19"
#define PART6_R2_LOW "Z18"
#define PART7_R2_UP "Z22"
#define PART7_R2_LOW "Z20"

// VL=256-bit and 512-bit, 1024-bit and 2048-bit > Level 6: gather the zetas needed for level 6 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL6\
    " dup z8.h, z29.h[1]                                                                    \n\ "

// VL=256-bit and 512-bit and 1024-bit and 2048-bit > Level 5: gather the zetas needed for level 5 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL5\
    " dup z8.s, z29.s[1]                                                                    \n\ "

// VL=256-bit and 512-bit and 1024-bit and 2048-bit > Level 4: gather the zetas needed for level 4 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL4\
    " dup z8.d, z29.d[1]                                                                    \n\ "

// VL=128-bit  > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option
#define GATHER128_ZETAS_LVL3\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\ "

// VL=256-bit and 512-bit and 1024-bit and 2048-bit > Level 3: gather the zetas needed for level 3 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL3\
    " dup z8.q, z29.q[1]                                                                    \n\ "

// VL=128-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option
#define GATHER128_ZETAS_LVL2\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\
      inch x13                                                                              \n\
      ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z29=zetas                   \n\ "

// VL=256-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option
#define GATHER256_ZETAS_LVL2\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\ "

// VL=512-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option
#define GATHER512_ZETAS_LVL2\
    " mov z8.d, z29.d                                                                       \n\
      ext z29.b, z29.b, z29.b, #32                                                          \n\
      ext z8.b, z8.b, z29.b, #32                                                            \n\ "

// VL=1024-bit and 2048-bit > Level 2: gather the zetas needed for level 2 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL2\
    " dup z8.q, z29.q[2]                                                                    \n\
      dup z12.q, z29.q[3]                                                                   \n\
      zip1 z8.d, z8.d, z12.d                                                                \n\
      dup z12.q, z8.q[0]                                                                    \n\
      dup z8.q, z8.q[1]                                                                     \n\
      zip1 z8.d, z12.d, z8.d                                                                \n\ "

// VL=128-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option
#define GATHER128_ZETAS_LVL1\
  " inch x13                                                                                \n\
    ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                       \n\
    inch x13                                                                                \n\
    ld1h    { z27.h }, p0/z, [%[zetas], x13, lsl #1]        //z27=zetas                     \n\
    inch x13                                                                                \n\
    ld1h    { z28.h }, p0/z, [%[zetas], x13, lsl #1]        //z28=zetas                     \n\
    inch x13                                                                                \n\
    ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z29=zetas                     \n\ "

// VL=256-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option
#define GATHER256_ZETAS_LVL1\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\
      inch x13                                                                              \n\
      ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z29=zetas                   \n\ "

// VL=512-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL1\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\ "

// VL=1024-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option
#define GATHER1024_ZETAS_LVL1\
    " mov z8.d, z29.d                                                                       \n\
      ext z29.b, z29.b, z29.b, #64                                                          \n\
      ext z8.b, z8.b, z29.b, #64                                                            \n\ "

// VL=2048-bit > Level 1: gather the zetas needed for level 1 for ZIP_ZETAS option
#define GATHER2048_ZETAS_LVL1\
    " mov z12.d, z29.d                                                                      \n\
      mov z8.d, z29.d                                                                       \n\
      ext z12.b, z12.b, z12.b, #64                                                          \n\
      ext z8.b, z8.b, z8.b, #128                                                            \n\
      mov z9.d, z8.d                                                                        \n\
      ext z9.b, z9.b, z12.b, #192                                                           \n\
      ext z8.b, z8.b, z12.b, #64                                                            \n\
      ext z8.b, z8.b, z9.b, #128                                                            \n\ "

// VL=128-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option
#define GATHER128_ZETAS_LVL0\
  " inch x13                                                                                \n\
    ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                       \n\
    inch x13                                                                                \n\
    ld1h    { z27.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                      \n\
    inch x13                                                                                \n\
    ld1h    { z28.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                      \n\
    inch x13                                                                                \n\
    ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                      \n\ "

// VL=256-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option
#define GATHER256_ZETAS_LVL0\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\
      inch x13                                                                              \n\
      ld1h    { z25.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                    \n\
      inch x13                                                                              \n\
      ld1h    { z26.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                    \n\
      inch x13                                                                              \n\
      ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                    \n\ "

// VL=512-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option
#define GATHER_ZETAS_LVL0\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\
      inch x13                                                                              \n\
      ld1h    { z29.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                    \n\ "

// VL=1024-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option
#define GATHER1024_ZETAS_LVL0\
    " inch x13                                                                              \n\
      ld1h    { z8.h }, p0/z, [%[zetas], x13, lsl #1]        //z8=zetas                     \n\ "

// VL=2048-bit > Level 0: gather the zetas needed for level 0 for ZIP_ZETAS option
#define GATHER2048_ZETAS_LVL0\
    " mov z8.d, z29.d                                                                       \n\
      ext z29.b, z29.b, z29.b, #128                                                         \n\
      ext z8.b, z8.b, z29.b, #128                                                           \n\ "

// VL=256-bit > One level of NTT for option LOAD_ZETAS by
// computing r_low and r_up and putting it in registers which are zipped into the right register
#define ITER256_LOAD_ZETAS\
    " "ITER("Z9", "Z0", PART0_R2_UP, PART0_R2_LOW)"                             \n\
    "ITER("Z10", "Z1", PART1_R2_UP, PART1_R2_LOW)"                              \n\
    "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ITER("Z11", "Z2", PART2_R2_UP, PART2_R2_LOW)"                              \n\
    "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                     \n\
    "ITER("Z16", "Z3", PART3_R2_UP, PART3_R2_LOW)"                              \n\
    "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                     \n\
    "ITER("Z17", "Z4", PART4_R2_UP, PART4_R2_LOW)"                              \n\
    "ZIP1("Z4", PART2_R2_LOW, PART2_R2_UP)"                                     \n\
    "ZIP12("Z9", "Z10", PART4_R2_LOW, PART4_R2_UP)"                             \n\
    "ITER("Z18", "Z5", PART5_R2_UP, PART5_R2_LOW)"                              \n\
    "ZIP2("Z5", PART2_R2_LOW, PART2_R2_UP)"                                     \n\
    "ZIP12("Z11", PART4_R2_LOW, PART5_R2_LOW, PART5_R2_UP)"                     \n\
    "ITER("Z19", "Z6", PART6_R2_UP, PART6_R2_LOW)"                              \n\
    "ZIP1("Z6", PART3_R2_LOW, PART3_R2_UP)"                                     \n\
    "ZIP12("Z17", "Z18", PART6_R2_LOW, PART6_R2_UP)"                            \n\
    "ITER("Z20", "Z7", PART7_R2_UP, PART7_R2_LOW)"                              \n\
    "ZIP2("Z7", PART3_R2_LOW, PART3_R2_UP)"                                     \n\
    "ZIP12("Z19", "Z20", PART7_R2_LOW, PART7_R2_UP)"                            \n\ "

// VL=512-bit > One level of NTT for option LOAD_ZETAS by
// computing r_low and r_up and putting it in registers which are zipped into the right register
#define ITER512_LOAD_ZETAS\
    " "ITER("Z4", "Z0", PART0_R2_UP, PART0_R2_LOW)"                             \n\
    "ITER("Z5", "Z1", PART1_R2_UP, PART1_R2_LOW)"                               \n\
    "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ITER("Z6", "Z2", PART2_R2_UP, PART2_R2_LOW)"                               \n\
    "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                     \n\
    "ZIP12("Z4", "Z5", PART2_R2_LOW, PART2_R2_UP)"                              \n\
    "ITER("Z7", "Z3", PART3_R2_UP, PART3_R2_LOW)"                               \n\
    "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                     \n\
    "ZIP12("Z6", "Z7", PART3_R2_LOW, PART3_R2_UP)"                              \n\ "

// VL=1024-bit > One level of NTT for option LOAD_ZETAS by
// computing r_low and r_up and putting it in registers which are zipped into the right register
#define ITER1024_LOAD_ZETAS\
    " "ITER("Z2", "Z0", PART0_R2_UP, PART0_R2_LOW)"                             \n\
    "ITER("Z3", "Z1", PART1_R2_UP, PART1_R2_LOW)"                               \n\
    "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ZIP12("Z2", "Z3", PART1_R2_LOW, PART1_R2_UP)"                              \n\ "

// VL=2048-bit > One level of NTT for option LOAD_ZETAS by
// computing r_low and r_up and putting it in registers which are zipped into the right register
#define ITER2048_LOAD_ZETAS\
    " "ITER("Z1", "Z0", PART0_R2_UP, PART0_R2_LOW)"                             \n\
    "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                     \n\
    "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                     \n\ "

// VL=256-bit > One level of NTT for option ZIP_ZETAS by initializing the zetas for that level and
// computing r_low and r_up and putting it in registers which are zipped into the right register
// This is done by loading zetas all in at once and zipping them into the right registers
// init_zetas: initialize zetas for that level
// zetas: zeta register to use for first half of level
// zetas2: zeta register to use for second half of level
#define ITER256_ZIP_ZETAS(init_zetas, zeta, zeta2, zeta3, zeta4)\
    " "init_zetas"                                                              \n\
      "ITER_ZIP_ZETAS("Z9", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"             \n\
      "ITER_ZIP_ZETAS("Z10", "Z1", PART1_R2_UP, PART1_R2_LOW, zeta2)"           \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ITER_ZIP_ZETAS("Z11", "Z2", PART2_R2_UP, PART2_R2_LOW, zeta3)"           \n\
      "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                   \n\
      "ITER_ZIP_ZETAS("Z16", "Z3", PART3_R2_UP, PART3_R2_LOW, zeta4)"           \n\
      "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                   \n\
      "ITER_ZIP_ZETAS("Z17", "Z4", PART4_R2_UP, PART4_R2_LOW, zeta)"            \n\
      "ZIP1("Z4", PART2_R2_LOW, PART2_R2_UP)"                                   \n\
      "ZIP12("Z9", "Z10", PART4_R2_LOW, PART4_R2_UP)"                           \n\
      "ITER_ZIP_ZETAS("Z18", "Z5", PART5_R2_UP, PART5_R2_LOW, zeta2)"           \n\
      "ZIP2("Z5", PART2_R2_LOW, PART2_R2_UP)"                                   \n\
      "ZIP12("Z11", PART4_R2_LOW, PART5_R2_LOW, PART5_R2_UP)"                   \n\
      "ITER_ZIP_ZETAS("Z19", "Z6", PART6_R2_UP, PART6_R2_LOW, zeta3)"           \n\
      "ZIP1("Z6", PART3_R2_LOW, PART3_R2_UP)"                                   \n\
      "ZIP12("Z17", "Z18", PART6_R2_LOW, PART6_R2_UP)"                          \n\
      "ITER_ZIP_ZETAS("Z20", "Z7", PART7_R2_UP, PART7_R2_LOW, zeta4)"           \n\
      "ZIP2("Z7", PART3_R2_LOW, PART3_R2_UP)"                                   \n\
      "ZIP12("Z19", "Z20", PART7_R2_LOW, PART7_R2_UP)"                          \n\ "

// VL=512-bit > One level of NTT for option ZIP_ZETAS by initializing the zetas for that level and
// computing r_low and r_up and putting it in registers which are zipped into the right register
// This is done by loading zetas all in at once and zipping them into the right registers
// init_zetas: initialize zetas for that level
// zetas: zeta register to use for first half of level
// zetas2: zeta register to use for second half of level
#define ITER512_ZIP_ZETAS(init_zetas, zeta, zeta2)\
    " "init_zetas"                                                              \n\
      "ITER_ZIP_ZETAS("Z4", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"             \n\
      "ITER_ZIP_ZETAS("Z5", "Z1", PART1_R2_UP, PART1_R2_LOW, zeta2)"            \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ITER_ZIP_ZETAS("Z6", "Z2", PART2_R2_UP, PART2_R2_LOW, zeta)"             \n\
      "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                   \n\
      "ZIP12("Z4", "Z5", PART2_R2_LOW, PART2_R2_UP)"                            \n\
      "ITER_ZIP_ZETAS("Z7", "Z3", PART3_R2_UP, PART3_R2_LOW, zeta2)"            \n\
      "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                   \n\
      "ZIP12("Z6", "Z7", PART3_R2_LOW, PART3_R2_UP)"                            \n\ "

// VL=1024-bit > One level of NTT for option ZIP_ZETAS by initializing the zetas for that level and
// computing r_low and r_up and putting it in registers which are zipped into the right register
// This is done by loading zetas all in at once and zipping them into the right registers
// init_zetas: initialize zetas for that level
// zetas: zeta register to use for first half of level
// zetas2: zeta register to use for second half of level
#define ITER1024_ZIP_ZETAS(init_zetas, zeta)\
    " "init_zetas"                                                              \n\
      "ITER_ZIP_ZETAS("Z2", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"             \n\
      "ITER_ZIP_ZETAS("Z3", "Z1", PART1_R2_UP, PART1_R2_LOW, zeta)"             \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ZIP12("Z2", "Z3", PART1_R2_LOW, PART1_R2_UP)"                            \n\ "

// VL=1024-bit > One level of NTT for option ZIP_ZETAS by initializing the zetas for that level and
// computing r_low and r_up and putting it in registers which are zipped into the right register
// This is done by loading zetas all in at once and zipping them into the right registers
// init_zetas: initialize zetas for that level
// zetas: zeta register to use for first half of level
// zetas2: zeta register to use for second half of level
#define ITER2048_ZIP_ZETAS(init_zetas, zeta)\
    " "init_zetas"                                                              \n\
      "ITER_ZIP_ZETAS("Z1", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"             \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                   \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                   \n\ "

// Do one vector register's worth of butterfly operations of the NTTlevel using values loaded from registers
// r_up: src register of r_up
// r_low: src register of r_low
// r2_up: dest register to store r_up
// r2_low: dest register to store r_low
#define ITER(r_up, r_low, r2_up, r2_low)\
    " ld1h    { z8.h }, p0/z, [%[zetas_level], x13, lsl #1]              //z2=zetas_level                      \n\
      sunpklo    z14.s, z8.h                                             //z4=lower of zetas_level             \n\
      sunpkhi    z15.s, z8.h                                             //z2=upper of zetas_level             \n\
      sunpklo    z12.s, "r_up".h                                         //z5=2/4 of *r                        \n\
      sunpkhi    z13.s, "r_up".h                                         //z3=3/4 of *r                        \n\
      mul    z15.s, p0/m, z15.s, z13.s                                   //z6=r[3/4] * zetas_level[up]         \n\
      mul    z14.s, p0/m, z14.s, z12.s                                   //z7=r[2/4] * zetas_level[low]        \n\
      movprfx z12, z14                                                                                         \n\
      mul    z12.s, p0/m, z12.s, z30.s                                   //z7=multiplied[2/4] * QINV           \n\
      movprfx z13, z15                                                                                         \n\
      mul    z13.s, p0/m, z13.s, z30.s                                   //z6=multiplied[3/4] * QINV           \n\
      asr    z13.s, z13.s, #16                                           //z6=t>>=16                           \n\
      asr    z12.s, z12.s, #16                                           //z7=t>>=16                           \n\
      mul    z12.s, p0/m, z12.s, z31.s                                   //z7=u[2/4] * KYBER_Q                 \n\
      mul    z13.s, p0/m, z13.s, z31.s                                   //z6=u[3/4] * KYBER_Q                 \n\
      add z13.s, z13.s, z15.s                                                                                  \n\
      add z12.s, z12.s, z14.s                                                                                  \n\
      uzp2    z12.h, z12.h, z13.h                                        //z2=zipped t[low] and t[up]          \n\
      inch x13                                                           //x8 + VL                             \n\
      sub    "r2_up".h, "r_low".h, z12.h                                 //z6=r - t                            \n\
      add    "r2_low".h, "r_low".h, z12.h                                //z5=r + t                            \n\ "

// VL=128-bit > Compute r_low and store half of the results in registers, this is an even level
// for the LOAD_ZETAS option
#define ITER128_EVEN_LEVEL\
      " //Compute r_low and store half of the results in registers         \n\
      "ITER128_REG_TO_REG"                                                 \n\
      //Store the other half of the results in temp array r2 (in memory)   \n\
      mov x14, #64                                                         \n\
      "STORE128_RLOW("r2")"                                                \n\
      //Compute first half of r_up and store the results in registers      \n\
      mov x8, xzr                                                          \n\
      add x9, %[r], #128                                                   \n\
      add x11, %[r], #384                                                  \n\
      "ITER128_MEM_TO_REG"                                                 \n\
      //Compute second half of r_up and store the results in temp array r2 (in memory)   \n\
      mov x14, #192                                                        \n\
      "STORE128_RUP("r2")"                                                 \n\ "

// VL=128-bit > Compute r_low and store half of the results in registers,
// this is an even level for the LOAD_ZETAS option
#define ITER128_ODD_LEVEL\
      " //Compute r_low and store half of the results in registers         \n\
      "ITER128_REG_TO_REG"                                                 \n\
      //Store the other half of the results in r array (in memory)         \n\
      mov x14, #64                                                         \n\
      "STORE128_RLOW("r")"                                                 \n\
      //Compute first half of r_up and store the results in registers      \n\
      mov x8, xzr                                                          \n\
      add x9, %[r2], #128                                                  \n\
      add x11, %[r2], #384                                                 \n\
      "ITER128_MEM_TO_REG"                                                 \n\
      //Compute second half of r_up and store the results in r array (in memory)   \n\
      mov x14, #192                                                        \n\
      "STORE128_RUP("r")"                                                  \n\ "

// VL=128-bit > Compute r_low and store half of the results in registers for option ZIP_ZETAS,
// this is an even level for the ZIP_ZETAS option
#define ITER128_ZIP_ZETAS_EVEN_LEVEL(init_zetas, zeta, zeta2, zeta3, zeta4)\
      " "init_zetas"                                                       \n\
      //Compute r_low and store half of the results in registers           \n\
      "ITER128_ZIP_ZETAS_REG_TO_REG(zeta, zeta2, zeta3, zeta4)"            \n\
      //Store the other half of the results in temp array r2 (in memory)   \n\
      mov x14, #64                                                         \n\
      "STORE128_RLOW("r2")"                                                \n\
      //Compute first half of r_up and store the results in registers      \n\
      mov x8, xzr                                                          \n\
      add x9, %[r], #128                                                   \n\
      add x11, %[r], #384                                                  \n\
      "ITER128_ZIP_ZETAS_MEM_TO_REG(zeta, zeta2, zeta3, zeta4)"             \n\
      //Compute second half of r_up and store the results in temp array r2 (in memory)   \n\
      mov x14, #192                                                        \n\
      "STORE128_ZIP_ZETAS_RUP("r2", zeta, zeta2, zeta3, zeta4)"            \n\ "

// VL=128-bit > Compute r_low and store half of the results in registers for option ZIP_ZETAS,
// this is the last level for the ZIP_ZETAS option
// init_zetas: macro to initialize zetas for this level
#define ITER128_ZIP_ZETAS_LAST_LEVEL(init_zetas, zeta, zeta2, zeta3, zeta4)\
    " "init_zetas"                                                       \n\
    //Compute r_low and store half of the results in registers           \n\
    "ITER128_ZIP_ZETAS_LAST_LEVEL_REG_TO_REG(zeta, zeta2, zeta3, zeta4)" \n\
    //Store the other half of the results in temp array r2 (in memory)   \n\
    mov x14, #64                                                         \n\
    "STORE128_RLOW("r2")"                                                \n\
    //Compute first half of r_up and store the results in registers      \n\
    mov x8, xzr                                                          \n\
    add x9, %[r], #128                                                   \n\
    add x11, %[r], #384                                                  \n\
    mov x13, #56                                                         \n\
    "GATHER128_ZETAS_LVL0"                                               \n\
    "ITER128_ZIP_ZETAS_MEM_TO_REG(zeta, zeta2, zeta3, zeta4)"            \n\
    //Compute second half of r_up and store the results in temp array r2 (in memory)   \n\
    mov x14, #192                                                        \n\
    "GATHER128_ZETAS_LVL0"                                               \n\
    "STORE128_ZIP_ZETAS_RUP("r2", zeta, zeta2, zeta3, zeta4)"            \n\ "


// VL=128-bit > Compute r_low and store half of the results in registers, this is an odd level
// for the ZIP_ZETAS option
// init_zetas: macro to initialize zetas for this level
#define ITER128_ZIP_ZETAS_ODD_LEVEL(init_zetas, zeta, zeta2, zeta3, zeta4)\
    " "init_zetas"                                                         \n\
      //Compute r_low and store half of the results in registers           \n\
      "ITER128_ZIP_ZETAS_REG_TO_REG(zeta, zeta2, zeta3, zeta4)"            \n\
      //Store the other half of the results in r array (in memory)         \n\
      mov x14, #64                                                         \n\
      "STORE128_RLOW("r")"                                                 \n\
      //Compute first half of r_up and store the results in registers      \n\
      mov x8, xzr                                                          \n\
      add x9, %[r2], #128                                                  \n\
      add x11, %[r2], #384                                                 \n\
      "ITER128_ZIP_ZETAS_MEM_TO_REG(zeta, zeta2, zeta3, zeta4)"            \n\
      //Compute second half of r_up and store the results in r array (in memory)   \n\
      mov x14, #192                                                        \n\
      "STORE128_ZIP_ZETAS_RUP("r", zeta, zeta2, zeta3, zeta4)"             \n\ "


// Do one vector register's worth of butterfly operations of the NTTlevel using values loaded from registers for option ZIP_ZETAS
// r_up: src register of r_up
// r_low: src register of r_low
// r2_up: dest register to store r_up
// r2_low: dest register to store r_low
// zetas: src register of zetas
#define ITER_ZIP_ZETAS(r_up, r_low, r2_up, r2_low, zeta)\
    " sunpklo    z14.s, "zeta".h                                      //z4=lower of zetas_level                 \n\
      sunpkhi    z15.s, "zeta".h                                      //z2=upper of zetaa_level                 \n\
      sunpklo    z12.s, "r_up".h                                      //z5=2/4 of *r                            \n\
      sunpkhi    z13.s, "r_up".h                                      //z3=3/4 of *r                            \n\
      mul    z15.s, p0/m, z15.s, z13.s                                //z6=r[3/4] * zetas_level[up]             \n\
      mul    z14.s, p0/m, z14.s, z12.s                                //z7=r[2/4] * zetas_level[low]            \n\
      movprfx z12, z14                                                                                          \n\
      mul    z12.s, p0/m, z12.s, z30.s                                //z7=multiplied[2/4] * QINV               \n\
      movprfx z13, z15                                                                                          \n\
      mul    z13.s, p0/m, z13.s, z30.s                                //z6=multiplied[3/4] * QINV               \n\
      asr    z13.s, z13.s, #16                                        //z6=t>>=16                               \n\
      asr    z12.s, z12.s, #16                                        //z7=t>>=16                               \n\
      mul    z12.s, p0/m, z12.s, z31.s                                //z7=u[2/4] * KYBER_Q                     \n\
      mul    z13.s, p0/m, z13.s, z31.s                                //z6=u[3/4] * KYBER_Q                     \n\
      add z13.s, z13.s, z15.s                                                                                   \n\
      add z12.s, z12.s, z14.s                                                                                   \n\
            uzp2    z12.h, z12.h, z13.h                               //z2=zipped t[low] and t[up]              \n\
      sub    "r2_up".h, "r_low".h, z12.h                              //z6=r - t                                \n\
      add    "r2_low".h, "r_low".h, z12.h                             //z5=r + t                                \n\ "

// VL=128-bit > Do one vector register's worth of butterfly operations of the NTTlevel using values loaded from memory
// r2_up: dest register to store r_up
// r2_low: dest register to store r_low
#define ITER128_MEM(r2_up, r2_low)\
    " ld1h    { z8.h }, p0/z, [%[zetas_level], x13, lsl #1]         //z2=zetas_level                      \n\
      ld1h    { z26.h }, p0/z, [x11, x8, lsl #1]                    //z3=r[up]                            \n\
      ld1h    { z25.h }, p0/z, [x9, x8, lsl #1]                     //z3=r[low]                           \n\
      sunpklo    z14.s, z8.h                                        //z4=lower of zetas_level             \n\
      sunpkhi    z15.s, z8.h                                        //z2=upper of zetas_level             \n\
      sunpklo    z12.s, z26.h                                       //z5=2/4 of *r                        \n\
      sunpkhi    z13.s, z26.h                                       //z3=3/4 of *r                        \n\
      mul    z15.s, p0/m, z15.s, z13.s                              //z6=r[3/4] * zetas_level[up]         \n\
      mul    z14.s, p0/m, z14.s, z12.s                              //z7=r[2/4] * zetas_level[low]        \n\
      movprfx z12, z14                                                                                    \n\
      mul    z12.s, p0/m, z12.s, z30.s                              //z7=multiplied[2/4] * QINV           \n\
      movprfx z13, z15                                                                                    \n\
      mul    z13.s, p0/m, z13.s, z30.s                              //z6=multiplied[3/4] * QINV           \n\
      asr    z13.s, z13.s, #16                                      //z6=t>>=16                           \n\
      asr    z12.s, z12.s, #16                                      //z7=t>>=16                           \n\
      mul    z12.s, p0/m, z12.s, z31.s                              //z7=u[2/4] * KYBER_Q                 \n\
      mul    z13.s, p0/m, z13.s, z31.s                              //z6=u[3/4] * KYBER_Q                 \n\
      add z13.s, z13.s, z15.s                                                                             \n\
      add z12.s, z12.s, z14.s                                                                             \n\
      uzp2    z12.h, z12.h, z13.h                                   //z2=zipped t[low] and t[up]          \n\
      inch x13                                                      //x8 + VL                             \n\
      inch x8                                                       //x8 + VL                             \n\
      sub    "r2_up".h, z25.h, z12.h                                //z6=r - t                            \n\
      add    "r2_low".h, z25.h, z12.h                               //z5=r + t                            \n\ "

// VL=128-bit > Do one vector register's worth of butterfly operations of the NTTlevel using values loaded from memory for option ZIP_ZETAS
// r2_up: dest register to store r_up
// r2_low: dest register to store r_low
// zetas: zetas register to use
#define ITER128_ZIP_ZETAS_MEM(r2_up, r2_low, zeta)\
  " ld1h    { z26.h }, p0/z, [x11, x8, lsl #1]                      //z3=r[up]                            \n\
    ld1h    { z25.h }, p0/z, [x9, x8, lsl #1]                       //z3=r[low]                           \n\
    sunpklo    z14.s, "zeta".h                                      //z4=lower of zetas_level             \n\
    sunpkhi    z15.s, "zeta".h                                      //z2=upper of zetas_level             \n\
    sunpklo    z12.s, z26.h                                         //z5=2/4 of *r                        \n\
    sunpkhi    z13.s, z26.h                                         //z3=3/4 of *r                        \n\
    mul    z15.s, p0/m, z15.s, z13.s                                //z6=r[3/4] * zetas_level[up]         \n\
    mul    z14.s, p0/m, z14.s, z12.s                                //z7=r[2/4] * zetas_level[low]        \n\
    movprfx z12, z14                                                                                      \n\
    mul    z12.s, p0/m, z12.s, z30.s                                //z7=multiplied[2/4] * QINV           \n\
    movprfx z13, z15                                                                                      \n\
    mul    z13.s, p0/m, z13.s, z30.s                                //z6=multiplied[3/4] * QINV           \n\
    asr    z13.s, z13.s, #16                                        //z6=t>>=16                           \n\
    asr    z12.s, z12.s, #16                                        //z7=t>>=16                           \n\
    mul    z12.s, p0/m, z12.s, z31.s                                //z7=u[2/4] * KYBER_Q                 \n\
    mul    z13.s, p0/m, z13.s, z31.s                                //z6=u[3/4] * KYBER_Q                 \n\
    add z13.s, z13.s, z15.s                                                                               \n\
    add z12.s, z12.s, z14.s                                                                               \n\
    uzp2    z12.h, z12.h, z13.h                                     //z2=zipped t[low] and t[up]          \n\
    inch x8                                                         //x8 + VL                             \n\
    sub    "r2_up".h, z25.h, z12.h                                  //z6=r - t                            \n\
    add    "r2_low".h, z25.h, z12.h                                 //z5=r + t                            \n\ "

// VL=128-bit > Do a level of the NTT using values loaded from registers and put them
// in specific registers by zipping them
#define ITER128_REG_TO_REG\
    " "ITER("Z9", "Z0", PART0_R2_UP, PART0_R2_LOW)"                  \n\
      "ITER("Z10", "Z1", PART1_R2_UP, PART1_R2_LOW)"                 \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                        \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                        \n\
      "ITER("Z11", "Z2", PART2_R2_UP, PART2_R2_LOW)"                 \n\
      "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                        \n\
      "ITER("Z16", "Z3", PART3_R2_UP, PART3_R2_LOW)"                 \n\
      "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                        \n\
      "ITER("Z17", "Z4", PART4_R2_UP, PART4_R2_LOW)"                 \n\
      "ZIP1("Z4", PART2_R2_LOW, PART2_R2_UP)"                        \n\
      "ZIP12("Z9", "Z10", PART4_R2_LOW, PART4_R2_UP)"                \n\
      "ITER("Z18", "Z5", PART5_R2_UP, PART5_R2_LOW)"                 \n\
      "ZIP2("Z5", PART2_R2_LOW, PART2_R2_UP)"                        \n\
      "ZIP12("Z11", PART4_R2_LOW, PART5_R2_LOW, PART5_R2_UP)"        \n\
      "ITER("Z19", "Z6", PART6_R2_UP, PART6_R2_LOW)"                 \n\
      "ZIP1("Z6", PART3_R2_LOW, PART3_R2_UP)"                        \n\
      "ZIP12("Z17", "Z18", PART6_R2_LOW, PART6_R2_UP)"               \n\
      "ITER("Z20", "Z7", PART7_R2_UP, PART7_R2_LOW)"                 \n\
      "ZIP2("Z7", PART3_R2_LOW, PART3_R2_UP)"                        \n\
      "ZIP12("Z19", "Z20", PART7_R2_LOW, PART7_R2_UP)"               \n\ "

// VL=128-bit > Compute r_low of the NTT for option ZIP_ZETAS using values loaded from registers and put them
// in specific registers by zipping them
#define ITER128_ZIP_ZETAS_REG_TO_REG(zeta, zeta2, zeta3, zeta4)\
    " "ITER_ZIP_ZETAS("Z9", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"                   \n\
      "ITER_ZIP_ZETAS("Z10", "Z1", PART1_R2_UP, PART1_R2_LOW, zeta2)"                 \n\
      "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                         \n\
      "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                         \n\
      "ITER_ZIP_ZETAS("Z11", "Z2", PART2_R2_UP, PART2_R2_LOW, zeta3)"                 \n\
      "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                         \n\
      "ITER_ZIP_ZETAS("Z16", "Z3", PART3_R2_UP, PART3_R2_LOW, zeta4)"                 \n\
      "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                         \n\
      "ITER_ZIP_ZETAS("Z17", "Z4", PART4_R2_UP, PART4_R2_LOW, zeta)"                  \n\
      "ZIP1("Z4", PART2_R2_LOW, PART2_R2_UP)"                                         \n\
      "ZIP12("Z9", "Z10", PART4_R2_LOW, PART4_R2_UP)"                                 \n\
      "ITER_ZIP_ZETAS("Z18", "Z5", PART5_R2_UP, PART5_R2_LOW, zeta2)"                 \n\
      "ZIP2("Z5", PART2_R2_LOW, PART2_R2_UP)"                                         \n\
      "ZIP12("Z11", PART4_R2_LOW, PART5_R2_LOW, PART5_R2_UP)"                         \n\
      "ITER_ZIP_ZETAS("Z19", "Z6", PART6_R2_UP, PART6_R2_LOW, zeta3)"                 \n\
      "ZIP1("Z6", PART3_R2_LOW, PART3_R2_UP)"                                         \n\
      "ZIP12("Z17", "Z18", PART6_R2_LOW, PART6_R2_UP)"                                \n\
      "ITER_ZIP_ZETAS("Z20", "Z7", PART7_R2_UP, PART7_R2_LOW, zeta4)"                 \n\
      "ZIP2("Z7", PART3_R2_LOW, PART3_R2_UP)"                                         \n\
      "ZIP12("Z19", "Z20", PART7_R2_LOW, PART7_R2_UP)"                                \n\ "

// VL=128-bit > Compute r_low of the NTT for option ZIP_ZETAS using values loaded from registers and put them
// in specific registers by zipping them
// This is for the last level, meaning we need to load in zeta,...,zeta4 in the first part and
// zeta5,...,zeta8 in the second part
#define ITER128_ZIP_ZETAS_LAST_LEVEL_REG_TO_REG(zeta, zeta2, zeta3, zeta4)\
  " "ITER_ZIP_ZETAS("Z9", "Z0", PART0_R2_UP, PART0_R2_LOW, zeta)"                   \n\
    "ITER_ZIP_ZETAS("Z10", "Z1", PART1_R2_UP, PART1_R2_LOW, zeta2)"                 \n\
    "ZIP2("Z1", PART0_R2_LOW, PART0_R2_UP)"                                         \n\
    "ZIP1("Z0", PART0_R2_LOW, PART0_R2_UP)"                                         \n\
    "ITER_ZIP_ZETAS("Z11", "Z2", PART2_R2_UP, PART2_R2_LOW, zeta3)"                 \n\
    "ZIP1("Z2", PART1_R2_LOW, PART1_R2_UP)"                                         \n\
    "ITER_ZIP_ZETAS("Z16", "Z3", PART3_R2_UP, PART3_R2_LOW, zeta4)"                 \n\
    "ZIP2("Z3", PART1_R2_LOW, PART1_R2_UP)"                                         \n\
    "GATHER128_ZETAS_LVL0"                                                          \n\
    "ITER_ZIP_ZETAS("Z17", "Z4", PART4_R2_UP, PART4_R2_LOW, zeta)"                  \n\
    "ZIP1("Z4", PART2_R2_LOW, PART2_R2_UP)"                                         \n\
    "ZIP12("Z9", "Z10", PART4_R2_LOW, PART4_R2_UP)"                                 \n\
    "ITER_ZIP_ZETAS("Z18", "Z5", PART5_R2_UP, PART5_R2_LOW, zeta2)"                 \n\
    "ZIP2("Z5", PART2_R2_LOW, PART2_R2_UP)"                                         \n\
    "ZIP12("Z11", PART4_R2_LOW, PART5_R2_LOW, PART5_R2_UP)"                         \n\
    "ITER_ZIP_ZETAS("Z19", "Z6", PART6_R2_UP, PART6_R2_LOW, zeta3)"                 \n\
    "ZIP1("Z6", PART3_R2_LOW, PART3_R2_UP)"                                         \n\
    "ZIP12("Z17", "Z18", PART6_R2_LOW, PART6_R2_UP)"                                \n\
    "ITER_ZIP_ZETAS("Z20", "Z7", PART7_R2_UP, PART7_R2_LOW, zeta4)"                 \n\
    "ZIP2("Z7", PART3_R2_LOW, PART3_R2_UP)"                                         \n\
    "ZIP12("Z19", "Z20", PART7_R2_LOW, PART7_R2_UP)"                                \n\ "

// VL=128-bit > Do a level of the NTT using values loaded from memory and put them
// in specific registers by zipping them
#define ITER128_MEM_TO_REG\
    " "ITER128_MEM("Z22", "Z10")"                                    \n\
      "ZIP12("Z9", "Z10", "Z10", "Z22")"                             \n\
      "ITER128_MEM("Z22", "Z16")"                                    \n\
      "ZIP12("Z11", "Z16", "Z16", "Z22")"                            \n\
      "ITER128_MEM("Z22", "Z18")"                                    \n\
      "ZIP12("Z17", "Z18", "Z18", "Z22")"                            \n\
      "ITER128_MEM("Z22", "Z20")"                                    \n\
      "ZIP12("Z19", "Z20", "Z20", "Z22")"                            \n\ "

// VL=128-bit > Do a level of the NTT for option ZIP_ZETAS using values loaded from memory and put them
// in specific registers by zipping them
#define ITER128_ZIP_ZETAS_MEM_TO_REG(zeta, zeta2, zeta3, zeta4)\
    " "ITER128_ZIP_ZETAS_MEM("Z22", "Z10", zeta)"                    \n\
      "ZIP12("Z9", "Z10", "Z10", "Z22")"                             \n\
      "ITER128_ZIP_ZETAS_MEM("Z22", "Z16", zeta2)"                   \n\
      "ZIP12("Z11", "Z16", "Z16", "Z22")"                            \n\
      "ITER128_ZIP_ZETAS_MEM("Z22", "Z18", zeta3)"                   \n\
      "ZIP12("Z17", "Z18", "Z18", "Z22")"                            \n\
      "ITER128_ZIP_ZETAS_MEM("Z22", "Z20", zeta4)"                   \n\
      "ZIP12("Z19", "Z20", "Z20", "Z22")"                            \n\ "
