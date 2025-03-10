default: game

game: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c) $(wildcard saves/*.c) $(wildcard characters/*.c) $(wildcard game_loop/*.c) # compile source files to out
	gcc main.c gen/*.c dsa/*.c saves/*.c characters/*.c game_loop/*.c -Igen -Idsa -Isaves -Icharacters -Igame_loop -Itypes -o out

debug: main.c $(wildcard gen/*.c) $(wildcard dsa/*.c) $(wildcard saves/*.c) $(wildcard characters/*.c) $(wildcard game_loop/*.c) # compile source files to out
	gcc main.c gen/*.c dsa/*.c saves/*.c characters/*.c game_loop/*.c -Igen -Idsa -Isaves -Icharacters -Igame_loop -Itypes -g -o debug

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.vscode ./copy/.gitignore ./copy/.git ./copy/out ./copy/debug.dSYM ./copy/.DS_Store
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -f *.o out debug *.tar.gz log.txt 
	rm -rf ./young_alex.assignment* ./debug.dSYM
