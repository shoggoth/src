CC = cc
CCFLAGS = -Wall -g -std=c++14 -I/usr/local/include -I../include
LDFLAGS = -lstdc++ -lthr -lboost_system -lboost_program_options -L/usr/local/lib
EXECUTABLE = ts

$(EXECUTABLE): main.o telnet_session.o telnet_server.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.o telnet_session.o telnet_server.o

clean:
	rm -f $(EXECUTABLE) *.o

main.o: main.cc ../include/telnet_session.h ../include/telnet_server.h
	$(CC) $(CCFLAGS) -c main.cc

telnet_session.o: ../include/telnet_session.h
	$(CC) $(CCFLAGS) -c telnet_session.cc

telnet_server.o: ../include/telnet_server.h
	$(CC) $(CCFLAGS) -c telnet_server.cc
