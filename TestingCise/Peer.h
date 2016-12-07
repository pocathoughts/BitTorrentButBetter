#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include "Message.h"
#include "utilLib.h"
class Peer
{
public:

	Peer(int _peerID, char * _hostName, int _portNum, bool _fileComplete, std::vector<Peer*> preexistingPeers);

	Peer(int _peerID, char * _hostName, int _portNum, bool _fileComplete);

	void InitializeTCPConnections(std::vector<Peer*> preexistingPeers);

	int * GetAndSetListOfNeighbors();

	void WaitForClientBitfieldMessage(int sockfd);

	void WaitForServerBitfieldMessage(int sockfd);

	void startServerLinux();
	void startClientLinux(char * hostName, int otherPeerID);


	void CreateBitfield();
	
	void AwaitMessage(int sockfd);
	bool receiveHandshakeMessage(std::vector<OURBYTE> receivedMessage, int sockfd);
	void DeterminePreferredNeighbors();
	void DetermineOptomisticallyUnchokedNeighbor();


	~Peer();
	void SendHandshakeMessageFromServer(int sockfd);
	bool SendHandshakeMessageFromClient(int sockfd);
	void SendClientBitfieldMessage(int sockfd);
	void SendServerBitfieldMessage(int sockfd);
private:
	utilLib * lib;

	//cfg loading
	void ReadCfgFile();
	void SetNumPrefferedNeighbors(std::string line);
	void SetUnchokingInterval(std::string line);
	void SetOptomisticUnchokingInterval(std::string line);
	void SetFileName(std::string line);
	void SetFileSize(std::string line);
	void SetPieceSize(std::string line);

	std::vector<Peer*> otherPeers;
	Peer * optomisticallyUnchokedNeighbor;
	// Socket neighborSocket
	// need some kind of input and output stream to send the data
	// 1 is true for below 0 is false
	// below are the boolean variables
	int peerIsChoking;
	
	//set when a handshake is supposed to be returned
	bool serverReturnHandshakeMessage = false;
	//if the client has just connected to the server and is expecting a handshake message
	bool clientExpectingHandshakeMessage = false;
	std::vector<OURBYTE> bitfield;
	void SetBitfieldToEmpty();
	void SetBitfieldToFull();
	void SetByteInBitfieldToTrue(int byteNum);

	char * CreateHandshakeMessage();

	//common.cfg properties
	int numPrefferedNeighbors;
	float unchokingInterval;
	float optomisticUnchokingInterval;
	char * fileName;
	long fileSize;
	long pieceSize;
	int numPieces; // maybe not necessary
	int lastPieceSize;

	//peerInfo.cfg properties
	int peerID;
	char * hostName;
	int portNum;
	bool fileComplete;

	//check if any pieces at all
	bool doesItHaveAnyPieces();

	//list of which pieces peer has
	std::vector<bool> listOfPieces;

	//list of my neighbors
	int listOfNeighbors[5];


	//message handling
	void DetermineInterested(std::vector<OURBYTE> messageStream, int sockfd);
	void SendInterestedMessage(int sockfd);
	void SendNotInterestedMessage(int sockfd);
	void receiveUnchokeMessage(std::vector<OURBYTE> messageStream);
	void receiveChokeMessage(std::vector<OURBYTE> messageStream);
	void receiveInterestedMessage(std::vector<OURBYTE> messageStream);
	void receiveNotInterestedMessage(std::vector<OURBYTE> messageStream);
	void receiveHaveMessage(std::vector<OURBYTE> messageStream);
	void receiveRequestMessage(std::vector<OURBYTE> messageStream);
	void receivePieceMessage(std::vector<OURBYTE> messageStream);

	//recent adds
	std::string subDirAndFile;
	std::string subdir;
};

