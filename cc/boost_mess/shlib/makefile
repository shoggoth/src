EXECUTABLE = xc
DYLIB = xc.dylib
CXX = cc
CXXFLAGS = -Wall -g -std=c++14 -I /usr/local/include -fPIC
LDFLAGS = -lstdc++ -lthr -lboost_system -L/usr/local/lib/boost
LDFLAGS = -lc++ -lboost_system -lboost_thread -L /usr/lib -L/usr/local/lib
SLFLAGS = -shared 

$(EXECUTABLE): main.o $(DYLIB)
	$(LD) $(LDFLAGS) -o $(EXECUTABLE) main.o $(DYLIB)

$(DYLIB): lib_test.o
	$(LD) $(SLFLAGS) -o $(DYLIB) lib_test.o

clean:
	rm -f $(EXECUTABLE) $(DYLIB) *.o
