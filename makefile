EMCC=~/sources/emscripten/emcc

COMMON_OPTS=-lSDL -Wall -pedantic -Iinc -std=c99 -g -O3
STATIC_LIB=-static-libgcc

all: create.o draw.o src/theseus.c
	gcc src/theseus.c create.o draw.o -o theseus $(COMMON_OPTS)

draw.o:
	gcc src/draw.c -o draw.o -c $(COMMON_OPTS)

create.o:
	gcc src/create.c -o create.o -c $(COMMON_OPTS)

clean:
	rm *.o theseus

javascript:
	$(EMCC) src/*.c -Iinc -o theseus.html
