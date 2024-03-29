#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "TiGER_CPAPKE.h"
#include "TiGER_CCAKEM.h"
#include "fips202.h"
#include "rng.h"
#include "params.h"

#define iter 10000 // iteration number for keygen & EncDec test
#define testnum 1	// repeatetion number of Enc Dec procedure in a single iteration

// add(rdtsc)//
uint64_t start_cycle1, finish_cycle1, start_cycle2, finish_cycle2, start_cycle3, finish_cycle3, cycles1, cycles2, cycles3, elapsed1, elapsed2;
long long rdtsc(void)
{
  unsigned long long result;
  __asm__ volatile(".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax"
    : "=a" (result) ::  "%rdx");
  return result;
}
// add(rdtsc)//


unsigned char pk[PUBLICKEYSIZE];//***
unsigned char sk[SECRETKEYSIZE];//***


void EncDecTest_RING() {

	unsigned char c[CIPHERTEXTSIZE]; //***
	int i,j, res=0, res_enc = 0, res_dec = 0;
	unsigned char coin[32];
	unsigned char shared_k1[KK_LEN] = {0,};
	unsigned char shared_k2[KK_LEN] = {0,};
	cycles1=0;
	cycles2=0;
	elapsed1 = 0;
	elapsed2 = 0;
	int fail = 0;
	int startTime, endTime;
	float execTime;

	startTime = clock();
	for (int l = 0; l < iter; ++l) {

		start_cycle3 = rdtsc();
		res=KEM_Keygen(pk,sk);
		finish_cycle3 = rdtsc();
		cycles3 += (finish_cycle3 - start_cycle3);
		for (i = 0; i < testnum; i++) {

			start_cycle1 = rdtsc();
			res_enc = KEM_Enc(c, shared_k1, pk);
			finish_cycle1 = rdtsc();
			cycles1 += (finish_cycle1 - start_cycle1);

			start_cycle2 = rdtsc();
			res_dec = KEM_dec(shared_k2, c, sk, pk);
			finish_cycle2 = rdtsc();
			cycles2 += (finish_cycle2 - start_cycle2);
		// Correctness check
			for (j = 0; j < KK_LEN; ++j) {
				if (shared_k1[j] != shared_k2[j]) {
					printf("Correctness Error, %d\n", j);
					break;
				}
			}
		}
	}
	endTime = clock();
	execTime = (float)(endTime - startTime)/CLOCKS_PER_SEC;

	printf("Execution time: %f\n", execTime);

	printf("    Key Cycles: %lu \n",  cycles3/iter);
	printf("    Enc Cycles: %lu \n",  cycles1/iter/testnum);
	printf("    Dec Cycles: %lu \n",  cycles2/iter/testnum);
}


void main() {
	printf(
"\t                ,,                                     \n"
"\t  MMPmmMMmmYMM  db   .g8mmmbgd  7MMmmmYMM   7MMmmmMq.  \n"
"\t  P'   MM   `7     .dP'     'M   MM     7    MM    MM. \n"
"\t       MM    ` 7MM dM'       '   MM   d      MM   ,M9  \n"
"\t       MM       MM MM            MMmmMM      MMmmdM9   \n"
"\t       MM       MM MM.   F`7MMF' MM   Y  ,   MM  YM.   \n"
"\t       MM       MM 'Mb.  `  MM   MM     ,M   MM   `Mb. \n"
"\t     .JMML.   .JMML. `'bmmmdPY .JMMmmmmMMM .JMML. .JMM.\n"  
"\n* Tiny bandwidth key encapsulation mechanism for easy miGration based on RLWE(R) *\n"                                                  
	);
	printf("\t\t"PARAMNAME" \n\n");
	// Enc and Dec
	EncDecTest_RING();
}
