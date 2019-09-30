
# Variables
CC=g++
FLAGS=-O2 -g -Wall -std=gnu++17
NERD=&& echo "SUCCESS!!" | toilet --gay
# Verbosity
V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "\nCompiling ...\n"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))


#Default
all:
	$(CC) $(FLAGS) main.cpp src/*/*.cpp -o x.out $(NERD)

#Uninstall
clean:
		rm -v x.out

# Wipe local memory, ALL DATA WILL BE REMOVED!
memory:
		rm -v data/*
