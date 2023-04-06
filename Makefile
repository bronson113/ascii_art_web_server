IDIR=inc
CC=gcc
CFLAGS=-I$(IDIR) -g -O0

ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm

_DEPS = main.h server.h worker.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o server.o worker.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

main: $(OBJ)
	$(CC) $(CFLAGS) -o ascii_server $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 