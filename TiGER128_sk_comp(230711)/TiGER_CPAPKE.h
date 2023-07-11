#include <stdint.h>
#include "params.h"


int Keygen(unsigned char *pk, unsigned char *sk);

int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin);

int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk);


void compress9to8(uint16_t* t, unsigned char* s);
void recover8to9(uint16_t* t, const unsigned char* s);

