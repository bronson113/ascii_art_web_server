IDIR=inc
CC=gcc
CFLAGS=-I$(IDIR) -g -O0 -Wall

ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm -lcurl -L$(LDIR)

_DEPS = main.h server.h worker.h request_queue.h curl.h stb_image.h easy.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o server.o worker.o request_queue.o protest.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

main: $(OBJ)
	$(CC) $(CFLAGS) -o ascii_server $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
