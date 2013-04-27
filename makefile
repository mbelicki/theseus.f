EMCC=~/sources/emscripten/emcc

COMMON_OPTS=-lSDL -Wall -pedantic -Iinc -std=c99 -g -O3
STATIC_LIB=-static-libgcc

all: create.o draw.o levels.o generator.o src/theseus.c
	gcc src/theseus.c create.o levels.o draw.o generator.o \
			-o theseus $(COMMON_OPTS)

draw.o:
	gcc src/draw.c -o draw.o -c $(COMMON_OPTS)

create.o:
	gcc src/create.c -o create.o -c $(COMMON_OPTS)

levels.o:
	gcc src/levels.c -o levels.o -c $(COMMON_OPTS)

generator.o:
	gcc src/generator.c -o generator.o -c $(COMMON_OPTS)

clean:
	rm *.o theseus

javascript:
	$(EMCC) src/*.c -Iinc -o theseus.html
