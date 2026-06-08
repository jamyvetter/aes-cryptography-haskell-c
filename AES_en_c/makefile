CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = test_aes
OBJS = AES.o test_aes.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

AES.o: AES.c AES.h
	$(CC) $(CFLAGS) -c AES.c

test_aes.o: test_aes.c AES.h
	$(CC) $(CFLAGS) -c test_aes.c

clean:
	rm -f $(OBJS) $(TARGET)