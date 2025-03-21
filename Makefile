default: game

game: main.c $(wildcard src/*.c) $(wildcard src/*/*.c)
	gcc main.c src/*.c src/*/*.c -Iinclude -Llib -o out

debug: main.c $(wildcard src/*.c) $(wildcard src/*/*.c)
	gcc main.c src/*.c src/*/*.c -Iinclude -Llib -g -o debug

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.vscode ./copy/.gitignore ./copy/.git ./copy/out ./copy/debug.dSYM ./copy/.DS_Store
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -f *.o out debug *.tar.gz log.txt 
	rm -rf ./young_alex.assignment* ./debug.dSYM
