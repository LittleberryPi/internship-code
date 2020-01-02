
//
//  PQCgenKAT_kem.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "rng.h"
#include "api.h"
#include "m5ops.h"
#include "ntt.h"
#include "fips202.h"
#include "util.h"

#define	MAX_MARKER_LEN		50
#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

int
main()
{
    char                fn_req[32], fn_rsp[32];
    FILE                *fp_req, *fp_rsp;
    unsigned char       seed[48];
    unsigned char       entropy_input[48];
    unsigned char       ct[CRYPTO_CIPHERTEXTBYTES], ss[CRYPTO_BYTES], ss1[CRYPTO_BYTES];
    int                 count;
    int                 done;
    unsigned char       pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
    int                 ret_val;

    // Check VL and assign function pointer
    int VL;
    asm (
        "addvl %[vl], %[vl], #8"
      : [vl] "=r" (VL)
      : "[vl]" (0)
      : /**no side effects**/
    );
    ntt_asm = NULL;
    invntt_asm = NULL;
    KeccakF1600_sve2 = NULL;
    switch (VL) {
      case 128: {
        ntt_asm = &ntt_asm_128;
        invntt_asm = &invntt_asm_128;
        //Comment this in if compiling with SVE2 compiler:
        // KeccakF1600_sve2 = &KeccakF1600_StatePermute_asm;
        printf("VL=128\n");
        break; }
      case 256: {
        ntt_asm = &ntt_asm_256;
        invntt_asm = &invntt_asm_256;
        //Comment this out if compiling with SVE2 compiler:
        // KeccakF1600_sve2 = &KeccakF1600_StatePermute_asm;
        printf("VL=256\n");
        break; }
      case 512: {
        ntt_asm = &ntt_asm_512;
        invntt_asm = &invntt_asm_512;
        //Comment this out if compiling with SVE2 compiler:
        // KeccakF1600_sve2 = &KeccakF1600_StatePermute_asm;
        printf("VL=512\n");
        break; }
      case 1024: {
        ntt_asm = &ntt_asm_1024;
        invntt_asm = &invntt_asm_1024;
        //Comment this out if compiling with SVE2 compiler:
        // KeccakF1600_sve2 = &KeccakF1600_StatePermute_asm;
        printf("VL=1024\n");
        break; }
      case 2048: {
        ntt_asm = &ntt_asm_2048;
        invntt_asm = &invntt_asm_2048;
        //Comment this out if compiling with SVE2 compiler:
        // KeccakF1600_sve2 = &KeccakF1600_StatePermute_asm_2048;
        printf("VL=2048\n");
        break; }
      default: {
        printf("switch statement failed in main()\n");
        break; }
    }

    // Check which NTT version we are using
    #ifdef INPLACE
        printf("INPLACE\n");
    #elif NTT
        printf("NTT\n");
    #elif ASM_AUTO
        printf("ASM_AUTO\n");
    #elif ASM_HAND
        printf("ASM_HAND\n");
    #endif

    // Check which m5 version we are using
    #ifdef GEM5_TEST_NTT
        printf("GEM5_TEST_NTT\n");
    #elif GEM5_TEST_INVNTT
        printf("GEM5_TEST_INVNTT\n");
    #endif

    // Check if we use the LOAD_ZETAS or ZIP_ZETAS option
    #ifdef LOAD_ZETAS
        printf("LOAD_ZETAS\n");
    #elif ZIP_ZETAS
        printf("ZIP_ZETAS\n");
    #endif

    // Check if we are measuring keygen/encap/decap
    #ifdef KEYGEN_ONLY
        printf("KEYGEN_ONLY\n");
    #elif ENCAP_ONLY
        printf("ENCAP_ONLY\n");
    #elif DECAP_ONLY
        printf("DECAP_ONLY\n");
    #endif


    // Create the REQUEST file
    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    if ( (fp_req = fopen(fn_req, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ( (fp_rsp = fopen(fn_rsp, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

    for (int i=0; i<48; i++)
        entropy_input[i] = i;

    randombytes_init(entropy_input, NULL, 256);
    for (int i=0; i<100; i++) {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 48);
        fprintBstr(fp_req, "seed = ", seed, 48);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
        fprintf(fp_req, "ss =\n\n");
    }
    fclose(fp_req);

    // Create the REQUEST file
    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    if ( (fp_req = fopen(fn_req, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    // if ( (fp_rsp = fopen(fn_rsp, "w")) == NULL ) {
    //     printf("Couldn't open <%s> for write\n", fn_rsp);
    //     return KAT_FILE_OPEN_ERROR;
    // }

    for (int i=0; i<48; i++)
        entropy_input[i] = i;

    randombytes_init(entropy_input, NULL, 256);
    for (int i=0; i<4; i++) {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 48);
        fprintBstr(fp_req, "seed = ", seed, 48);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
        fprintf(fp_req, "ss =\n\n");
    }
    fclose(fp_req);

    //Create the RESPONSE file based on what's in the REQUEST file
    if ( (fp_req = fopen(fn_req, "r")) == NULL ) {
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }

    // fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);
    done = 0;
    do {
        if ( FindMarker(fp_req, "count = ") )
            fscanf(fp_req, "%d", &count);
        else {
            done = 1;
            break;
        }
        // fprintf(fp_rsp, "count = %d\n", count);

        if ( !ReadHex(fp_req, seed, 48, "seed = ") ) {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
            return KAT_DATA_ERROR;
        }
        // fprintBstr(fp_rsp, "seed = ", seed, 48);

        randombytes_init(seed, NULL, 256);

        // Generate the public/private keypair
        #ifdef GEM5_TEST_ALL
            #ifdef KEYGEN_ONLY
                m5_reset_stats(0,0);
            #endif
        #endif
            ret_val = crypto_kem_keypair(pk, sk);
        #ifdef GEM5_TEST_ALL
            #ifdef KEYGEN_ONLY
                m5_dump_stats(0,0);
            #endif
        #endif
        if ( ret_val != 0) {
            printf("crypto_kem_keypair returned <%d>\n", ret_val);
            // return KAT_CRYPTO_FAILURE;
        }
        // fprintBstr(fp_rsp, "pk = ", pk, CRYPTO_PUBLICKEYBYTES);
        // fprintBstr(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);

        #ifdef GEM5_TEST_ALL
            #ifdef ENCAP_ONLY
                m5_reset_stats(0,0);
            #endif
        #endif
            ret_val = crypto_kem_enc(ct, ss, pk);
        #ifdef GEM5_TEST_ALL
            #ifdef ENCAP_ONLY
                m5_dump_stats(0,0);
            #endif
        #endif
        if ( ret_val != 0) {
            printf("crypto_kem_enc returned <%d>\n", ret_val);
            // return KAT_CRYPTO_FAILURE;
        }
        // fprintBstr(fp_rsp, "ct = ", ct, CRYPTO_CIPHERTEXTBYTES);
        // fprintBstr(fp_rsp, "ss = ", ss, CRYPTO_BYTES);

        // fprintf(fp_rsp, "\n");

        #ifdef GEM5_TEST_ALL
            #ifdef DECAP_ONLY
                m5_reset_stats(0,0);
            #endif
        #endif
            ret_val = crypto_kem_dec(ss1, ct, sk);
        #ifdef GEM5_TEST_ALL
            #ifdef DECAP_ONLY
                m5_dump_stats(0,0);
            #endif
        #endif
        if ( ret_val != 0) {
            printf("crypto_kem_dec returned <%d>\n", ret_val);
            // return KAT_CRYPTO_FAILURE;
        }

        if ( memcmp(ss, ss1, CRYPTO_BYTES) ) {
            printf("crypto_kem_dec returned bad 'ss' value\n");
            // return KAT_CRYPTO_FAILURE;
        }

    } while ( !done );

    fclose(fp_req);
    // fclose(fp_rsp);

    return KAT_SUCCESS;
}



//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
	char	line[MAX_MARKER_LEN];
	int		i, len;
	int curr_line;

	len = (int)strlen(marker);
	if ( len > MAX_MARKER_LEN-1 )
		len = MAX_MARKER_LEN-1;

	for ( i=0; i<len; i++ )
	  {
	    curr_line = fgetc(infile);
	    line[i] = curr_line;
	    if (curr_line == EOF )
	      return 0;
	  }
	line[len] = '\0';

	while ( 1 ) {
		if ( !strncmp(line, marker, len) )
			return 1;

		for ( i=0; i<len-1; i++ )
			line[i] = line[i+1];
		curr_line = fgetc(infile);
		line[len-1] = curr_line;
		if (curr_line == EOF )
		    return 0;
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
	int			i, ch, started;
	unsigned char	ich;

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) )
		while ( (ch = fgetc(infile)) != EOF ) {
			if ( !isxdigit(ch) ) {
				if ( !started ) {
					if ( ch == '\n' )
						break;
					else
						continue;
				}
				else
					break;
			}
			started = 1;
			if ( (ch >= '0') && (ch <= '9') )
				ich = ch - '0';
			else if ( (ch >= 'A') && (ch <= 'F') )
				ich = ch - 'A' + 10;
			else if ( (ch >= 'a') && (ch <= 'f') )
				ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;

			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
		}
	else
		return 0;

	return 1;
}

void
fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}
