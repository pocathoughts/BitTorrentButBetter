#include "stdafx.h"
#include "Peer.h"
#include "Message.h"

Peer::Peer(int _peerID, char * _hostName, int _portNum, bool _fileComplete, std::vector<Peer*> preexistingPeers)
{
	peerID = _peerID;
	hostName = _hostName;
	portNum = _portNum;
	fileComplete = _fileComplete;
	lib = new utilLib();

	//set neighborlist
	lib->setNeighborList(GetAndSetListOfNeighbors());


	// lolz love the name
	optomisticallyUnchokedNeighbor = NULL;
	//std::vector<Peer*> otherPeers = preexistingPeers;
	//probably best to handle the above upon a successful connection
	ReadCfgFile();
	numPieces = ceil((float)fileSize / (float)pieceSize);
	lastPieceSize = fileSize % pieceSize;

	listOfPieces.reserve(numPieces);
	//file name inits
	std::cout << "HOLY FUCK";
	
	//dont do this its fucking evil
		//std::stringstream oss;
		//oss << "peer_";
		//oss << peerID; //WHY DOES THIS CAUSE MALLOC MEMORY ERROR
		//subdir = oss.str();
		//oss << fileName;
		//subDirAndFile = oss.str();
	//end evil

	subdir = "peer_"  + std::to_string(peerID);
	subDirAndFile = "peer_" + std::to_string(peerID) + "/" + fileName;
	//initialize all to false
	if (fileComplete)
	{
		for (int i = 0; i < numPieces; i++)
		{
			listOfPieces.push_back(true);
		}
		//put all 1s for a full file

		const int dir_err = mkdir(subdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == dir_err)
		{
			printf("Error creating directory, already exists");
			remove(subDirAndFile.c_str());
		}

		std::ofstream myfile;
		myfile.open(subDirAndFile.c_str());
		for (int i = 0; i < fileSize; ++i)
		{
			myfile << "1"; //a char is one byte, fill all bytes full for a full file
		}
		myfile.close();
	}
	else
	{
		for (int i = 0; i < numPieces; i++)
		{
			listOfPieces.push_back(false);
		}
		//put all 0s for an empty file
		const int dir_err = mkdir(subdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == dir_err)
		{
			printf("Error creating directory, already exists");
			remove(subDirAndFile.c_str());
		}

		std::ofstream myfile;
		myfile.open(subDirAndFile.c_str());
		for (int i = 0; i < fileSize; ++i)
		{
			myfile << "0"; //a char is one byte, fill all bytes empty for a full file
		}
		myfile.close();
	}
	//see peer one has piece 0 
	//receive a message of 306/8 bytes with each bit representing a piece
		//if (listOfPieces[index] == false && (*i) == true) 
			//get it
			//listOfPieces[index] = true;
	CreateBitfield();

	//comment back in once you can remotely start servers from other machines
	InitializeTCPConnections(preexistingPeers);
}
//for the other peers, won't initialize any connections
Peer::Peer(int _peerID, char * _hostName, int _portNum, bool _fileComplete)
{
	peerID = _peerID;
	hostName = _hostName;
	portNum = _portNum;
	fileComplete = _fileComplete;
	lib = new utilLib();

	//set neighborlist
	lib->setNeighborList(GetAndSetListOfNeighbors());


	// lolz love the name
	optomisticallyUnchokedNeighbor = NULL;
	//std::vector<Peer*> otherPeers = preexistingPeers;
	//probably best to handle the above upon a successful connection
	ReadCfgFile();
	numPieces = ceil((float)fileSize / (float)pieceSize);
	lastPieceSize = fileSize % pieceSize;

	//initialize all to false
	if (fileComplete)
	{
		for (int i = 0; i < numPieces; i++)
		{
			listOfPieces.push_back(true);
		}
	}
	else
	{
		for (int i = 0; i < numPieces; i++)
		{
			listOfPieces.push_back(false);
		}
	}

	//see peer one has piece 0 
	//receive a message of 306/8 bytes with each bit representing a piece
	//if (listOfPieces[index] == false && (*i) == true) 
	//get it
	//listOfPieces[index] = true;
	CreateBitfield();
}
void Peer::InitializeTCPConnections(std::vector<Peer*> preexistingPeers)
{

	//START OF HARD CODING FOR TESTING FIRST CONNECTIONS
	if (peerID == 1002)
	{
		std::cout << "num peers: " << preexistingPeers.size();
		std::cout << "hard code, still need to test multiple users running server and client at same time...\n";
		startClientLinux(preexistingPeers[0]); //
		return;
	}
	//END OF HARD CODING FOR TESTING FIRST CONNECTIONS

	//start listening (TCP Server)
	std::cout << "starting server";
	startServerLinux(); //needs to be implemented and called
	//startServerWinsock();
	if (preexistingPeers.size() == 0)
	{
		return;
	}
	for (std::vector<Peer*>::iterator i = preexistingPeers.begin(); i < preexistingPeers.end(); i++)
	{
		std::cout << "attempting to connect to a client\n";
		startClientLinux((*i)); //
		std::cout << "afterclientConnection in loop\n";
		//startClientWinsock((*i)->hostName, (*i)->peerID);
	}
	//connect to all others (TCP Client)
	//startClient(handshakeMessageString -- for all peers in preexistingPeers
}

int * Peer::GetAndSetListOfNeighbors()
{
	int allNeighborsPlusMyself[6] = { 1001, 1002, 1003, 1004, 1005, 1006 };
	int index = 0;
	for (int i = 0; i < 6; i++)
	{
		if (allNeighborsPlusMyself[i] == peerID)
			continue;
		listOfNeighbors[index] = allNeighborsPlusMyself[i];
		index++;
	}
	return listOfNeighbors;
}

void Peer::CreateBitfield()
{
	//will be size numPieces
	if (fileComplete)
	{
		SetBitfieldToEmpty();
	}
	else
	{
		SetBitfieldToFull();
	}
}
void Peer::ReadCfgFile()
{
	std::ifstream loadedFile("common.cfg");

	std::string line;
	int index = 0;
	if (loadedFile.is_open())
	{
		while (loadedFile.good())
		{
			std::getline(loadedFile, line);


			switch (index)
			{
			case 0:
				SetNumPrefferedNeighbors(line);
				break;
			case 1:
				SetUnchokingInterval(line);
				break;
			case 2:
				SetOptomisticUnchokingInterval(line);
				break;
			case 3:
				SetFileName(line);
				break;
			case 4:
				SetFileSize(line);
				break;
			case 5:
				SetPieceSize(line);
				break;

			default:
				break;
			}
			index++;
		}
	}
	else
	{
		std::cout << "Failed to open common.cfg 404 file not found lol\n";
		return;
	}
	std::cout << "Common.cfg loaded\n";

}
void Peer::SetNumPrefferedNeighbors(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	numPrefferedNeighbors = atoi(message);
}
void Peer::SetUnchokingInterval(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	unchokingInterval = atoi(message);
}
void Peer::SetOptomisticUnchokingInterval(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	optomisticUnchokingInterval = atoi(message);
}
void Peer::SetFileName(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	fileName = message;
}
void Peer::SetFileSize(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	fileSize = atol(message);
}
void Peer::SetPieceSize(std::string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	pieceSize = atol(message);
}

void Peer::SetBitfieldToEmpty()
{
	OURBYTE zeroByte;
	for (int i = 0; i < numPieces; i++)
	{

		bitfield.push_back(zeroByte);
	}
}

void Peer::SetBitfieldToFull()
{
	OURBYTE oneByte;
	lib->setAllBitsTo1(oneByte);
	for (int i = 0; i < numPieces; i++)
	{

		bitfield.push_back(oneByte);
	}
}

void Peer::SetByteInBitfieldToTrue(int byteNum)
{
	//not sure if this works
	lib->setAllBitsTo1(bitfield.at(byteNum));
}
//called in startClient
char * Peer::CreateHandshakeMessage()
{
	// need to create handshake message first
	HandshakeMessage* hsm = new HandshakeMessage(peerID);
	return lib->GetStringFromByteStream(hsm->GetHandshakeMessageByteStream());
}

bool Peer::doesItHaveAnyPieces()
{
	for (std::vector<bool>::iterator i = listOfPieces.begin(); i < listOfPieces.end(); i++)
	{
		if ((*i))
		{
			return true;
		}
	}
	return false;
}


void Peer::DeterminePreferredNeighbors()
{

}

void Peer::DetermineOptomisticallyUnchokedNeighbor()
{

}



Peer::~Peer()
{
	delete optomisticallyUnchokedNeighbor;
	for (std::vector<Peer*>::iterator i = otherPeers.begin(); i < otherPeers.end(); i++)
	{
		delete (*i);
	}
}


// linux servers
//A simple server in the internet domain using TCP
//The port number is passed as an argument

///*

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

void errorS(const char *msg)
{
	perror(msg);
	exit(1);
}
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

//main decision loop for messafe reception
void Peer::HandleMessage(int sockfd, char * message)
{
	
	std::vector<OURBYTE> messageStream = lib->GetByteStreamFromString(message);
	int type = lib->GetMessageTypeFromStream(messageStream);
	std::cout << "received message of type: " << type << std::endl;
	//lib->printByteStream(messageStream);
	if (type == 0)
	{
		//choke
		receiveChokeMessage(messageStream);
	}
	else if (type == 1)
	{
		//unchoke
		receiveUnchokeMessage(messageStream);
	}
	else if (type == 2)
	{
		//interested
		receiveInterestedMessage(messageStream);
	}
	else if (type == 3)
	{
		//not interested
		receiveNotInterestedMessage(messageStream);
	}
	else if (type == 4) {
		//have message
		receiveHaveMessage(messageStream);
	}
	else if (type == 5) {
		//bitfield message
		//will never be called in this function, handled in StartServer and StartClient
		//std::cout << "THIS SHOULD NEVER HAPPEN\n"; //changed my mind it's coo'
	}
	else if (type == 6) {
		//request message
		receiveRequestMessage(messageStream);
	}
	else if (type == 7) {
		//piece message
		receivePieceMessage(messageStream);
	}
	else {
		//invalid type returns and empty vector
		std::cout << "received invalid message type\n";
	}
}
//analyzes the handshake message, closes socket if it's wrong somehow
bool Peer::receiveHandshakeMessage(std::vector<OURBYTE> receivedMessage, int sockfd)
{
	std::cout << "begin receiveHandshakeMessage\n";
	//std::cout << lib->GetStringFromByteStream(receivedMessage) << "\n";
	//Check if header has “P2PFILESHARINGPROJ”
	std::vector<OURBYTE>::iterator first = receivedMessage.begin();
	std::vector<OURBYTE>::iterator last = receivedMessage.begin() + 18; //first 18 bytes
	//std::cout << "A seg fault right after this means the message wasn't sent properly\n";
	//lib->printByteStream(receivedMessage);
	//std::vector<OURBYTE> headerPortion;
	//headerPortion.reserve(18);
	//headerPortion = std::vector<OURBYTE>(first, last); //seg fault on this line
	//std::cout << "what is ashually causing the seg fault\n";

	char * header = lib->GetStringFromByteStream(receivedMessage); //TODO NOAH GET HEADER FROM HS MESSAGE
	header[18] = '\0';
	//std::cout << "received header: " << header << "\n";
	if (strcmp(header, "P2PFILESHARINGPROJ") != 0)
	{
		std::cout << "handshake header not correct, closing connection\n";
		close(sockfd);
		return false;
	}
	else
		std::cout << "handshake header correct!\n";



	//get peerID from end of handshake message
	first = receivedMessage.begin() + 28; //last 4 bytes
	last = receivedMessage.end();
	std::vector<OURBYTE> peerIDPortion(first, last);

	int receivedPeerID = lib->GetIntFromByteStream(peerIDPortion); //TODO NOAH GET PEER ID FROM HS MESSAGE
																   //Check if peerID is in list of peerIDs
	for (int i = 0; i < 5; i++)
	{
		if (listOfNeighbors[i] == receivedPeerID)
		{
			std::cout << "peer ID in list!\n";
			return true;
		}
	}
	std::cout << "received peerID not in list of peerIDs, closing connection\n";
	close(sockfd);
	return false;
}

//sends a message from the server to the client, to be called after receiving a handshake message
void Peer::SendHandshakeMessageFromServer(int sockfd)
{
	std::cout << "returning handshake message from server\n";
	int n = 0;
	HandshakeMessage * h = new HandshakeMessage(peerID); //creates a handshake message using this peerID
	char * message = lib->GetStringFromByteStream(h->GetHandshakeMessageByteStream());

	n = write(sockfd, message, strlen(message)); //sends the handshake message
	if (n < 0)
		error("ERROR writing to socket - SendHandshakeMessageFromServer");
	else
	{
		std::cout << "server sent handshake message back\n";
		//std::cout << message;
	}
}

//sends a message over the connected socket, then awaits a return message
bool Peer::SendHandshakeMessageFromClient(int sockfd)
{

	int n = 0;
	HandshakeMessage * h = new HandshakeMessage(peerID); //creates a handshake message using this peerID
	char * message = lib->GetStringFromByteStream(h->GetHandshakeMessageByteStream());
	n = write(sockfd, message, strlen(message)); //sends the handshake message
	if (n < 0)
		error("ERROR writing to socket - SendHandshakeMessageFromClient");
	else
	{
		std::cout << "client sent handshake message\n";
	}
	//waits for a return handshake message
	bzero(message, 255);
	n = read(sockfd, message, 256);

	if (n < 0)
		error("ERROR writing to socket");
	else
	{
		std::cout << ("client received return handshake message: ");
		std::cout << message << "\n";
	}
	//analyze returned handshake message
	std::vector<OURBYTE> returnMessage = lib->GetByteStreamFromString((char*)message); //MESSAGE
	return receiveHandshakeMessage(returnMessage, sockfd);
}
void Peer::SendClientBitfieldMessage(int sockfd)
{
	int n = 0;
	//std::cout << "here";
	Message * m = new Message(lib->BITFIELD, doesItHaveAnyPieces(), listOfPieces, 0);
	//std::cout << "here1";
	char * message = lib->GetStringFromByteStream(m->GetActualMessageByteStream());
	delete m;
	//std::cout << "here2";
	n = write(sockfd, message, strlen(message)); //sends the bitfield message
	if (n < 0)
		error("ERROR writing to socket - SendClientBitfieldMessage");
	else
	{
		std::cout << "server sent bitfield message to client\n";
	}
	//await returned interested or not interested message
}
void Peer::SendServerBitfieldMessage(int sockfd)
{
	int n = 0;
	std::cout << "here";
	Message * m = new Message(lib->BITFIELD, doesItHaveAnyPieces(), listOfPieces, 0);
	std::cout << "here1";
	char * message = lib->GetStringFromByteStream(m->GetActualMessageByteStream());
	delete m;
	std::cout << "here2";
	n = write(sockfd, message, strlen(message)); //sends the bitfield message
	if (n < 0)
		error("ERROR writing to socket - SendServerBitfieldMessage");
	else
	{
		std::cout << "client sent bitfield message to server\n";
	}

	//await returned "interested" or "not interested" message (loop await general message)
	//called in startClient()
}

void Peer::WaitForClientBitfieldMessage(int sockfd)
{
	std::cout << "Server waiting for client bitfield message\n";
	int n = 0;

	char message[256];
	bzero(message, 255);

	//hacky
	while (strlen(message) < 3)
	{
		n = recv(sockfd, message, 256, 0);
	}

	if (n < 0)
		error("ERROR reading socket");

	//std::cout << "Server waiting for client bitfield message 2 \n";
	std::vector<OURBYTE> returnMessage = lib->GetByteStreamFromString(message); //this is a bitfield message
	//lib->printByteStream(returnMessage);
	//std::cout << "Server waiting for client bitfield message 3\n";
	SendClientBitfieldMessage(sockfd);
	//waits for server's interested or not interested message, saves it for after sending back own message
	//bitfield
	
	std::cout << "awaiting message\n";
	n = 0;
	char message2[256];
	//waits for a return message
	bzero(message2, 255);
	while (strlen(message2) < 3) //shouldnt have to do this, it should block
	{
		n = recv(sockfd, message2, 256, 0);
	}
	HandleMessage(sockfd, message2); //handles the received interested or not interested message
	DetermineInterested(returnMessage, sockfd); //sends either an interested or a not interested message	
	//await returned "interested" or "not interested" message (loop await general message)
	//called in startServer()
}
void Peer::WaitForServerBitfieldMessage(int sockfd)
{
	std::cout << "client waiting for Server bitfield message\n";
	int n = 0;

	char message[256];
	bzero(message, 255);

	//hacky
	while (strlen(message) < 3)
	{
		n = recv(sockfd, message, 256, 0);
	}

	if (n < 0)
		error("ERROR reading socket");

	//std::cout << "client waiting for Server bitfield message 2 \n";
	std::vector<OURBYTE> returnMessage = lib->GetByteStreamFromString(message); //this is a bitfield message
	DetermineInterested(returnMessage, sockfd); //sends either an interested or a not interested message	
}

#define TRUE   1
#define FALSE  0
#define PORT 9001
void Peer::startServerLinux()
{
	/* //start original server
	std::cout << "begin start server\n";
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	//enable blocking mode

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		errorS("ERROR opening socket");

	bzero((char *)&serv_addr, sizeof(serv_addr));

	portno = portNum;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		errorS("ERROR on binding");

	std::cout << "waiting for client connection\n";
	//wait for client
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,
		(struct sockaddr *) &cli_addr,
		&clilen);
	if (newsockfd < 0)
		errorS("ERROR on accept");

	//begin server handshake process
	//receive message
	bzero(buffer, 255);
	n = recv(newsockfd, buffer, 256, 0);
	//after connection, do the following
	receiveHandshakeMessage(lib->GetByteStreamFromString(buffer), newsockfd); //await a handshake message
	SendHandshakeMessageFromServer(newsockfd); //send the handshake message back
	WaitForClientBitfieldMessage(newsockfd); //waits for the client to send a bitfield message and then sends one back
	while (true)
	{
		std::cout << "awaiting message\n";
		int n = 0;
		char message[256];
		//waits for a return message
		bzero(message, 255);
		while (strlen(message) < 3) //shouldnt have to do this, it should block
		{
			n = recv(sockfd, message, 256, 0);
		}
		HandleMessage(newsockfd, message); //this call will receive the interested or not interested method
	}




	//if (n < 0) errorS("ERROR reading from socket");
	//printf("Here is the message: %s\n", buffer);
	//n = write(newsockfd, "I got your message", 18);
	//if (n < 0) errorS("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);
	return;
	*/ // end original server, start multiclient server
	int opt = TRUE;
	int master_socket, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025];  //data buffer of 1K

						//set of socket descriptors
	fd_set readfds;

	//a message
	char *message = "ECHO Daemon v1.0 \r\n";

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections , this is just a good habit, it will work without this
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//bind the socket to localhost port 9001
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	while (TRUE)
	{
		std::cout << "AYYYY MOTHAFUCKERASDFASDGASDFGADSFADSFASF";
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		} 
	

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			//send new connection greeting message
			//begin server handshake process
			//receive message
			bzero(buffer, 255);
			int n = recv(new_socket, buffer, 256, 0);
			//after connection, do the following
			receiveHandshakeMessage(lib->GetByteStreamFromString(buffer), new_socket); //await a handshake message
			SendHandshakeMessageFromServer(new_socket); //send the handshake message back
			WaitForClientBitfieldMessage(new_socket); //waits for the client to send a bitfield message and then sends one back
			//while (true)
			//{
			//	std::cout << "awaiting message\n";
			//	int n = 0;
			//	char message[256];
			//	//waits for a return message
			//	bzero(message, 255);
			//	while (strlen(message) < 3) //shouldnt have to do this, it should block
			//	{
			//		n = recv(sockfd, message, 256, 0);
			//	}
			//	HandleMessage(newsockfd, message); //this call will receive the interested or not interested method
			//}


			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);

					break;
				}
			}
		}
		//else its some IO operation on some other socket :)
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))
			{
				//Check if it was for closing , and also read the incoming message
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close(sd);
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					HandleMessage(client_socket[i], buffer);
					//set the string terminating NULL byte on the end of the data read
					//buffer[valread] = '\0';
					//send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
}


void Peer::startClientLinux(Peer * otherPeer)
{
	char * hostName = otherPeer->hostName;
	hostName[strlen(hostName) - 1] = '\0'; //hacky space handling
	int otherPeerID = otherPeer->peerID;
	std::cout << "beginning start client\n";
	std::cout << hostName << " " << otherPeerID;
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	portno = portNum; ///define the portnumber


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(hostName);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	//begin establish handshake from server
	std::cout << "about to send handshake message from client\n";
	otherPeer->establishedSockfd = sockfd;
	//send the handshake message to the server
	if (!SendHandshakeMessageFromClient(otherPeer->establishedSockfd))
	{
		std::cout << "falsity\n";
		return;
	}
	//std::cout << "truth\n";
	//send bitfield message
	SendServerBitfieldMessage(otherPeer->establishedSockfd); //sends the server a bitfield message
	WaitForServerBitfieldMessage(otherPeer->establishedSockfd);
	//then waits to receive one
	char message[256];
	bzero(message, 255);
	recv(otherPeer->establishedSockfd, message, 256, 0);
	std::vector<OURBYTE> returnMessage = lib->GetByteStreamFromString(message);
	//DetermineInterested(returnMessage, otherPeer->establishedSockfd); //send back interested message
	
	while (true)
	{
		std::cout << "awaiting message\n";
		int n = 0;
		char message[256];
		//waits for a return message
		bzero(message, 255);
		//while (strlen(message) < 3) //shouldnt have to do this, it should block
		//{
			n = recv(sockfd, message, 256, 0);
		//}
		HandleMessage(sockfd, message); //loops forever basically
	}

	//printf("Please enter the message: ");
	//bzero(buffer, 255);
	//fgets(buffer, 256, stdin);
	//n = write(sockfd, buffer, strlen(buffer));
	//if (n < 0)
	//	error("ERROR writing to socket");

	//bzero(buffer, 255);
	//n = read(sockfd, buffer, 256);
	//if (n < 0)
	//	error("ERROR reading from socket");
	//printf("%s\n", buffer);


	close(sockfd);
	return;
}
//*/

//each of the below must send back a message through the connected stream

//only one that is not called in AwaitMessageAndLoop()
void Peer::DetermineInterested(std::vector<OURBYTE> messageStream, int sockfd)
{
	//we're dealing with a bitfield message, we need to access the payload, which is from bytes 5 to the end()
	int counter = 0;
	for (std::vector<OURBYTE>::iterator i = messageStream.begin() + 5; i < messageStream.end(); i++)
	{
		//for each bit
		for (int j = 0; j < 8; j++) {
			//setting the value of the bits of the currentByte with
			//the value of listOfPieces
			if (listOfPieces[counter] == false) //if this peer does not have the piece
			{
				if ((*i)[j] == true)
				{
					//we are interested, there is a piece we don't have
					std::cout << "interested\n";
					SendInterestedMessage(sockfd);
					return;
				}
			}
			counter++;
		}
	}
	std::cout << "not interested\n";
	SendNotInterestedMessage(sockfd);
}

void Peer::SendInterestedMessage(int sockfd)
{
	int n = 0;
	Message * m = new Message(lib->INTERESTED, doesItHaveAnyPieces(), listOfPieces, 0);
	char * message = lib->GetStringFromByteStream(m->GetActualMessageByteStream());

	delete m;
	n = write(sockfd, message, strlen(message)); //sends the bitfield message
	if (n < 0)
		error("ERROR writing to socket - SendInterestedMessage");
	else
	{
		std::cout << "sent interested\n";
	}
}

void Peer::SendNotInterestedMessage(int sockfd)
{
	int n = 0;
	Message * m = new Message(lib->NOTINTERESTED, doesItHaveAnyPieces(), listOfPieces, 0);
	char * message = lib->GetStringFromByteStream(m->GetActualMessageByteStream());
	delete m;
	n = write(sockfd, message, strlen(message)); //sends the bitfield message
	if (n < 0)
		error("ERROR writing to socket - SendNotInterestedMessage");
	else
	{
		std::cout << "sent not interested messagE\n";
	}
}
void Peer::receiveUnchokeMessage(std::vector<OURBYTE> messageStream)
{
}

void Peer::receiveChokeMessage(std::vector<OURBYTE> messageStream)
{
}

void Peer::receiveInterestedMessage(std::vector<OURBYTE> messageStream)
{
	interestedInMainPeer = true;
}

void Peer::receiveNotInterestedMessage(std::vector<OURBYTE> messageStream)
{
	interestedInMainPeer = false;
}

void Peer::receiveHaveMessage(std::vector<OURBYTE> messageStream)
{
}

void Peer::receiveRequestMessage(std::vector<OURBYTE> messageStream)
{
}

void Peer::receivePieceMessage(std::vector<OURBYTE> messageStream)
{
}

void Peer::UploadPieces()
{
	for (std::vector<Peer*>::iterator i = otherPeers.begin(); i < otherPeers.end(); i)
	{
		if ((*i)->isPrefferedNeighbor || (*i)->isOptomisticallyUnchokedNeighbor)
		{
			SendPieceMessage((*i)); //maybe not a great idea TODO decide
		}
	}
}
void Peer::SendPieceMessage(Peer * otherPeer)
{
	Message * m = new Message(lib->PIECE, doesItHaveAnyPieces(), listOfPieces, otherPeer->requestedPieceIndex); //todo check requested piece index
																												//std::cout << "here1";
	char * message = lib->GetStringFromByteStream(m->GetActualMessageByteStream());
	delete m;
	//std::cout << "here2";
	int n = write(otherPeer->establishedSockfd, message, strlen(message)); //sends the bitfield message
	if (n < 0)
		error("ERROR writing to socket - sendPieceMessage");
	else
	{
		std::cout << "piece message sent, requested index: " << otherPeer->requestedPieceIndex << std::endl;
	}
}
