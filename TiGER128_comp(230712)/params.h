#define PARAMNAME "TiGER128"
#define LWE_N 512		// LWE dim and LWR dim
#define LOG_N 9
#define LOG_Q 8
#define Modulus_Q 256
#define LOG_P 7
#define LOG_K_1 6
#define LOG_K_2 4
#define HR 110			// Hamming weight of coefficient vector r
#define HS 142			// Hamming weight of coefficient vector s
#define HE 32
#define SEED_LEN 32
#define KK_LEN 16
#define size_of_delta 16
#define MESSAGE_LEN 16
#define _8_LOG_T 7
#define sft (sizeof(size_t) * 4 - 1)

#define PUBLICKEYSIZE 32+(LOG_P*LWE_N/8) //***
#define SECRETKEYSIZE 161+MESSAGE_LEN //***
#define CIPHERTEXTSIZE (LOG_K_1*LWE_N/8) + (LOG_K_2*LWE_N/8) //***
