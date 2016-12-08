Network Fundamentals Semester Project
Written by: Noah Presser, Christine Moore, Sara Lichtenstein, and Lara Dedic
 
______________________________________________________________________________

*Important Note*
To compile our program ensure you are using the following commands:

g++ -std=c++11 *.cpp -o peerProcess.o 

The g++ compiler must be used with c++11 because we use string functions found in c++11

______________________________________________________________________________

Current State of Our Program
----------------------------

Note on our project:
We were not able to complete the program as specified in the document. I think we were
the only group to use c++ to complete this project and in hindsight Java would have
been the better choice for a project of this nature as C++ yielded many memory issues
that were extremely time consuming to address due to how Linux manages memory.

What our program currently CAN DO:
Our program allows a connection to be set up between a server and a client machine. 
These two machines are able to send handshake messages, bitfield messages, and 
interested and uninterested messages. They are able to send, receive, and interpret 
these kinds of messages without and issue.

We have a system logger in place to be implemented as well, however the issue lies in 
the fact that we couldn't work out a memory issue with string creation in order to create
a file path that we could write the logger to. There seems to be an issue whenever we try
to set a string member of our Peer class. The third string member that attempts to be set
causes a malloc error. 

Our program will also:
Will notify if a host disconnects, when messages are sent and recieved, when a peerID is added
to the list of Peer Ids, and when the system is waiting for another message. 

__________________________________________________________________________________

To Run Our Project
------------------
-First open up two putty windows that are using the Thunder server
-Then using the first command for the first putty window, and the second command for
the second putty window, ssh into the machines.
	-ssh lin114-00.cise.ufl.edu 
	-ssh lin114-01.cise.ufl.edu 
-Once youre in to the machiens run this comand for each machine
	-cd NetworkingProject/BitTorrentButBetter/TestingCise/ 

-Pull the code from our working repository and run it as peer process 1001
git pull origin workingFORREAL && g++ -std=c++11 *.cpp -o peerProcess.o -w && ./peerProcess.o 1001

-run the other peer process on the other machine with:
	./peerProcess.o 1002

-now you can observe our project working as described above


_______________________________________________________________________________________________________

Project Reflections
-------------------

We are aware we could not get our project to work. We all put countless hours into the project and did all we could
to get as far as we did. Our holdup for progress on the project was dealing with memory allocation errors due to 
our choice in c++ for this project. Each memory error took hours to track down and really slowed progress. We have
put a ton of effort, time, and learning into our project and while it's not all the way complete we achieved some
serious milestones and produced a lot of code. A lot of the code you see commented out would work in theory with the method
calls and params we would be using later on down the road. It as hard though, because we spent the majority of our time
dealing with memory errors. 
