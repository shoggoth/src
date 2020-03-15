CC = cc
CCFLAGS = -Wall -g -std=c++14 -I /usr/local/include
LDFLAGS = -lstdc++ -lthr -lboost_system -L/usr/local/lib
EXECUTABLE = /home/fritx/bin/main

$(EXECUTABLE): main.o boost_test.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o boost_test.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc boost_test.h
	$(CC) $(CCFLAGS) -c main.cc

boost_test.o: boost_test.h
	$(CC) $(CCFLAGS) -c boost_test.cc
