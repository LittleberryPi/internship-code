/**
* FINISH macros for INVNTT
**/
// VL=128-bit > Store r from registers into r array in memory
#define INVSTORE128_FROM_REG\
  " "INVSTORE("Z0")"      //128                     \n\
    "INVSTORE("Z1")"      //136                     \n\
    "INVSTORE("Z2")"      //144                     \n\
    "INVSTORE("Z3")"      //152                     \n\
    "INVSTORE("Z21")"     //192                     \n\
    "INVSTORE("Z22")"     //200                     \n\
    "INVSTORE("Z23")"     //208                     \n\
    "INVSTORE("Z24")"     //216                     \n\
    "INVSTORE("Z4")"      //160                     \n\
    "INVSTORE("Z5")"      //168                     \n\
    "INVSTORE("Z6")"      //176                     \n\
    "INVSTORE("Z7")"      //184                     \n\
    "INVSTORE("Z9")"      //224                     \n\
    "INVSTORE("Z10")"     //232                     \n\
    "INVSTORE("Z11")"     //240                     \n\
    "INVSTORE_LAST("Z16")" //248                    \n\ "

// VL=128-bit > Store r orignally from memory to registers into r array in memory
#define INVSTORE128_FROM_MEM\
  " "INVSTORE("Z0")"      //128                     \n\
    "INVSTORE("Z1")"      //136                     \n\
    "INVSTORE("Z2")"      //144                     \n\
    "INVSTORE("Z3")"      //152                     \n\
    "INVSTORE("Z4")"     //192                      \n\
    "INVSTORE("Z5")"     //200                      \n\
    "INVSTORE("Z6")"     //208                      \n\
    "INVSTORE("Z7")"     //216                      \n\
    "INVSTORE("Z21")"      //160                    \n\
    "INVSTORE("Z22")"      //168                    \n\
    "INVSTORE("Z23")"      //176                    \n\
    "INVSTORE("Z24")"      //184                    \n\
    "INVSTORE("Z9")"      //224                     \n\
    "INVSTORE("Z10")"     //232                     \n\
    "INVSTORE("Z11")"     //240                     \n\
    "INVSTORE_LAST("Z16")" //248  "

// VL=256-bit > Store r from registers into r array in memory
#define INVSTORE256\
  " "INVSTORE("Z0")"                           \n\
    "INVSTORE("Z1")"                           \n\
    "INVSTORE("Z2")"                           \n\
    "INVSTORE("Z3")"                           \n\
    "INVSTORE("Z4")"                           \n\
    "INVSTORE("Z5")"                           \n\
    "INVSTORE("Z6")"                           \n\
    "INVSTORE("Z7")"                           \n\
    "INVSTORE("Z21")"                          \n\
    "INVSTORE("Z22")"                          \n\
    "INVSTORE("Z23")"                          \n\
    "INVSTORE("Z24")"                          \n\
    "INVSTORE("Z9")"                           \n\
    "INVSTORE("Z10")"                          \n\
    "INVSTORE("Z11")"                          \n\
    "INVSTORE_LAST("Z16")" "

// VL=512-bit > Store r from registers into r array in memory
#define INVSTORE512\
  " "INVSTORE("Z0")"                           \n\
    "INVSTORE("Z1")"                           \n\
    "INVSTORE("Z2")"                           \n\
    "INVSTORE("Z3")"                           \n\
    "INVSTORE("Z4")"                           \n\
    "INVSTORE("Z5")"                           \n\
    "INVSTORE("Z6")"                           \n\
    "INVSTORE_LAST("Z7")" "

// VL=1024-bit > Store r from registers into r array in memory
#define INVSTORE1024\
  " "INVSTORE("Z0")"                           \n\
    "INVSTORE("Z1")"                           \n\
    "INVSTORE("Z2")"                           \n\
    "INVSTORE_LAST("Z3")" "

// VL=2048-bit > Store r from registers into r array in memory
#define INVSTORE2048\
  " "INVSTORE("Z0")"                           \n\
    "INVSTORE_LAST("Z1")" "

// VL=512-bit, 1024-bit and 2048-bit > At the end of invntt, multiply every element in r with
// zetas_inv[127] and do montgomery_reduce
// src: the src and target register to be multiplied by zetas_inv[127] and reduced
#define MUL_MONT_ZETA(src)\
    " sunpklo	z19.s, "src".h                                   //sub_low                                    \n\
    sunpkhi	z27.s, "src".h                                       //sub_up                                     \n\
    mul	z27.s, p0/m, z27.s, z8.s                                 //multiplied_up=sub_up*zeta_up               \n\
    mul	z19.s, p0/m, z19.s, z8.s                                 //multiplied_low=sub_low*zeta_low            \n\
    movprfx z17, z19                                                                                          \n\
    mul	z17.s, p0/m, z17.s, z30.s                                //a_low=multiplied_low*qinv                  \n\
    movprfx z18, z27                                                                                          \n\
    mul	z18.s, p0/m, z18.s, z30.s                                //a_up=multiplied_up*qinv                    \n\
    asr	z18.s, z18.s, #16                                        //a_up >> 16                                 \n\
    asr	z17.s, z17.s, #16                                        //a_low >> 16                                \n\
    mul	z17.s, p0/m, z17.s, z31.s                                //tmp_reduce_low=a_low*kyber_q               \n\
    mul	z18.s, p0/m, z18.s, z31.s                                //tmp_reduce_up=a_up*kyber_q                 \n\
    add z17.s, z17.s, z19.s                                                                                   \n\
    add z18.s, z18.s, z27.s                                                                                   \n\
    uzp2 "src".h, z17.h, z18.h                                                                                \n\ "
