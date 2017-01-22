CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -s
CFILES = $(wildcard *.cpp)
HFILES = $(wildcard *.h)
OBJECTS = $(CFILES:.cpp=.o)
TARGET = allocator

all : $(TARGET)
allocator : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) -MM $(CFILES) > deps.make

clean:
	rm -f $(TARGET) *.o *~
