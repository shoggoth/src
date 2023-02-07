CC = cc
SRC=src
INC=include
BLD=build
CCFLAGS = -Wall -g -std=c++14 -I /usr/local/include -I $(INC)
LDFLAGS = -lstdc++ -lboost_system -lboost_program_options -lpthread -L/usr/local/lib
EXECUTABLE = $(BLD)/ts

$(EXECUTABLE): $(BLD)/main.o $(BLD)/telnet_session.o $(BLD)/telnet_server.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(EXECUTABLE) $(BLD)/main.o $(BLD)/telnet_session.o $(BLD)/telnet_server.o

clean:
	rm -f $(EXECUTABLE) $(BLD)/*.o

$(BLD)/main.o: $(SRC)/main.cc $(INC)/telnet_session.h $(INC)/telnet_server.h
	$(CC) $(CCFLAGS) -c $(SRC)/main.cc -o $(BLD)/main.o

$(BLD)/telnet_session.o: $(INC)/telnet_session.h
	$(CC) $(CCFLAGS) -c $(SRC)/telnet_session.cc -o $(BLD)/telnet_session.o

$(BLD)/telnet_server.o: $(INC)/telnet_server.h $(INC)/telnet_session.h
	$(CC) $(CCFLAGS) -c $(SRC)/telnet_server.cc -o $(BLD)/telnet_server.o
