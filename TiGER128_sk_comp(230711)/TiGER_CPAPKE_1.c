#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "TiGER_CPAPKE.h"
#include "rng.h"
#include "fips202.h"
#include "xef.h"
#include "D2.h"
#include "util.h"


int Keygen(unsigned char *pk, unsigned char *sk){
	unsigned char pk_a[LWE_N];
	unsigned char pk_b[LWE_N*2]={0,};
	unsigned char seed_a[SEED_LEN], seed_s[SEED_LEN];
	int i, j;

//// Step1 : Gen Seed_a and Seed_s using PRG(randombytes) ////
	randombytes(seed_a, SEED_LEN);
	randombytes(seed_s, SEED_LEN);


//// Step2 : Gen poly a using Expender(SHAKE256) //// 
	shake256(pk_a, LWE_N, seed_a, SEED_LEN);


//// Step3 : Gens poly s and s_idx. s_idx is a index in -1 or 1 ////
	unsigned char tmp_s[HS*4];
	uint16_t sk_s[HS];
	unsigned int sk_random_idx;
	int hw=0, count = 0, neg_start=0, back_position = HS;
	memset(sk_t, 0, LWE_N);

	shake256(tmp_s, HS*3, seed_s, SEED_LEN);
	unsigned char ts;
	while (hw < HS) {
		sk_random_idx = tmp_s[count++]; 
		sk_random_idx <<= 8;
		sk_random_idx ^= tmp_s[count++];
		ts=(sk_random_idx&0x02)-1;
		sk_random_idx >>= 2;
		sk_random_idx &= (LWE_N - 1);
		if (sk[sk_random_idx] == 0) {
			sk[sk_random_idx] = ts;
			hw++;
			if (sk[sk_random_idx]==0x01){sk_s[neg_start++] = sk_random_idx;}
			if (sk[sk_random_idx]==0xff){sk_s[--back_position] = sk_random_idx;}
		}
		if (count >= HS*3 - 3) {
			shake256(tmp_s, HS*3, tmp_s, HS*3);
			printf("Make the tmp_s. \n");
			count = 0;
		}
	}
///////////////Compress sk/////////////	



//// Step4 : Gen poly_b := (p/q)*a*s ; p=128 ////
	for (i = 0; i < HS; ++i) {
		uint16_t deg = sk_s[i];
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		if(branch==0x0001){
				for (j = 0; j < LWE_N; ++j) {pk_b[deg + j] += pk_a[j];}
		}
		if(branch==0xffff){
				for (j = 0; j < LWE_N; ++j) {pk_b[deg + j] += ((~pk_a[j])+0x01);}
		}
	}
	for (i = 0; i < LWE_N; ++i) {pk_b[i] -= pk_b[LWE_N + i];}
	for (i = 0; i < LWE_N; ++i) {pk_b[i] = ((pk_b[i] + 0x01) & 0xfe);} // 16=0x08/0xf0, 32=0x04/0xf8 64=0x02/0xfc, 128= 0x01 0xfe



//// Step5 : Concat seed_genA || pk_b ////
	memcpy(pk, seed_a, SEED_LEN);
	memcpy(pk+SEED_LEN, pk_b, LWE_N);

	return 0;
}


int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin){ 
	int i, j;
	unsigned char c1[LWE_N*2]={0,};
	unsigned char c2[LWE_N*2]={0,};	
	

//// Step1 : Gen r_poly and r_idx ////
	unsigned char tmp_r[HR*4];
	uint16_t r_idx[HR];
	unsigned int r_random_idx; 
	int hw=0, count = 0, neg_start = 0, back_position = HR;

	unsigned char r[LWE_N]={0,};
	shake256(tmp_r, HR*3, coin, SEED_LEN);
	unsigned char tr;
	while (hw < HR) {
		r_random_idx = tmp_r[count++]; 
		r_random_idx <<= 8;	
		r_random_idx ^= tmp_r[count++];
		tr=(r_random_idx&0x02)-1;
		r_random_idx >>= 2;		
		r_random_idx &= (LWE_N - 1);  
		if (r[r_random_idx] == 0) {
			r[r_random_idx] = tr;
			hw++;
			if (r[r_random_idx] == 0x01){r_idx[neg_start++] = r_random_idx;}
			if (r[r_random_idx] == 0xff){r_idx[--back_position] = r_random_idx;}
		}
		if (count >= (HR*3 - 3)) { 
			shake256(tmp_r, HR*3, tmp_r, HR*3);
			count = 0;
			printf("Make the tmp_r. \n");
		}
	}


//// Step2~3 : Gen Seed_e1, Seed_e2 ////
	coin[SEED_LEN-1]=coin[SEED_LEN-1]+1; // ADD NONCE (is 1)
	
	unsigned char Seed_e1[SEED_LEN];
	memcpy(Seed_e1, coin, SEED_LEN);

	unsigned char Seed_e2[SEED_LEN];
	coin[SEED_LEN-1]=coin[SEED_LEN-1]+1;  // ADD NONCE (is 1)
	memcpy(Seed_e2, coin, SEED_LEN);


//// Step4 : Gen poly_e1 and poly_e2. //// 
	unsigned char tmp_e1[HE*3], tmp_e2[HE*3];
	unsigned int e1_random_idx, e2_random_idx; 
	hw=0, count = 0;
	
	shake256(tmp_e1, HE*3, Seed_e1, SEED_LEN);
	unsigned char te1;
	while (hw < HE) {
		e1_random_idx = tmp_e1[count++]; 
		e1_random_idx <<= 8;	
		e1_random_idx ^= tmp_e1[count++];
		te1=(e1_random_idx&0x02)-1;
		e1_random_idx >>= 2;
		e1_random_idx &= (LWE_N - 1);  
		if (c1[e1_random_idx] == 0) {
			c1[e1_random_idx] = te1;
			hw++;
		}
		if (count >= (HE*3 - 3)) { 
			shake256(tmp_e1, HE*3, tmp_e1, HE*3);
			count = 0;
			printf("Make the tmp_e1.\n");
		}
	}

	hw=0, count = 0;
	
	shake256(tmp_e2, HE*3, Seed_e2, SEED_LEN);
	unsigned char te2;
	while (hw < HE) {
		e2_random_idx = tmp_e2[count++]; 
		e2_random_idx <<= 8;	
		e2_random_idx ^= tmp_e2[count++];
		te2=(e2_random_idx&0x02)-1;
		e2_random_idx >>= 2;
		e2_random_idx &= (LWE_N - 1);  
		if (c2[e2_random_idx] == 0) {
			c2[e2_random_idx] = te2;
			hw++;
		}
		if (count >= (HE*3 - 3)) { 
			shake256(tmp_e2, HE*3, tmp_e2, HE*3);
			count = 0;
			printf("Make the tmp_e2.\n");
		}
	}


//// Step5 : Parse seed_a||pk_b from pk //// 
	unsigned char pk_a[LWE_N];
	unsigned char pk_b[LWE_N];
	unsigned char seed_a[SEED_LEN];

	memcpy(seed_a, pk, SEED_LEN);
	memcpy(pk_b, pk+SEED_LEN, LWE_N);


//// Step6 : Gen poly_a //// 
	shake256(pk_a, LWE_N, seed_a, SEED_LEN);


//// Step7~9 : (1) eccENC(M) -> (2) c1=a*r+e1 and c2=b*r+e2 -> (3) c={(k1/q)*c1 || (k2/q)*c2} //// 
// (1-1) Encoding Message using XEf
	unsigned char MSG_hat[MESSAGE_LEN*2]={0,};
	memcpy(MSG_hat, Message, MESSAGE_LEN);
	xef_compute(MSG_hat, MESSAGE_LEN, 3); // f=3 
// (1-2) Encoding Code-word(=MSG_hat) using D2 and add to c2.
   /* We gratefully used the code by https://github.com/newhopecrypto/newhope.git */
	unsigned int mask;
	for(i=0;i<MESSAGE_LEN*2;i++) 
	{
		for(j=0;j<8;j++)
		{
			mask = -((MSG_hat[i] >> j)&1);
			c2[8*i+j] = mask & (Modulus_Q/2);
			c2[8*i+j+LWE_N/2] = mask & (Modulus_Q/2);
		}
	}
// (2) Compute a * r + e1 = c1 and b * r + e2 = c2.
	for(i = 0; i < HR; ++i){
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		uint16_t deg = r_idx[i];
		if(branch==0x0001){
				for (j = 0; j < LWE_N; ++j) {c1[deg+j] += pk_a[j]; c2[deg+j] += pk_b[j];}
		}
		if(branch==0xffff){
				for (j = 0; j < LWE_N; ++j) {c1[deg + j] += ((~pk_a[j])+0x01); c2[deg + j] += ((~pk_b[j])+0x01);}
		}
	}
	for(j = 0; j < LWE_N; ++j){
		c1[j] -= c1[LWE_N+j];
		c2[j] -= c2[LWE_N+j];
	}
// (3) Send c1h_a and c1h_b from mod q to mod k1(64) and mod k2(16).
	for (i=0; i< LWE_N; ++i) {
		c[i] = ((c1[i] + 0x02) & 0xfc);
		c[LWE_N + i] = ((c2[i] + 0x08) & 0xf0);    // 8=0x10/0xe0, 16=0x08/0xf0, 32=0x04/0xf8 64=0x02/0xfc, 128= 0x01 0xfe
	}

	return 0;
}


int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk){
	int i, j;

	unsigned char c1_hat[LWE_N*2] = { 0, };
	unsigned char c2_hat[LWE_N*2] = { 0, };
	unsigned char decomp_delta[LWE_N*2]={0,};

//// Step1 : (1) Parsing c and (2) Gen poly s. ////

// (1) Parsing c. where, decomp_delta = c2_hat.
	memcpy(c1_hat, c, LWE_N);
	memcpy(decomp_delta, c+LWE_N, LWE_N);
	
// (2) Gen s_idx
	uint16_t sk_s[HS]={0,};
	int neg_start = 0, back_position = HS;	
	
	for (i=0; i<17; ++i){
		recover8to9(&sk_s[8*i], &sk[9*i]);
	}
	sk_s[136] = ((sk[153] & 0xFF) << 1) | ((sk[154] >> 7) & 0x01);
	sk_s[137] = ((sk[154] & 0x7F) << 2) | ((sk[155] >> 6) & 0x03);
	sk_s[138] = ((sk[155] & 0x3F) << 3) | ((sk[156] >> 5) & 0x07);
	sk_s[139] = ((sk[156] & 0x1F) << 4) | ((sk[157] >> 4) & 0x0F);
	sk_s[140] = ((sk[157] & 0x0F) << 5) | ((sk[158] >> 3) & 0x1F);
	sk_s[141] = ((sk[158] & 0x07) << 6) | ((sk[159] >> 2) & 0x3F);	
	neg_start = sk[160];
/*	
	for (i = 0; i < LWE_N; ++i) {
		if (sk[i] == 0x01){sk_s[neg_start++] = i;}
		if (sk[i] == 0xff){sk_s[--back_position] = i;}
	}
*/
//// Step2 : Compute Message_hat_prime = c2 - c1 * s. ////

	for(i = 0; i < HS; ++i){
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		uint16_t deg = sk_s[i];
		if(branch==0x0001){
				for (j = 0; j < LWE_N; ++j) {decomp_delta[deg+j] -= c1_hat[j];}
		}
		if(branch==0xffff){
				for (j = 0; j < LWE_N; ++j) {decomp_delta[deg+j] -= ((~c1_hat[j])+0x01);}
		}
	}
	for(j = 0; j < LWE_N; ++j){decomp_delta[j] -= decomp_delta[LWE_N+j];}



//// Step3 : Get original Message using D2 -> Xef. ////
// (1) D2 Decoding
  	uint16_t t;
	unsigned char delta_hat[size_of_delta*2]={0,};
	
	for(i=0;i<LWE_N/2;i++) 
	{
	    t  = flipabs(decomp_delta[i+  0]);
	    t += flipabs(decomp_delta[i+LWE_N/2]);
		t = ((t - Modulus_Q/2));
	    t >>= 15;
		delta_hat[i>>3] |= t<<(i&7);
	}
// (2) Xef Decoding
	xef_compute(delta_hat, size_of_delta, 3);
	xef_fixerr(delta_hat, size_of_delta, 3);
	memcpy(Message, delta_hat, size_of_delta);
	
	return 0;
}