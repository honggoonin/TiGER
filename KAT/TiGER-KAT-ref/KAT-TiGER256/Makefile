CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

# // [MODIFIED] -lcrypto option is vital for openssl headers (related to rng.h)
all :
	$(CC) $(CFLAGS) -c PQCgenKAT_kem.c TiGER_CCAKEM.c TiGER_CPAPKE.c fips202.c xe5.c rng.c kem.c
	$(CC) $(CFLAGS) -o katkem PQCgenKAT_kem.o  TiGER_CCAKEM.o TiGER_CPAPKE.o fips202.o xe5.o rng.o kem.o -lcrypto
	
run : all
	./katkem

clean :
	rm -f *.o
	rm -f katkem

new :
	make clean
	make all
	./katkem

