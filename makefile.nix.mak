CFLAGS=-I/usr/local/include -D_REENTRANT -g -Wall
LIBS=-L/usr/local/lib -Wl,-rpath,/usr/local/lib -lGL -lGLU -lglut -lpthread
CC=gcc

include makefile.vars.mak

default: clean all

PHONY: clean default

clean:
	@echo Cleaning...
	rm -f $(OUTPUT)
	rm -f *~
	@echo Done.

all: $(TARGETS)
	$(CC) $(CFLAGS) $(TARGETS) -o $(OUTPUT) $(LIBS)
