default: game

game: main.cpp $(wildcard src/*/*.cpp)
	g++ -std=c++11 main.cpp src/*/*.cpp -Iinclude -Llib -lcurses -o out

debug: main.cpp $(wildcard src/*/*.cpp)
	g++ -std=c++11 main.cpp src/*/*.cpp -Iinclude -Llib -lcurses -g -o debug

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.gitignore ./copy/.git ./copy/.DS_Store ./copy/compile_flags.txt
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -f *.o out debug *.tar.gz log.txt 
	rm -rf ./young_alex.assignment* ./debug.dSYM
