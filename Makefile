default: game

game: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c) $(wildcard saves/*.c) # compile source files to game
	gcc main.c gen/*.c dsa/*.c saves/*.c -I gen -I dsa -I saves -o game

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.vscode ./copy/.gitignore ./copy/.git ./copy/game
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -f *.o game *.tar.gz log.txt
	rm -rf ./young_alex.assignment* 