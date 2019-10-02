#!/bin/bash

g++ -O2 -g -Wall -std=gnu++17 client.cpp -o client.out && echo "Client!!" | toilet --gay
