PROG = main

CC = gcc
CFLAGS = -O2 -g

OBJS = main.o \
	   job.o

.PHONY: clean all
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

.c.o:
	$(CC) -c $(CFLAGS) $? -o $@

clean:
	rm -rf $(OBJS) $(PROG)

test: $(PROG)
	./$(PROG) $(S) | awk -f eval.awk
