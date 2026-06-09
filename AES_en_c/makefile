CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = test_aes test_ecb

all: $(TARGETS)

test_aes: AES.o test_aes.o ECB.o
	$(CC) $(CFLAGS) -o test_aes AES.o test_aes.o ECB.o

test_ecb: AES.o test_ecb.o ECB.o
	$(CC) $(CFLAGS) -o test_ecb AES.o test_ecb.o ECB.o

AES.o: AES.c AES.h
	$(CC) $(CFLAGS) -c AES.c

test_aes.o: test_aes.c AES.h
	$(CC) $(CFLAGS) -c test_aes.c

test_ecb.o: test_ecb.c AES.h
	$(CC) $(CFLAGS) -c test_ecb.c

ECB.o: ECB.c AES.h
	$(CC) $(CFLAGS) -c ECB.c

clean:
	rm -f *.o $(TARGETS)