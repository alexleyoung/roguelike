default: game

game: main.c gen/gen.c
	gcc -o game main.c gen/gen.c

clean:
	rm game