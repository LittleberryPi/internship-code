#ifndef UTIL_H
#define UTIL_H

#define UNUSED(x) (void)(x)  // Macro used to avoid warnings about unused paramters



typedef enum {
    U8,
    U16,
    U32,
    U64
} TYPE;

void print_vec(char* descr, void *vector, TYPE t, uint16_t len,
               uint16_t per_line);




#endif // UTIL_H
