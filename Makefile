SOURCES=lowlevel.c usedb.c del.c get.c put.c inout.c debug.c

OBJECTS=$(SOURCES:.c=.o)

all: libdb.so

libdb.so: $(OBJECTS) libdb.h
	gcc -g -O0 -shared $(OBJECTS) -o $@

lowlevel.o: lowlevel.c libdb.h
	gcc -g -fPIC -c lowlevel.c

usedb.o: usedb.c libdb.h
	gcc -g -fPIC -c usedb.c

del.o: del.c libdb.h
	gcc -g -fPIC -c del.c

get.o: get.c libdb.h
	gcc -g -fPIC -c get.c

put.o: put.c libdb.h
	gcc -g -fPIC -c put.c

inout.o: inout.c libdb.h
	gcc -g -fPIC -c inout.c

debug.o: debug.c libdb.h
	gcc -g -fPIC -c debug.c


clean:
	rm -rf main libdb.so *.o *~
