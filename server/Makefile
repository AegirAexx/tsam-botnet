
# Variables
CC=g++
FLAGS=-O2 -g -Wall -std=gnu++17
# Verbosity
V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "\nCompiling ...\n"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))


#Default
all:
	server client

#Uninstall
clean:
	rm -v server client

server:
	$(CC) $(FLAGS) server/*.cpp -o server

client:
	$(CC) $(FLAGS) client/client.cpp -o client

