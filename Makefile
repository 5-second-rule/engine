CC=g++
CFLAGS=-c -Wall -std=c++11 -ICommsProcessor -IServer -ISampleClient
LDFLAGS=-Lunix -lCommsProcessor
SERVERSRC=Server/Server.cpp
CLIENTSRC=SampleClient/SampleClient.cpp
LIBSRC=CommsProcessor/CommsProcessor.cpp CommsProcessor/Socket.cpp
SERVEROBJ=$(SERVERSRC:.cpp=.o)
CLIENTOBJ=$(CLIENTSRC:.cpp=.o)
LIBOBJ=$(LIBSRC:.cpp=.o)
SERVEREXE=unix/server
CLIENTEXE=unix/sampleClient
LIBRARY=unix/libCommsProcessor.so

all: $(SERVERSRC) $(CLIENTSRC) $(LIBSRC) $(SERVEREXE) $(CLIENTEXE) $(LIBRARY)

$(SERVEREXE): $(SERVEROBJ) $(LIBRARY)
		mkdir -p unix
		$(CC) $(LDFLAGS) $(SERVEROBJ) -o $@

$(CLIENTEXE): $(CLIENTOBJ) $(LIBRARY)
		mkdir -p unix
		$(CC) $(LDFLAGS) $(CLIENTOBJ) -o $@

$(LIBRARY): $(LIBOBJ)
		mkdir -p unix
		$(CC) $(LIBOBJ) -shared -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(SERVEROBJ) $(CLIENTOBJ) $(LIBOBJ) unix
