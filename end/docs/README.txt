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

The program takes in 2 port numbers as parameters. Sockets are then created and added to a set where we listen to incoming connections
on either port, first parameter creates a socket for server connections and second parameter for client connections.
When some action is registered on the sockets we are monitoring we check if it is an incoming connection, if so we establish connection and
add it to our set of sockets we are monitoring.
We then listen for the incoming SERVERS message to register the group info (name, ip, port etc.).
In order to let other servers know that we are not idle, we send a keep alive message every 60 - 90 seconds.
If the action that is registered on the sockets is a message. We read the buffer and act according to the instructions given in the project
description.



Authors:
Aegir Tomasson <aegir15@ru.is>
Dagur Kristjansson <dagur17@ru.is>


Teacher:
Jacky Mallett <jacky@ru.is>)


TA:
Jon Petur Jonsson <jonpj16@ru.is>


Total time spent on project: 80 hours.


Disclaimer

We discussed the project with a lot of TA's and fellow students. We basically discussed the project with everyone and their grandmother,
but all of the code is our own (as much as anyone can own code).
