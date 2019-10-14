 _____ ____    _    __  __   ____            _           _     _____ _
|_   _/ ___|  / \  |  \/  | |  _ \ _ __ ___ (_) ___  ___| |_  |_   _| |__  _ __ ___  ___
  | | \___ \ / _ \ | |\/| | | |_) | '__/ _ \| |/ _ \/ __| __|   | | | '_ \| '__/ _ \/ _ \
  | |  ___) / ___ \| |  | | |  __/| | | (_) | |  __/ (__| |_    | | | | | | | |  __/  __/
  |_| |____/_/   \_\_|  |_| |_|   |_|  \___// |\___|\___|\__|   |_| |_| |_|_|  \___|\___|
                                          |__/

T-409-TSAM-2019-3

Project three is a client server combination. The idea was to make a store and forward
botnet with a command and control client.

Dependencies / Requirements:
- POSIX compliant shell. Like bash or zsh.
- coreutils.
- build-essential.


Install / Uninstall:

The program has two binaries that have to be compiled from source and are written in C/C++.

Compile:

To install the program run:

    make

To remove the program run:

    make clean

To make only one binary, use either:

    make server
        or
    make client


The Server files:

/server
├── Command.cpp
├── Command.h
├── logs
│   └── MessageLog.txt
├── Makefile
├── Message.cpp
├── Message.h
├── README.txt
├── server.cpp
├── Utilities.cpp
└── Utilities.h

The Client file:

/client
└── client.cpp


Usage:

The server binary takes two arguments, port number for other servers and a port number
for the control client. E.g. ./server 4000 4001

The client binary takes two arguments, IPv4 number of the server you wish to connect to
and the port number. E.g. ./client 178.62.12.185 4001


Flow of the program:

BLA BLA BLA


Authors:
Aegir Tomasson <aegir15@ru.is>
Dagur Kristjansson <dagur17@ru.is>


Teacher:
Jacky Mallett <jacky@ru.is>)


TA:
Jon Petur Jonsson <jonpj16@ru.is>


Total time spent on project: 40 hours.


Disclaimer

NEW DISCLAIMER!!!!

We discussed the project with a lot of TA's and fellow students but we spent the most time
with a group of students (Petur Orn Gudmundsson and Throstur Sveinbjornsson). We discussed
the project a lot and some similarities in code could exist.

Also we used the code that TA Einar Orn Sveinbjornsson shared. Our checksum calculating
function and our send function is heavily unfluenced from that code.