CC = cc
CCFLAGS = -Wall -g -std=c++11 -stdlib=libc++
LDFLAGS = -lstdc++
EXECUTABLE = ./a.out

$(EXECUTABLE): main.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc
	$(CC) $(CCFLAGS) -c main.cc
