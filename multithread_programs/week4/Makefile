all: shell

shell: shell.o
	g++ -o bin/shell shell.o
	rm -f shell.o

shell.o: shell.cpp
	g++ -std=c++11 -c shell.cpp
