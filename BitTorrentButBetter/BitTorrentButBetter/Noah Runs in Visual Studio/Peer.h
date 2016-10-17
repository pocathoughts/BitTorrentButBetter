#pragma once
#include <vector>
#include "Message.h"
#include "utilLib.h"

class Peer
{
public:
	Peer();
	void SendHandshakeMessage(Peer * otherPeer);
	void SendActualMessage(int type);
	void EstablishConnection(Peer * otherPeer);
	void ReceiveHandshakeMessage(Message m);
	void ReceiveActualMessage(Message m, Peer * sender);
	void DeterminePreferredNeighbors();
	void DetermineOptomisticallyUnchokedNeighbor();
	void DetermineInterested();
	void CreateHandshakeMessage();
	~Peer();
private:
	std::vector<Peer*> otherPeers;
	Peer * optomisticallyUnchokedNeighbor;
};

