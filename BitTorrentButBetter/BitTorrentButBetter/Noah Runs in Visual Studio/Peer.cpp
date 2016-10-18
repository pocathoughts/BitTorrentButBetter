#include "stdafx.h"
#include "Peer.h"

Peer::Peer()
{
	optomisticallyUnchokedNeighbor = NULL;
}

void Peer::SendHandshakeMessage(Peer * otherPeer)
{

}
void Peer::SendActualMessage(int type)
{

}
void Peer::EstablishConnection(Peer * otherPeer)
{
	//go to peerinfo.cfg to establish connections w/ peers above it
	//after establishing TCP connection, send handshake message to other peer
}
void Peer::ReceiveHandshakeMessage(Message m)
{
	//check handshake header peerID and make sure its expected
}
void Peer::ReceiveActualMessage(Message m, Peer * sender)
{
	//bitfield message after handshake: check for new pieces, send interested or
	//not interested
}
void Peer::DeterminePreferredNeighbors()
{
	//# of preferred neighbors in common.cfg
	//this functioned should be called after every p seconds in common.cfg
	//check download rate of peers from prev unchoking interval
	//send unchoke/choke messages to selected peers
}
void Peer::DetermineOptomisticallyUnchokedNeighbor()
{

}
void Peer::DetermineInterested()
{
	//if peer has received message type = 'bitfield' or 'have' AND bitfields differ
}
Peer::~Peer()
{
	delete optomisticallyUnchokedNeighbor;
	for (std::vector<Peer*>::iterator i = otherPeers.begin(); i < otherPeers.end(); i++)
	{
		delete (*i);
	}
}
