CC=g++
CFLAGS=-Werror

all: LRU

LRU:
	$(CC) $(CFLAGS) main.cpp -o LRU.out

clean:
	rm -rf *.out
