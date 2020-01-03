CC=aarch64-linux-gnu-gcc
CFLAGS=-static -march=armv8.2-a+sve -O3 -Wall -Wextra

SOURCES_GEM5_TEST= cbd.c fips202.c indcpa.c kem.c ntt_constant_geometry.c poly.c polyvec.c zetas.c PQCgenKAT_kem.c reduce.c rng.c verify.c symmetric-fips202.c m5op_arm_A64.S tiny-AES-c/aes.c
HEADERS_GEM5_TEST= api.h cbd.h fips202.h indcpa.h ntt.h params.h poly.h polyvec.h reduce.h rng.h verify.h symmetric.h m5ops.h

# The make targets for Kyber encryption with gem5 reset&dump for NTT
m5_ntt_c_in_place: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_NTT -DNTT_INPLACE -DINVNTT_INPLACE

m5_ntt_c_constant: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_NTT -DNTT_CONSTANT -DINVNTT_CONSTANT

m5_ntt_asm_auto: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_NTT -DNTT_ASM_AUTO -DINVNTT_ASM_AUTO

m5_ntt_asm_hand_load_zetas: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_NTT -DNTT_ASM_HAND -DLOAD_ZETAS -DINVNTT_ASM_HAND

m5_ntt_asm_hand_zip_zetas: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_NTT -DNTT_ASM_HAND -DZIP_ZETAS -DINVNTT_ASM_HAND

# The make targets for Kyber encryption with gem5 reset&dump for INVNTT
m5_invntt_c_in_place: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_INVNTT -DNTT_INPLACE -DINVNTT_INPLACE

m5_invntt_c_constant: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_INVNTT -DNTT_CONSTANT -DINVNTT_CONSTANT

m5_invntt_asm_auto: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_INVNTT -DNTT_ASM_AUTO -DINVNTT_ASM_AUTO

m5_invntt_asm_hand: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_INVNTT -DNTT_ASM_HAND -DZIP_ZETAS -DINVNTT_ASM_HAND

# The make targets for Kyber encryption with gem5 reset&dump for POLY_BASEMUL
m5_poly_basemul_c: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_POLY_BASEMUL -DNTT_INPLACE -DINVNTT_INPLACE

m5_poly_basemul_asm: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_POLY_BASEMUL -DNTT_INPLACE -DINVNTT_INPLACE -DPOLY_BASEMUL_ASM

# The make targets for Kyber encryption with gem5 reset&dump for KECCAK
m5_keccak_c: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_KECCAK -DNTT_INPLACE -DINVNTT_INPLACE

m5_keccak_asm: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_KECCAK -DNTT_INPLACE -DINVNTT_INPLACE -DKECCAK_ASM

# The make targets for Kyber encryption with gem5 reset&dump for whole key generation
m5_keygen_all_c_ref: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DKEYGEN_ONLY -DNTT_INPLACE -DINVNTT_INPLACE

m5_keygen_all_opt: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DKEYGEN_ONLY -DNTT_ASM_HAND -DZIP_ZETAS -DINVNTT_ASM_HAND -DPOLY_BASEMUL_ASM -DPOLY_REDUCE_ASM -DKECCAK_ASM

# The make targets for Kyber encryption with gem5 reset&dump for whole encapsulation
m5_enc_all_c_ref: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DENCAP_ONLY -DNTT_INPLACE -DINVNTT_INPLACE

m5_enc_all_opt: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DENCAP_ONLY -DNTT_ASM_HAND -DZIP_ZETAS -DINVNTT_ASM_HAND -DPOLY_BASEMUL_ASM -DPOLY_REDUCE_ASM -DKECCAK_ASM

# The make targets for Kyber encryption with gem5 reset&dump for whole decapsulation
m5_dec_all_c_ref: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DDECAP_ONLY -DNTT_INPLACE -DINVNTT_INPLACE

m5_dec_all_opt: $(HEADERS_GEM5_TEST) $(SOURCES_GEM5_TEST)
	$(CC) -o $@ $(SOURCES_GEM5_TEST) $(CFLAGS) -DGEM5_TEST_ALL -DDECAP_ONLY -DNTT_ASM_HAND -DZIP_ZETAS -DINVNTT_ASM_HAND -DPOLY_BASEMUL_ASM -DPOLY_REDUCE_ASM -DKECCAK_ASM

m5: m5_ntt_c_in_place m5_ntt_c_constant m5_ntt_asm_auto m5_ntt_asm_hand_zip_zetas m5_ntt_asm_hand_load_zetas \
	m5_invntt_c_in_place m5_invntt_c_constant m5_invntt_asm_auto m5_invntt_asm_hand \
	m5_poly_basemul_c m5_poly_basemul_asm \
	m5_keccak_c m5_keccak_asm \
	m5_keygen_all_c_ref m5_keygen_all_opt \
	m5_enc_all_c_ref m5_enc_all_opt \
	m5_dec_all_c_ref m5_dec_all_opt
m5_scalar: m5_ntt_c_in_place \
	m5_invntt_c_in_place \
	m5_poly_basemul_c \
	m5_keccak_c \
	m5_keygen_all_c_ref \
	m5_enc_all_c_ref \
	m5_dec_all_c_ref

.PHONY: clean

clean:
	-rm m5_*
