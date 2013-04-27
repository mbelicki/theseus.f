EMCC=~/sources/emscripten/emcc

COMMON_OPTS=-lSDL -lSDL_image -Wall -pedantic -Iinc -std=c99 -g -O3
STATIC_LIB=-static-libgcc

all: create.o draw.o levels.o generator.o src/theseus.c
	gcc src/theseus.c create.o levels.o draw.o generator.o \
			-o theseus $(COMMON_OPTS)

draw.o: src/draw.c
	gcc src/draw.c -o draw.o -c $(COMMON_OPTS)

create.o: src/create.c
	gcc src/create.c -o create.o -c $(COMMON_OPTS)

levels.o: src/levels.c
	gcc src/levels.c -o levels.o -c $(COMMON_OPTS)

generator.o: src/generator.c
	gcc src/generator.c -o generator.o -c $(COMMON_OPTS)

clean:
	rm -f *.o theseus

javascript:
	$(EMCC) src/*.c -Iinc -o theseus.html
