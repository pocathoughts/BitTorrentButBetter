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

	void InitializeTCPConnections(std::vector<Peer*> preexistingPeers);

	void StartServer();

	void CreateBitfield();
	
	void SendHandshakeMessage(Peer * otherPeer);
	void SendActualMessage(int type);
	void EstablishConnection(Peer * otherPeer);
	void ReceiveMessage(std::vector<OURBYTE> receivedMessage);
	void ServerReceiveHandshakeMessage(std::vector<OURBYTE> receivedMessage);
	void ClientReceiveReturnHandshakeMessage(std::vector<OURBYTE> receivedMessage, int expectedPeerID);
	void ReceiveActualMessage(std::vector<OURBYTE> receivedMessage);
	void ReceiveActualMessage(Message m, Peer * sender);
	void DeterminePreferredNeighbors();
	void DetermineOptomisticallyUnchokedNeighbor();
	void DetermineInterested();


	~Peer();
	int startServer();
	int startClient(char * hostNameToConnectTo, int otherPeerID);
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
	int hasFile;
	
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
};

