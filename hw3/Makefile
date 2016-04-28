PROG = yosh
CC = gcc
LD = gcc
CFLAGS = -D__DEBUG__
LDFLAGS = 
LIBS = 

OBJS = main.o

.PHONY: all test clean

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

clean:
	rm -rf $(PROG) $(OBJS)
