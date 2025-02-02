default: game

game: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c)
	gcc main.c gen/*.c dsa/*.c -I gen -I dsa -o game

clean:
	rm -f game
