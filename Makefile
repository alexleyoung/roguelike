default: game

game: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c) $(wildcard saves/*.c) $(wildcard characters/*.c) $(wildcard game/*.c) # compile source files to out
	gcc main.c gen/*.c dsa/*.c saves/*.c characters/*.c game/*.c -Igen -Idsa -Isaves -Icharacters -Igame -Itypes -o out

debug: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c) $(wildcard saves/*.c) $(wildcard characters/*.c) $(wildcard game/*.c) # compile source files to out
	gcc main.c gen/*.c dsa/*.c saves/*.c characters/*.c game/*.c -Igen -Idsa -Isaves -Icharacters -Igame -Itypes -g -o debug

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.vscode ./copy/.gitignore ./copy/.git ./copy/game
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -f *.o game *.tar.gz log.txt
	rm -rf ./young_alex.assignment* 
