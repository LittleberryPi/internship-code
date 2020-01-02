#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"





/*************************************************
* Name:        print_vect
*
* Description: Print contents of array representing a vector for debug purposes
*
* Arguments:   - char* descr        description of the vector to be printed
*              - T *vector:         pointer to vector to be printed
*              - uint8_t len:       number of elements to print
*              - uint8_t per_line   number of elements printed per line
**************************************************/
void print_vec(char* descr, void *vector, TYPE t, uint16_t len,
               uint16_t per_line)
{
    uint16_t VL_max;
    switch (t) {
        case (U8):
            VL_max = 256;
            break;
        case (U16):
            VL_max = 128;
            break;
        case (U32):
            VL_max = 64;
            break;
        case (U64):
            VL_max = 32;
            break;
        default:
            printf("ERROR - print_vec: unknown type");
            exit(1);
    }
    if (len > VL_max) {
        printf("ERROR - print_vec: len > max. VL");
        exit(1);
    }

    uint64_t var;

    printf("%s\n", descr);
    for (uint16_t i = 0; i < len; i++) {
        switch (t) {
            case (U8):
                var = ((uint8_t*)vector)[i];
                break;
            case (U16):
                var = ((uint16_t*)vector)[i];
                break;
            case (U32):
                var = ((uint32_t*)vector)[i];
                break;
            case (U64):
                var = ((uint64_t*)vector)[i];
                break;
        }

        printf("\t%2i: %16lx", i, var);
        if ((i+1) % per_line == 0) {
            printf("\n");
        }
    }
    printf("\n");
}