// FrodoKEM: Learning with Errors Key Encapsulation

#include <string.h>

unsigned char ct_verify(const unsigned char *a, const unsigned char *b, size_t len) 
{ // Compare two arrays in constant time.
  // Returns 0 if the byte arrays are equal, -1 otherwise.
    unsigned char r = 0;

    for (size_t i = 0; i < len; i++) {
        r |= a[i] ^ b[i];
    }

    r = (-(unsigned char)(r >> 1) | -(unsigned char)(r & 1)) >> (8*sizeof(unsigned char)-1);
    return (unsigned char)r;
}


void ct_select(unsigned char *r, const unsigned char *a, const unsigned char *b, size_t len, unsigned char selector) 
{ // Select one of the two input arrays to be moved to r
  // If (selector == 0) then load r with a, else if (selector == -1) load r with b

    for (size_t i = 0; i < len; i++) {
        r[i] = (~selector & a[i]) | (selector & b[i]);
    }
}

