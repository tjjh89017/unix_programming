PROG = libhijack.so

CC = gcc
LD = gcc

CFLAGS = -fPIC
LDFLAGS = -shared
LIBS = -ldl

OBJS = hijack.o

.PHONY: all clean test

.c.o:
	$(CC) $(CFLAGS) -c $? -o $@

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS)

clean:
	rm -rf $(PROG) $(OBJS)

test: $(PROG)
	$(shell LD_PRELOAD=./$(PROG) wget http://rubygems.nos.ncu.edu.tw -O ./tmp.output)
	rm -rf ./tmp.output

testSSL: $(PROG)
	$(shell LD_PRELOAD=./$(PROG) wget https://rubygems.nos.ncu.edu.tw -O ./tmp.output)
	rm -rf ./tmp.output
