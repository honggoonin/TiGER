#include "params.h"
#include "api.h"
#include "TiGER_CCAKEM.h"


//generate keypair
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
	//call the key generation algorithm of pke
	KEM_Keygen(pk, sk);
	return 0;
}

int crypto_kem_enc( unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
	KEM_Enc(ct,ss,pk);
	return 0;
}

int crypto_kem_dec( unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
    KEM_dec(ss, ct, sk, pk);
	return 0;
}