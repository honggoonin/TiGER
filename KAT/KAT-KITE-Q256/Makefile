CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

all :
	$(CC) $(CFLAGS) -c KITE-Q_CCAKEM.c KITE-Q_CPAPKE.c main.c randombytes.c fips202.c xe5.c
	$(CC) $(CFLAGS) -o test_KAT KITE-Q_CCAKEM.o KITE-Q_CPAPKE.o main.o randombytes.o fips202.o xe5.o
	
run : all
	./test_KAT

clean :
	rm -f *.o
	rm -f test_KAT

new :
	make clean
	make all
	./test_KAT
