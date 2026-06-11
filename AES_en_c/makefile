CC = gcc
CFLAGS = -Wall -Wextra -O0 -g#-fsanitize=address -fsanitize=undefined -fsanitize=shift -fsanitize=shift-base \
-fsanitize=shift-exponent -fsanitize=integer-divide-by-zero -fsanitize=unreachable \
-fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=bounds-strict \
-fsanitize=bounds -fsanitize=bool -fsanitize=signed-integer-overflow \
-fsanitize=enum
#L'ajout de tous ces flags n'a causé aucun problème

TARGETS = test_aes test_ecb test_cbc chiffrement_tux_ecb chiffrement_tux_cbc

all: $(TARGETS)

test_aes: AES.o test_aes.o
	$(CC) $(CFLAGS) -o test_aes AES.o test_aes.o

test_ecb: AES.o ECB.o test_ecb.o
	$(CC) $(CFLAGS) -o test_ecb AES.o ECB.o test_ecb.o

test_cbc: AES.o CBC.o test_cbc.o
	$(CC) $(CFLAGS) -o test_cbc AES.o CBC.o test_cbc.o

chiffrement_tux_ecb: AES.o ECB.o bmp.o chiffrement_tux_ecb.o
	$(CC) $(CFLAGS) -o chiffrement_tux_ecb AES.o ECB.o bmp.o chiffrement_tux_ecb.o

chiffrement_tux_cbc: AES.o CBC.o bmp.o chiffrement_tux_cbc.o
	$(CC) $(CFLAGS) -o chiffrement_tux_cbc AES.o CBC.o bmp.o chiffrement_tux_cbc.o

AES.o: AES.c AES.h
	$(CC) $(CFLAGS) -c AES.c

ECB.o: ECB.c ECB.h AES.h
	$(CC) $(CFLAGS) -c ECB.c

CBC.o: CBC.c CBC.h AES.h
	$(CC) $(CFLAGS) -c CBC.c

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

test_aes.o: test_aes.c AES.h
	$(CC) $(CFLAGS) -c test_aes.c

test_ecb.o: test_ecb.c ECB.h AES.h
	$(CC) $(CFLAGS) -c test_ecb.c

test_cbc.o: test_cbc.c CBC.h AES.h
	$(CC) $(CFLAGS) -c test_cbc.c

chiffrement_tux_ecb.o: chiffrement_tux_ecb.c ECB.h bmp.h
	$(CC) $(CFLAGS) -c chiffrement_tux_ecb.c

chiffrement_tux_cbc.o: chiffrement_tux_cbc.c CBC.h bmp.h
	$(CC) $(CFLAGS) -c chiffrement_tux_cbc.c

clean:
	rm -f *.o $(TARGETS)