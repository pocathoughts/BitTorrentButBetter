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
	// lolz love the name
	optomisticallyUnchokedNeighbor = NULL;
	//std::vector<Peer*> otherPeers = preexistingPeers;
	//probably best to handle the above upon a successful connection
	ReadCfgFile();
	numPieces = ceil((float)fileSize / (float)pieceSize);
	lastPieceSize = fileSize % pieceSize;
	CreateBitfield();

	//comment back in once you can remotely start servers from other machines
	InitializeTCPConnections(preexistingPeers);
}
void Peer::InitializeTCPConnections(std::vector<Peer*> preexistingPeers)
{
	//start listening (TCP Server)

	startServer();
	if (preexistingPeers.size() == 0)
	{
		return;
	}
	for (std::vector<Peer*>::iterator i = preexistingPeers.begin(); i < preexistingPeers.end(); i++)
	{
		startClient((*i)->hostName, (*i)->peerID);
	}
	//connect to all others (TCP Client)
	//startClient(handshakeMessageString -- for all peers in preexistingPeers

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
	}
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

void Peer::SendActualMessage(int type)
{
	// TODO
	// find a way to actually send it, need ouptut stream
}

void Peer::EstablishConnection(Peer * otherPeer)
{
	// code to connect to other peer's socket
}


void Peer::ReceiveMessage(std::vector<OURBYTE> receivedMessage)
{
	//just use receiveActualMessage
}
void Peer::ServerReceiveHandshakeMessage(std::vector<OURBYTE> receivedMessage)
{
	//add peer to list

	//
}
void Peer::ClientReceiveReturnHandshakeMessage(std::vector<OURBYTE> receivedMessage, int expectedPeerID)
{
	//if it is the client, we need to do the below

	//check if the sender is the right sender
		//is the peer ID the expected peer ID
		//check handshake header to see if it is correct (P2PFILE....)

}
void Peer::ReceiveActualMessage(std::vector<OURBYTE> receivedMessage)
{

}

void Peer::DeterminePreferredNeighbors()
{

}

void Peer::DetermineOptomisticallyUnchokedNeighbor()
{

}

void Peer::DetermineInterested()
{

}

//void Peer::SetPeerID(int _peerID)
//{
//	peerID = _peerID;
//}
//
//void Peer::SetHostName(char * _hostName)
//{
//	hostName = _hostName;
//}
//
//void Peer::SetPortNum(int _portNum)
//{
//	portNum = _portNum;
//}
//
//void Peer::SetFileCompletion(bool _fileComplete)
//{
//	fileComplete = _fileComplete;
//}

Peer::~Peer()
{
	delete optomisticallyUnchokedNeighbor;
	for (std::vector<Peer*>::iterator i = otherPeers.begin(); i < otherPeers.end(); i++)
	{
		delete (*i);
	}
}

#include "stdafx.h"
#include "winsockServer.h"
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 30000 //may need to increase
//#define DEFAULT_PORT "27015"

int Peer::startServer()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	std::string s = std::to_string(portNum);
	char const *pchar = s.c_str();
	iResult = getaddrinfo(NULL, pchar, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// This call is used if you no longer need server socket, however, we do
	//closesocket(ListenSocket);
	bool expectingHandshakeMessage = true;
	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//this is where receive message functionality goes
			printf("Bytes received: %d\n", iResult);
			ReceiveMessage(lib->GetByteStreamFromString(recvbuf));

			//server just started, get handshake message
			if (expectingHandshakeMessage)
			{
				ServerReceiveHandshakeMessage(lib->GetByteStreamFromString(recvbuf));
				iSendResult = send(ClientSocket, CreateHandshakeMessage(), iResult, 0);
				expectingHandshakeMessage = false;
			}
			else
			{
				ReceiveActualMessage(lib->GetByteStreamFromString(recvbuf));
			}
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}

int Peer::startClient(char * hostNameToConnectTo, int otherPeerID)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	//creates the handshake message to be sent to the host
	char *sendbuf = CreateHandshakeMessage(); //client sends first handshake
	clientExpectingHandshakeMessage = true; //Now expecting a return handshake message
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	std::string s = std::to_string(portNum);
	char const *pchar = s.c_str();
	iResult = getaddrinfo(hostNameToConnectTo, pchar, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	bool expectingReturnHandshake = true;
	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			//wait for the handshake message before using other messages
			if (expectingReturnHandshake)
			{
				ClientReceiveReturnHandshakeMessage(lib->GetByteStreamFromString(recvbuf), otherPeerID);
				expectingReturnHandshake = false;
			}
			else
			{
				ReceiveActualMessage(lib->GetByteStreamFromString(recvbuf));
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}