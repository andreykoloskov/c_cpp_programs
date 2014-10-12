all:
	gcc -g -O0 libdb.c -shared -fPIC -o libdb.so
main:
	gcc -g -O0 libdb.c main.c -o main
clean:
	rm -f test libdb.so a.out
