SOURCES=lowlevel.c usedb.c del.c get.c put.c inout.c debug.c cash.c
HEADERS=libdb.h

OBJECTS=$(SOURCES:.c=.o)

all: libdb.so

libdb.so: $(OBJECTS) $(HEADERS)
	gcc -g -O0 -shared $(OBJECTS) -o $@

lowlevel.o: lowlevel.c $(HEADERS)
	gcc -g -fPIC -c lowlevel.c

usedb.o: usedb.c $(HEADERS)
	gcc -g -fPIC -c usedb.c

del.o: del.c $(HEADERS)
	gcc -g -fPIC -c del.c

get.o: get.c $(HEADERS)
	gcc -g -fPIC -c get.c

put.o: put.c $(HEADERS)
	gcc -g -fPIC -c put.c

inout.o: inout.c $(HEADERS)
	gcc -g -fPIC -c inout.c

debug.o: debug.c $(HEADERS)
	gcc -g -fPIC -c debug.c

cash.o: cash.c $(HEADERS)
	gcc -g -fPIC -c cash.c


prog: main

main: main.o $(OBJECTS) $(HEADERS)
	gcc -g -O0 $(OBJECTS) main.o -o main

main.o: main.c $(HEADERS)
	gcc -g -c main.c

clean:
	rm -rf main libdb.so *.o *~
