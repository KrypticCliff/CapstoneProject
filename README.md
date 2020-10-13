# Udacity Socket Connection Bridge Capstone Project

This is a personal project repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The Capstone Project focuses on creating a Socket connection between 2 different devices and relays messages to the server.

![ServerConnection](https://github.com/KrypticCliff/CapstoneProject/blob/master/images/Server.png)
![ClientConnection](https://github.com/KrypticCliff/CapstoneProject/blob/master/images/Client.png)

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
2. For establishing the Server, compile: `g++ server.cpp src/SocketBuild.cpp -o server` on one device.
3. For connecting the Client, compile: `g++ client.cpp src/SocketBuild.cpp  -o client` on the other device.
4. Run the Server first using `./server`
5. Grab the IP of device running Server. Can use `ifconfig` and locate wired or ethernet connected IP.
5. Run Client last with IP and Port number (Set to 8333)`./client 10.0.0.1 8333`
6. Send Strings to the server from the client session.

## Tasks Done
- At least two variables are defined as references, or two functions use pass-by-reference in the project code.
In SocketBuild.cpp, the functions on Line 12, 45, and 86 provide examples of pointers (struct of addrinfo) being passed by references. The information within the referenced addrinfos are later modified and used to construct the socket descriptor.

https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp#L12
https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp#L45
https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp#L86

- The project accepts input from a user as part of the necessary operation of the program.
In Client.cpp, line 48 provides prood of accepting a user's input which is formatted to be placed in stdin. The data is sent to the output of the Server console and logged into a file named ClientLog.txt

https://github.com/KrypticCliff/CapstoneProject/blob/master/client.cpp#L48

- The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
In Server.cpp on line 28, the implementation of writing to a file is initiated and on line 89 & 93, the data is appended to the create or existing ClientLog.txt file. Using the Select() function starting from Line 61-70 helps read any incoming data from the client. The buffer out may be skipped due to the rate the timeout check by rapidly throwing data into the Client. This may overlap the output data that the ClientLog.txt should receive.  

https://github.com/KrypticCliff/CapstoneProject/blob/master/server.cpp#L28
https://github.com/KrypticCliff/CapstoneProject/blob/master/server.cpp#L89
https://github.com/KrypticCliff/CapstoneProject/blob/master/server.cpp#L89

- The project code is clearly organized into functions.
The functions on building, connecting a socket file descriptor, and Sending messages have been allocated to src/SocketBuild.cpp & include/SocketBuild.h. This helps clear up the Client.cpp & Server.cpp files from having a wall of multiple if-statements and function checks.

https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp
https://github.com/KrypticCliff/CapstoneProject/blob/master/include/SocketBuild.h

- One function is overloaded with different signatures for the same function name.
In SocketBuild.cpp on line 12 & 45, CreateSocket() has been overloaded to match the build between the client and server. One CreateSocket catches the "node" or "IP Address" which the client will need to indicate where to connect to. The other CreateSocket removes the "node" capability because it is automatically filled by the OS through Line 19. It also includes binding the address information to a socket file descriptor to make sure the server is found on the local network.

https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp#L12
https://github.com/KrypticCliff/CapstoneProject/blob/master/src/SocketBuild.cpp#L45
