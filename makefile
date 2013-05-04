CC=gcc

COMMON_OPTS=-lm -lSDL -lSDL_image -Wall -pedantic -Iinc -std=c99 -O3
WINDOWS_OPTS=-mwindows

all: create.o draw.o levels.o generator.o logic.o map.o entity.o src/theseus.c
	$(CC) src/theseus.c create.o levels.o draw.o logic.o generator.o \
			entity.o map.o \
			-o theseus $(COMMON_OPTS)

windows: create.o draw.o levels.o generator.o logic.o map.o entity.o src/theseus.c
	$(CC) src/theseus.c create.o levels.o draw.o logic.o generator.o \
			-o theseus $(COMMON_OPTS) $(WINDOWS_OPTS)

draw.o: src/draw.c
	$(CC) src/draw.c -o draw.o -c $(COMMON_OPTS)

create.o: src/create.c
	$(CC) src/create.c -o create.o -c $(COMMON_OPTS)

levels.o: src/levels.c
	$(CC) src/levels.c -o levels.o -c $(COMMON_OPTS)

generator.o: src/generator.c
	$(CC) src/generator.c -o generator.o -c $(COMMON_OPTS)

logic.o: src/logic.c
	$(CC) src/logic.c -o logic.o -c $(COMMON_OPTS)

map.o: src/map.c
	$(CC) src/map.c -o map.o -c $(COMMON_OPTS)

entity.o: src/entity.c
	$(CC) src/entity.c -o entity.o -c $(COMMON_OPTS)

clean:
	rm -f *.o theseus
