#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "KITE-Q_CPAPKE.h"
#include "KITE-Q_CCAKEM.h"
#include "fips202.h"
#include "randombytes.h"
#include "params.h"
#include "api.h"

#define	MAX_MARKER_LEN		50
#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

unsigned char pk[32+LWE_N];
unsigned char sk[LWE_N+MESSAGE_LEN];


int KAT_RING() {

	unsigned char c[2*LWE_N];
	int i,j, res=0, res_enc = 0, res_dec = 0;
	unsigned char coin[32];
	unsigned char shared_k1[KK_LEN] = {0,};
	unsigned char shared_k2[KK_LEN] = {0,};

	char                fn_req[32], fn_rsp[32];
    FILE                *fp_req, *fp_rsp;
    unsigned char       seed[32];
    int                 count;
    int                 done;
    int                 ret_val;

    // Create the REQUEST file
    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    if ( (fp_req = fopen(fn_req, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ( (fp_rsp = fopen(fn_rsp, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

 	for (int i=0; i<100; i++) {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 32);
        fprintBstr(fp_req, "seed = ", seed, 32);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n"); // == c
        fprintf(fp_req, "ss =\n\n"); // == shared_k1
    }
    fclose(fp_req);

    if ( (fp_req = fopen(fn_req, "r")) == NULL ) {
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }	

    fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);
    done = 0;
    do {
        if ( FindMarker(fp_req, "count = ") )
            fscanf(fp_req, "%d", &count);
        else {
            done = 1;
            break;
        }
        fprintf(fp_rsp, "count = %d\n", count);

        if ( !ReadHex(fp_req, seed, 32, "seed = ") ) {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
            return KAT_DATA_ERROR;
        }
        fprintBstr(fp_rsp, "seed = ", seed, 32);


		if ( (ret_val=KEM_Keygen(pk,sk)) != 0){
            printf("KEM_Keygen returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
		}
        fprintBstr(fp_rsp, "pk = ", pk, CRYPTO_PUBLICKEYBYTES);
        fprintBstr(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);


		//start_cycle1 = rdtsc();
		if ( (ret_val = KEM_Enc(c, shared_k1, pk)) != 0){
			printf("KEM_Enc returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;		
		};

		fprintBstr(fp_rsp, "ct = ", c, CRYPTO_CIPHERTEXTBYTES);
		fprintBstr(fp_rsp, "ss = ", shared_k1, CRYPTO_BYTES);

        fprintf(fp_rsp, "\n");

		//start_cycle2 = rdtsc();
		if ((ret_val = KEM_dec(shared_k2, c, sk, pk)) != 0){
            printf("KEM_dec returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
		}

        if ( memcmp(shared_k1, shared_k2, CRYPTO_BYTES) ) {
            printf("KEM_dec returned bad 'ss' value\n");
            return KAT_CRYPTO_FAILURE;
        }
	
	} while ( !done );

    fclose(fp_req);
    fclose(fp_rsp);

    return KAT_SUCCESS;

}

void main() {
	printf("\n  //////////////////////////////////////////////////////////////////\n\n");
	printf("\t\t"PARAMNAME" \n\n");
	printf("\t\tKnown Answer Test\n\n");
	printf("  //////////////////////////////////////////////////////////////////\n\n");
	// Enc and Dec
	KAT_RING();
}



//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
	char	line[MAX_MARKER_LEN];
	int		i, len;
	int curr_line;

	len = (int)strlen(marker);
	if ( len > MAX_MARKER_LEN-1 )
		len = MAX_MARKER_LEN-1;

	for ( i=0; i<len; i++ )
	  {
	    curr_line = fgetc(infile);
	    line[i] = curr_line;
	    if (curr_line == EOF )
	      return 0;
	  }
	line[len] = '\0';

	while ( 1 ) {
		if ( !strncmp(line, marker, len) )
			return 1;

		for ( i=0; i<len-1; i++ )
			line[i] = line[i+1];
		curr_line = fgetc(infile);
		line[len-1] = curr_line;
		if (curr_line == EOF )
		    return 0;
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
	int			i, ch, started;
	unsigned char	ich;

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) )
		while ( (ch = fgetc(infile)) != EOF ) {
			if ( !isxdigit(ch) ) {
				if ( !started ) {
					if ( ch == '\n' )
						break;
					else
						continue;
				}
				else
					break;
			}
			started = 1;
			if ( (ch >= '0') && (ch <= '9') )
				ich = ch - '0';
			else if ( (ch >= 'A') && (ch <= 'F') )
				ich = ch - 'A' + 10;
			else if ( (ch >= 'a') && (ch <= 'f') )
				ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;

			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
		}
	else
		return 0;

	return 1;
}

void
fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}

