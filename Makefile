IDIR = ../include
CC=gcc
CFLAGS = -Wall -std=c11 -pedantic -Wvla -O0 -I$(IDIR)
ODIR = obj
LDIR = ../lib

LIBS = -lm

_DEPS = hw1given.h
DEPS = $(patsubset %,$(IDIR)/%,$(_DEPS))

_OBJ = hw1.o
OBJ = $(patsubset %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hw1: hw1.o 
	$(CC) -o hw1 hw1.o

.PHONY: clean

clean:
	rm -f $(obj) hw1 hw1.o
