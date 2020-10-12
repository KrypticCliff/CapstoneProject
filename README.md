# Udacity Socket Connection Bridge Capstone Project

This is a personal project repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The Capstone Project focuses on creating a Socket connection between 2 different devices and relays messages to the server.

## Further Background of Socket Creation
"Beej's Guide to Network Programming" provides more insight on how each function play a role in establishing socket connections/
https://beej.us/guide/bgnet/html/


## Dependencies for Running Locally
* Only optimized for UNIX devices. (Uses UNIX socket files. i.e. socket.h )
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)

## Basic Build Instructions

1. Clone this repo on 2 different devices (Ex. Linux/Mac Desktop & Raspberry Pi).
2. For establishing the Server, compile: `g++ server.cpp -o server` on one device.
3. For connecting the Client, compile: `g++ client.cpp -o client` on the other device.
4. Run the Server first using `./server`
5. Grab the IP of Server running device. Can use `ifconfig` and locate wired or ethernet connected IP
5. Run Client last with IP and Port number (Set to 8333)`./client 10.0.0.1 8333`
6. Send Strings to the server from the client session.
