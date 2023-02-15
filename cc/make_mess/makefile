CC = cc
CCFLAGS = -Wall -g -std=c++2a -stdlib=libc++
LDFLAGS = -lstdc++
EXECUTABLE = ~fritx/bin/main

$(EXECUTABLE): main.o gumpert.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o gumpert.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc gumpert.h
	$(CC) $(CCFLAGS) -c main.cc

gumpert.o: gumpert.h
	$(CC) $(CCFLAGS) -c gumpert.cc
