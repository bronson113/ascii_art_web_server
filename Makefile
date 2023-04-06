IDIR=inc
CC=gcc
CFLAGS=-I$(IDIR) -g -O0

ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm

_DEPS = server.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: server

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

server: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^  $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 