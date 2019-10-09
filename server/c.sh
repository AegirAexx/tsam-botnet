#!/bin/bash

g++ -O2 -g -Wall -std=gnu++17 *.cpp -o server.out && echo "Server!!" | toilet --gay
