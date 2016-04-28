PROG = yosh
CC = gcc
LD = gcc
CFLAGS = 
LDFLAGS = 
LIBS = 

OBJS = 

.PHONY: all test clean

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

clean:
	rm -rf $(PROG) $(OBJS)
