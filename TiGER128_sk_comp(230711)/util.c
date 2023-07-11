// FrodoKEM: Learning with Errors Key Encapsulation

#include <string.h>
#include <stdint.h>

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


void compress9to8(uint16_t* t, unsigned char* s){
	s[0] = (t[0] >> 1) & 0xFF;
	s[1] = ((t[0] & 0x01) << 7) | ((t[1] >> 2) & 0x7F);
	s[2] = ((t[1] & 0x03) << 6) | ((t[2] >> 3) & 0x3F);
	s[3] = ((t[2] & 0x07) << 5) | ((t[3] >> 4) & 0x1F);
	s[4] = ((t[3] & 0x0F) << 4) | ((t[4] >> 5) & 0x0F);
	s[5] = ((t[4] & 0x1F) << 3) | ((t[5] >> 6) & 0x07);
	s[6] = ((t[5] & 0x3F) << 2) | ((t[6] >> 7) & 0x03);
	s[7] = ((t[6] & 0x7F) << 1) | ((t[7] >> 8) & 0x01);
	s[8] = ((t[7] & 0xFF) << 0);
}

void recover8to9(uint16_t* t, char* s){
	t[0] = ((s[0] & 0xFF) << 1) | ((s[1] >> 7) & 0x01);
	t[1] = ((s[1] & 0x7F) << 2) | ((s[2] >> 6) & 0x03);
	t[2] = ((s[2] & 0x3F) << 3) | ((s[3] >> 5) & 0x07);
	t[3] = ((s[3] & 0x1F) << 4) | ((s[4] >> 4) & 0x0F);
	t[4] = ((s[4] & 0x0F) << 5) | ((s[5] >> 3) & 0x1F);
	t[5] = ((s[5] & 0x07) << 6) | ((s[6] >> 2) & 0x3F);
	t[6] = ((s[6] & 0x03) << 7) | ((s[7] >> 1) & 0x7F);
	t[7] = ((s[7] & 0x01) << 8) | ((s[8] >> 0) & 0xFF);
}

