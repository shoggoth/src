CC = cc
CCFLAGS = -Wall -g -std=c++11
LDFLAGS = -lstdc++
EXECUTABLE = /home/fritx/bin/main

$(EXECUTABLE): main.o lambda.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o lambda.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc lambda.h
	$(CC) $(CCFLAGS) -c main.cc

lambda.o: lambda.h
	$(CC) $(CCFLAGS) -c lambda.cc
