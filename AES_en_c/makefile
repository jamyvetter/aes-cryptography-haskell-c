CC = gcc
CFLAGS = -Wall -Wextra #-g -fsanitize=address -fsanitize=undefined -fsanitize=shift -fsanitize=shift-base \
-fsanitize=shift-exponent -fsanitize=integer-divide-by-zero -fsanitize=unreachable \
-fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=bounds-strict \
-fsanitize=bounds -fsanitize=bool -fsanitize=signed-integer-overflow \
-fsanitize=enum
#L'ajout de tous ces flags n'a causé aucun problème

TARGETS = test_aes test_ecb test_cbc chiffrement_tux

all: $(TARGETS)

test_aes: AES.o test_aes.o
	$(CC) $(CFLAGS) -o test_aes AES.o test_aes.o

test_ecb: AES.o ECB.o test_ecb.o
	$(CC) $(CFLAGS) -o test_ecb AES.o ECB.o test_ecb.o

test_cbc: AES.o CBC.o test_cbc.o
	$(CC) $(CFLAGS) -o test_cbc AES.o CBC.o test_cbc.o

chiffrement_tux: AES.o ECB.o bmp.o chiffrement_tux.o
	$(CC) $(CFLAGS) -o chiffrement_tux AES.o ECB.o bmp.o chiffrement_tux.o

# --- Compilation (Création des fichiers objets .o) ---

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

chiffrement_tux.o: chiffrement_tux.c ECB.h bmp.h
	$(CC) $(CFLAGS) -c chiffrement_tux.c

# --- Nettoyage ---

clean:
	rm -f *.o $(TARGETS)