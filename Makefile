IDIR=inc
CC=gcc
CFLAGS=-I$(IDIR) -g -O0 -Wall

ODIR=obj
LDIR=lib
SDIR=src

LIBS=-lm -lcurl -L$(LDIR)

_DEPS = ascii_conversion.h  curl.h  curlver.h  download_image.h  easy.h  header.h  main.h  mprintf.h  multi.h  options.h  request_queue.h  server.h  stb_image.h  stdcheaders.h  system.h  typecheck-gcc.h  urlapi.h  worker.h urldecode.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o server.o worker.o request_queue.o download_image.o ascii_conversion.o urldecode.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

main: $(OBJ)
	$(CC) $(CFLAGS) -o ascii_server $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
