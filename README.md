# TSAM - The Botnet Rises


## Folder structure
```BASH
.
├── client
│   ├── client.cpp
│   └── c.sh
├── commands.txt
├── legacy_source
│   ├── Makefile
│   ├── project1
│   │   ├── client.cpp
│   │   └── server.cpp
│   ├── prototype
│   │   ├── Client.cpp
│   │   ├── Client.h
│   │   ├── Command.cpp
│   │   ├── Command.h
│   │   ├── ReceivedMessage.cpp
│   │   ├── ReceivedMessageData.cpp
│   │   ├── ReceivedMessageData.h
│   │   ├── ReceivedMessage.h
│   │   ├── SentMessage.cpp
│   │   ├── SentMessageData.cpp
│   │   ├── SentMessageData.h
│   │   ├── SentMessage.h
│   │   ├── ServerProfile.cpp
│   │   ├── ServerProfileData.cpp
│   │   ├── ServerProfileData.h
│   │   ├── ServerProfile.h
│   │   ├── Utilities.cpp
│   │   └── Utilities.h
│   ├── ReceivedMessage
│   │   ├── main.cpp
│   │   ├── ReceivedMessage.cpp
│   │   └── ReceivedMessage.h
│   ├── SentMessage
│   │   ├── main.cpp
│   │   ├── SentMessage.cpp
│   │   └── SentMessage.h
│   ├── ServerProfile
│   │   ├── main.cpp
│   │   ├── ServerProfile.cpp
│   │   └── ServerProfile.h
│   ├── teacher_code
│   │   ├── client.cpp
│   │   ├── ip.cpp
│   │   ├── ip.out
│   │   ├── readme.txt
│   │   └── server.cpp
│   └── val.sh
├── LICENSE.md
├── README.md
├── server
│   ├── Command.cpp
│   ├── Command.h
│   ├── c.sh
│   ├── Message.cpp
│   ├── Message.h
│   ├── server.cpp
│   ├── server.out
│   ├── skel.sh
│   ├── Utilities.cpp
│   └── Utilities.h
└── validation
    ├── Command.cpp
    ├── Command.h
    ├── c.sh
    ├── logs
    │   └── MessageLog.txt
    ├── Message.cpp
    ├── Message.h
    ├── scratch_pad.cpp
    ├── Server.cpp
    ├── Server.h
    ├── Utilities.cpp
    ├── Utilities.h
    └── x.out


```

## Usage
Compile:

> `make`

Remove:

> `make clean`

Wipe local persistent memory:

> `make memory`

## V_Group_4
Ægir Tómasson (aegir15)<br>
Dagur Kristjánsson (dagur17)

# TODO README !!!

# The handout PDF

## Assignment
1. Implement client and server as described above. All local commands to the server must be implemented by a separate client over the network. __*(4 points)*__
2. Provide a __wireshark__ trace of communication between your client and server for all commands implemented __*(2 points)*__
3. Have been successfully connected to by an Instructor’s server. __*(1 point)*__
4. Successfully receive messages from at least 10 other groups (Provide timestamped log) __*(1 point)*__
5. Successfully send messages to at least 10 other groups (Provide timestamped log) __*(1 point)*__
6. Code is submitted as a single tar file, with README and Makefile. (Do not include hg/git/etc. repositories.) __*(1 point)*__


## Bonus Points

Note: The maximum grade (including bonus points) for the assignment is __*15 points*__.


- Obtain bonus points for connectivity __*(maximum 3 points)*__:
  - __*0.5 points*__ for messages from servers from students in Akureyri (The Akureyi group whose messages are reported will receive a matching __*0.5 points*__, if they are reported at least 2 times by non-Akureyri groups.)
  - __*0.5 points*__ per 10 different groups you can show messages received from.
- Provide a time stamped plaintext communication from the Oracle. (Time stamp should be when you received it. The oracle will respond to any messages addressed to it and delivered. It may move.)
- Decode a hashed message from the oracle __*(1 point)*__

## Programming Assignment
In this project you will write a simple store and forward botnet message server, with accompanying Command and Control (C&C) client, following the specification below.

The goal is to link your server and client into a class wide botnet, and use them to fulfill the assignment todo list. In order to do that, you will need to give some thought to routing commands through the network, connecting to and leaving the botnet, storing messages for disconnected servers, message expiry, and handling commands for other groups. There is also a mysterious Oracle to be found.

In particular, remember that there is other information besides that purely in the messages for example, knowing which link to your server the message arrived on.


## Rules of Engagement:
0. Don’t crash the (bot) network.
1. Try not to crash the campus network either.
2. Messages should not be longer than 5000 characters. You may truncate any messages you receive longer than that.
3. "Be strict in what you send, be tolerant in what you receive".
4. The executable for your server should be named as `tsam<Group_ID>` eg. `tsamvgroup1`, the first parameter should be the port it is accepting connections on.
5. Your server should identify itself within the Botnet using your group ID, eg. V Group 1 (Note, Instructor servers will also be running, prefixed by I )
6. While part of the botnet, your server must maintain connections with at least 2 other servers and no more than 5 servers.
7. TCP ports 4000-4100 are available on skel.ru.is for external connections.
8. Your clients must connect to your server only, and issue any commands to other servers on the botnet via your server
9. You must use two token characters to indicate the stand and end of each message between servers: ASCII character, 01 (SOH) for start, and 04(EOT) for end of message, with bitstuffing as necessary.
10. You are encouraged to reach out to other groups to be on the botnet with them at the same time, and co-operate in getting things working.

## Server Specification
You do not have to run your server on skel.ru.is, as long as it can connect to at least one server that is running on skel.ru.is. If you are having problems connecting to skel, contact an instructor in the first week.

Your server should listen on one TCP port for other servers to connect to it, and continue to listen to a separate port for your clients to connect.

The server port you are listening on should be included in the command line, so that servers running on skel.ru.is can be found. For example:

> `./tsamvgroup1 4044`

where __4044__ is the TCP port for server connections. You should allow a maximum of __5__ servers to connect to you.

---

## Server Communication

Messages between servers should be sent using the following format:
> `<SOH><Command>,<comma-separated-parameters><EOT>`


# Server Commands

The server should support at least the following commands with other servers.

---

> `LISTSERVERS,<FROM_GROUP_ID>`

Reply with servers response (below)

`SERVERS`

Provide a list of directly connected - i.e. 1-hop, servers to this server.

The first IP address in the list should be the IP of the connected server.

Servers should be specified as `GROUP_ID`, the `HOST_IP`, and `PORT` they will accept connections on, comma separated within the message, each server separated by `;`, eg:

> `SERVERS,V_GROUP_1,130.208.243.61,8888;V_GROUP_2,10.2.132.12,8888;`

---

> `KEEPALIVE,<# of Messages>`

Periodic message to 1-hop connected servers, indicating still alive and the no. of messages waiting for the server at the other end. Do not send more than once/minute.

---

> `GET_MSG,<GROUP_ID>`

Get messages for the specified group. This may be for your own group, or another group.

---

> `SEND_MSG,<FROM_GROUP_ID>,<TO_GROUP_ID>,<Message content>`

Send message to another group

---

> `LEAVE,<SERVER_IP>,<PORT>`

Disconnect from server at specified port.

---

> `STATUSREQ,<FROM_GROUP>`

Reply with `STATUSRESP` as below.


> `STATUSRESP,<FROM_GROUP>,<TO_GROUP>,<server, msgs held>,...`

Reply with comma separated list of servers and no. of messages you have for them eg.

> `STATUSRESP,V_GROUP_2,I_1,V_GROUP_4,20,V_GROUP_71,2`


# Client Commands

Communication between the __Client__ and __Server__ should use the protocol below. You may implement additional commands if you wish. The client should timestamp, (day, minute and second, please no nano seconds) all messages sent and received, when they are printed out to the console.

---

 > `GETMSG, GROUP_ID`

Get a single message from the server for the GROUP_ID.

---

> `SENDMSG, GROUP_ID`

Send a message to the server for the GROUP_ID.

---

> `LISTSERVERS`

List servers your server is connected to.
