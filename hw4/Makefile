PROG = dserv
CC = gcc
LD = gcc
CFLAGS = -I./ \
		 -I/usr/local/Cellar/libevent/2.0.22/include
LDFLAGS = -L/usr/local/Cellar/libevent/2.0.22/lib
LIBS = -levent -lmagic

OBJS = main.o \
	   request.o \
	   file.o

HEADERS = dserv.h 

CFLAGS += -D__DEBUG__ -g

.PHONY: all test clean

.c.o: $(HEADERS)
	$(CC) -c $(CFLAGS) $*.c

all: $(PROG)

$(PROG): $(OBJS) $(HEADERS)
	$(CC) -o $(PROG) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

clean:
	rm -rf $(PROG) $(OBJS)

test: $(PROG)
	./$(PROG) 8000 ./www
