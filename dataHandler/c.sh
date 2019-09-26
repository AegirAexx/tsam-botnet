#!/bin/bash

g++ -O2 -g -Wall -std=gnu++17 $1 -o x.out && echo "Success!!" | figlet | lolcat
