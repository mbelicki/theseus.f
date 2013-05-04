CC=gcc

COMMON_OPTS=-lm -lSDL -lSDL_image -Wall -pedantic -Iinc -std=c99 -O3 -g
WINDOWS_OPTS=-mwindows

all: assets.o draw.o logic.o map.o entity.o src/theseus.c
	$(CC) src/theseus.c assets.o draw.o logic.o entity.o map.o \
			-o theseus $(COMMON_OPTS)

windows: assets.o draw.o logic.o map.o entity.o src/theseus.c
	$(CC) src/theseus.c assets.o draw.o logic.o entity.o map.o \
			-o theseus $(COMMON_OPTS) $(WINDOWS_OPTS)

draw.o: src/draw.c
	$(CC) src/draw.c -o draw.o -c $(COMMON_OPTS)

assets.o: src/assets.c
	$(CC) src/assets.c -o assets.o -c $(COMMON_OPTS)

logic.o: src/logic.c
	$(CC) src/logic.c -o logic.o -c $(COMMON_OPTS)

entity.o: src/entity.c
	$(CC) src/entity.c -o entity.o -c $(COMMON_OPTS)


map.o: src/map.c map.levels.o map.generator.o
	$(CC) src/map.c -o _map.o -c $(COMMON_OPTS)
	ld -r -o map.o _map.o map.levels.o map.generator.o

map.levels.o: src/map/levels.c
	$(CC) src/map/levels.c -o map.levels.o -c $(COMMON_OPTS)

map.generator.o: src/map/generator.c
	$(CC) src/map/generator.c -o map.generator.o -c $(COMMON_OPTS)


clean:
	rm -f *.o theseus
