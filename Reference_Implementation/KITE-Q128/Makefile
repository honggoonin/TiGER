CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

all :
	$(CC) $(CFLAGS) -c KITE-Q_CCAKEM.c KITE-Q_CPAPKE.c main.c fips202.c xef_ref.c rng.c
	$(CC) $(CFLAGS) -o test_CCAKEM KITE-Q_CCAKEM.o KITE-Q_CPAPKE.o main.o fips202.o xef_ref.o rng.o -lcrypto
	
run : all
	./test_CCAKEM

clean :
	rm -f *.o
	rm -f test_CCAKEM

new :
	make clean
	make all
	./test_CCAKEM
