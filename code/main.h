#ifndef MAIN_H
#define MAIN_H

void (*ntt_asm)(int16_t*, int16_t*);
void (*invntt_asm)(int16_t*);
void (*KeccakF1600_sve2)(uint64_t*);

#endif
