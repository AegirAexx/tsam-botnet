#!/bin/bash

g++ -O2 -g -Wall -std=gnu++17 *.cpp -o x.out && echo "Success!!" | figlet | lolcat
