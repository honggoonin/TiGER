CC = gcc
CFLAGS=-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

# // [MODIFIED] -lcrypto option is vital for openssl headers
all :
	$(CC) $(CFLAGS) -c PQCgenKAT_kem.c KITE-Q_CCAKEM.c KITE-Q_CPAPKE.c fips202.c xef_ref.c rng.c
	$(CC) $(CFLAGS) -o katkem PQCgenKAT_kem.o KITE-Q_CCAKEM.o KITE-Q_CPAPKE.o fips202.o xef_ref.o rng.o  -lcrypto

run : all
	./katkem

clean :
	rm -f *.o
	rm -f katkem

new :
	make clean
	make all
	./katkem
