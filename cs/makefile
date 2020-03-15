CC = mcs
CCFLAGS = -Wall -g -std=c++11
LDFLAGS = -lstdc++
EXECUTABLE = /home/fritx/bin/main

$(EXECUTABLE): main.o gumpert.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o gumpert.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc gumpert.h
	$(CC) $(CCFLAGS) -c main.cc

gumpert.o: gumpert.h
	$(CC) $(CCFLAGS) -c gumpert.cc
