/**
* FINISH macros for NTT
**/
// VL=128-bit > Store r_low in memory dest
#define STORE128_RLOW(dest)\
    " "STORE_TEMP("Z9", dest)"                                       \n\
      "STORE_TEMP("Z10", dest)"                                      \n\
      "STORE_TEMP("Z11", dest)"                                      \n\
      "STORE_TEMP("Z16", dest)"                                      \n\
      "STORE_TEMP("Z17", dest)"                                      \n\
      "STORE_TEMP("Z18", dest)"                                      \n\
      "STORE_TEMP("Z19", dest)"                                      \n\
      "STORE_TEMP_LAST("Z20", dest)"                                 \n\ "

// VL=128-bit > Compute and store r_up in memory dest for option ZIP_ZETAS
#define STORE128_ZIP_ZETAS_RUP(dest, zeta, zeta2, zeta3, zeta4)\
  " "ITER128_ZIP_ZETAS_MEM("Z23", "Z22", zeta)"                      \n\
    "ZIP12("Z21", "Z22", "Z22", "Z23")"                              \n\
    "STORE_TEMP("Z21", dest)"                                        \n\
    "STORE_TEMP("Z22", dest)"                                        \n\
    "ITER128_ZIP_ZETAS_MEM("Z23", "Z22", zeta2)"                     \n\
    "ZIP12("Z21", "Z22", "Z22", "Z23")"                              \n\
    "STORE_TEMP("Z21", dest)"                                        \n\
    "STORE_TEMP("Z22", dest)"                                        \n\
    "ITER128_ZIP_ZETAS_MEM("Z23", "Z22", zeta3)"                     \n\
    "ZIP12("Z21", "Z22", "Z22", "Z23")"                              \n\
    "STORE_TEMP("Z21", dest)"                                        \n\
    "STORE_TEMP("Z22", dest)"                                        \n\
    "ITER128_ZIP_ZETAS_MEM("Z23", "Z22", zeta4)"                     \n\
    "ZIP12("Z21", "Z22", "Z22", "Z23")"                              \n\
    "STORE_TEMP("Z21", dest)"                                        \n\
    "STORE_TEMP_LAST("Z22", dest)"                                   \n\ "

// VL=128-bit > Compute and store r_up in memory dest
#define STORE128_RUP(dest)\
    " "ITER128_MEM("Z23", "Z22")"                                    \n\
      "ZIP12("Z21", "Z22", "Z22", "Z23")"                            \n\
      "STORE_TEMP("Z21", dest)"                                      \n\
      "STORE_TEMP("Z22", dest)"                                      \n\
      "ITER128_MEM("Z23", "Z22")"                                    \n\
      "ZIP12("Z21", "Z22", "Z22", "Z23")"                            \n\
      "STORE_TEMP("Z21", dest)"                                      \n\
      "STORE_TEMP("Z22", dest)"                                      \n\
      "ITER128_MEM("Z23", "Z22")"                                    \n\
      "ZIP12("Z21", "Z22", "Z22", "Z23")"                            \n\
      "STORE_TEMP("Z21", dest)"                                      \n\
      "STORE_TEMP("Z22", dest)"                                      \n\
      "ITER128_MEM("Z23", "Z22")"                                    \n\
      "ZIP12("Z21", "Z22", "Z22", "Z23")"                            \n\
      "STORE_TEMP("Z21", dest)"                                      \n\
      "STORE_TEMP_LAST("Z22", dest)"                                 \n\ "

// VL=128-bit > Zip registers so the values are in the bitreversed order
#define ZIP128\
    " "ZIP12("Z8", "Z12", "Z0", "Z9")"         \n\
      "ZIP12("Z13", "Z14", "Z1", "Z10")"       \n\
      "ZIP12("Z15", "Z21", "Z2", "Z11")"       \n\
      "ZIP12("Z22", "Z23", "Z3", "Z16")"       \n\
      "ZIP12("Z0", "Z9", "Z4", "Z17")"         \n\
      "ZIP12("Z1", "Z10", "Z5", "Z18")"        \n\
      "ZIP12("Z2", "Z11", "Z6", "Z19")"        \n\
      "ZIP12("Z3", "Z16", "Z7", "Z20")"        \n\ "

// VL=128-bit > Store r from registers into r array in memory
#define STORE128\
    " "STORE("Z8")"                            \n\
      "STORE("Z12")"                           \n\
      "STORE("Z13")"                           \n\
      "STORE("Z14")"                           \n\
      "STORE("Z15")"                           \n\
      "STORE("Z21")"                           \n\
      "STORE("Z22")"                           \n\
      "STORE("Z23")"                           \n\
      "STORE("Z0")"                            \n\
      "STORE("Z9")"                            \n\
      "STORE("Z1")"                            \n\
      "STORE("Z10")"                           \n\
      "STORE("Z2")"                            \n\
      "STORE("Z11")"                           \n\
      "STORE("Z3")"                            \n\
      "STORE_LAST("Z16")" "

// VL=256-bit > Zip registers so the values are in the bitreversed order
#define ZIP256\
    " "ZIP12("Z8", "Z12", "Z0", "Z9")"         \n\
      "ZIP12("Z13", "Z14", "Z1", "Z10")"       \n\
      "ZIP12("Z15", "Z21", "Z2", "Z11")"       \n\
      "ZIP12("Z22", "Z23", "Z3", "Z16")"       \n\
      "ZIP12("Z0", "Z9", "Z4", "Z17")"         \n\
      "ZIP12("Z1", "Z10", "Z5", "Z18")"        \n\
      "ZIP12("Z2", "Z11", "Z6", "Z19")"        \n\
      "ZIP12("Z3", "Z16", "Z7", "Z20")"        \n\ "

// VL=256-bit > Store r from registers into r array in memory
#define STORE256\
    " "STORE("Z8")"                            \n\
      "STORE("Z12")"                           \n\
      "STORE("Z13")"                           \n\
      "STORE("Z14")"                           \n\
      "STORE("Z15")"                           \n\
      "STORE("Z21")"                           \n\
      "STORE("Z22")"                           \n\
      "STORE("Z23")"                           \n\
      "STORE("Z0")"                            \n\
      "STORE("Z9")"                            \n\
      "STORE("Z1")"                            \n\
      "STORE("Z10")"                           \n\
      "STORE("Z2")"                            \n\
      "STORE("Z11")"                           \n\
      "STORE("Z3")"                            \n\
      "STORE_LAST("Z16")" "

// VL=512-bit > Zip registers so the values are in the bitreversed order
#define ZIP512\
    " "ZIP12("Z8", "Z12", "Z0", "Z4")"         \n\
      "ZIP12("Z13", "Z14", "Z1", "Z5")"        \n\
      "ZIP12("Z15", "Z16", "Z2", "Z6")"        \n\
      "ZIP12("Z17", "Z18", "Z3", "Z7")"        \n\ "

// VL=512-bit > Store r from registers into r array in memory
#define STORE512\
    " "STORE("Z8")"                            \n\
      "STORE("Z12")"                           \n\
      "STORE("Z13")"                           \n\
      "STORE("Z14")"                           \n\
      "STORE("Z15")"                           \n\
      "STORE("Z16")"                           \n\
      "STORE("Z17")"                           \n\
      "STORE_LAST("Z18")" "

// VL=1024-bit > Zip registers so the values are in the bitreversed order
#define ZIP1024\
    " "ZIP12("Z8", "Z12", "Z0", "Z2")"         \n\
      "ZIP12("Z13", "Z14", "Z1", "Z3")"        \n\ "

// VL=1024-bit > Store r from registers into r array in memory
#define STORE1024\
    " "STORE("Z8")"                            \n\
      "STORE("Z12")"                           \n\
      "STORE("Z13")"                           \n\
      "STORE_LAST("Z14")" "

// VL=2048-bit > Zip registers so the values are in the bitreversed order
#define ZIP2048\
    " "ZIP12("Z8", "Z12", "Z0", "Z1")"         \n\ "

// VL=2048-bit > Store r from registers into r array in memory
#define STORE2048\
    " "STORE("Z8")"                            \n\
      "STORE("Z12")" "
