#!/bin/bash

# valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes ./x.out
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./x.out