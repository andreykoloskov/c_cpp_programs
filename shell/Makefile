CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -s -std=c++11
CFILES = $(wildcard *.cpp)
HFILES = $(wildcard *.h)
OBJECTS = $(CFILES: .cpp = .o)
TARGET = shell

all : $(TARGET)
shell : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) -MM $(CFILES) > deps.make

clean:
	rm -f $(TARGET) *.o
