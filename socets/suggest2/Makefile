CC = g++
CFLAGS = -Wall -O2 -pthread -std=c++0x
LDFLAGS = -s
CFILES = $(wildcard *.cpp)
HFILES = $(wildcard *.h)
OBJECTS = $(CFILES: .c = .o)
TARGET = suggest

all : $(TARGET)
suggest : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -MM $(CFILES) > deps.make

clean:
	rm -f $(TARGET) *.o *~
