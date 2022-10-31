#include <stdint.h>
#include "params.h"
#include "KITE-Q_CPAPKE.h"


int KEM_Keygen(unsigned char *pk, unsigned char *sk);

int KEM_Enc(unsigned char *c, unsigned char *shared_k, unsigned char *pk);

int KEM_dec(unsigned char *shared_k, unsigned char *c, unsigned char *sk, unsigned char *pk);

