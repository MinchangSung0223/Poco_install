all:server client
LDLIBS= -I/usr/local/include -L/usr/local/lib
LDFLAGS= -lPocoFoundation -lPocoUtil -lPocoNet

CC=g++
CFLAGS=-g -Wall

server: TCPServer.cpp
	$(CC) $(LDLIBS) -o $@ $< $(LDFLAGS)
client: TCPClient.cpp
	$(CC) $(LDLIBS) -o $@ $< $(LDFLAGS)
clean:
	rm server client
