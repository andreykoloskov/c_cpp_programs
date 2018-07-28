CC = g++
CFLAGS = -Wall -O2
LDFLAGS = -s
CFILES = $(wildcard *.cpp)
HFILES = $(wildcard *.h)
OBJECTS = $(CFILES: .c = .o)
TARGET = daemon

all : $(TARGET)
daemon : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) -MM $(CFILES) > deps.make

clean:
	rm -f $(TARGET) *.o *~
