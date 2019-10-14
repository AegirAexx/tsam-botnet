
#Default
all: server	client

server:	server.cpp
	g++ -O2 -g -Wall -std=gnu++17 server.cpp Message.cpp Command.cpp Utilities.cpp -o server

client:	client.cpp
	g++ -O2 -g -Wall -std=gnu++17 client.cpp -o client

clean:
	rm -v server client
