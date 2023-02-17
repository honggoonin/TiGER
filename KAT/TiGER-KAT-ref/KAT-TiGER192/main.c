#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "TiGER_CPAPKE.h"
#include "TiGER_CCAKEM.h"
#include "fips202.h"
#include "rng.h"
#include "params.h"

#define iter 1 // iteration number for keygen & EncDec test
#define testnum 1000000	// repeatetion number of Enc Dec procedure in a single iteration

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


unsigned char pk[32+LWE_N];
unsigned char sk[LWE_N+MESSAGE_LEN];

void progress_bar(int count) {
	const char bar = "+";
	const char blank = " ";
	const int LEN = 40;
	const int MAX = testnum;
	const int SPEED = 2000;
	int i;
	float tick = (float)100/LEN;
	int bar_count;
	float percent;

	printf("\r%d/%d [", count, MAX);
	percent = (float)count/MAX*100;
	bar_count = percent/tick;
	for(i=0; i<LEN; i++) {
		if(bar_count > i){
			printf("%c", bar);
		} 
	}
	printf("% *c",LEN-bar_count+1);
	printf("] %0.2f%%", percent);
	fflush(stdout);
	//usleep(SPEED);
}



void EncDecTest_RING() {

	unsigned char c[2*LWE_N];
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
		clock_t start = clock();
		printf("\n암호 키 생성을 시작합니다.\n");
		res=KEM_Keygen(pk,sk);
		clock_t end = clock();
		printf("암호 키 생성을 완료합니다.(소요시간: %lf 초)\n", (double)(end-start)/CLOCKS_PER_SEC);
		finish_cycle3 = rdtsc();
		cycles3 += (finish_cycle3 - start_cycle3);
		printf("\n%d 회의 암복호화 테스트를 실시합니다.\n\n", testnum);
		for (i = 0; i < testnum; i++) {
			progress_bar(i+1);
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

	printf("\n\n오류 없이 테스트를 완료했습니다.(소요시간: %lf 초)\n\n", (double)execTime);
	
	/*
	printf("Execution time: %f\n", execTime);

	printf("    Key Cycles: %lu \n",  cycles3/iter);
	printf("    Enc Cycles: %lu \n",  cycles1/iter/testnum);
	printf("    Dec Cycles: %lu \n",  cycles2/iter/testnum);
	*/
}


void main() {
	//printf("\n  //////////////////////////////////////////////////////////////////\n\n");
	//printf("\t\t"PARAMNAME" \n\n");
	//printf("\t\tPerformance Test\n\n");
	//printf("  //////////////////////////////////////////////////////////////////\n\n");
	// Enc and Dec
	printf("%s\n", PARAMNAME);
	printf("\n");

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
	EncDecTest_RING();
	//test_progress_bar();
}
