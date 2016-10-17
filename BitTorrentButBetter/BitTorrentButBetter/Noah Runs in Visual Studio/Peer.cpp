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

}
void Peer::ReceiveHandshakeMessage(Message m) 
{

}
void Peer::ReceiveActualMessage(Message m, Peer * sender)
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
Peer::~Peer()
{
	delete optomisticallyUnchokedNeighbor;
	for (std::vector<Peer*>::iterator i = otherPeers.begin(); i < otherPeers.end(); i++)
	{
		delete (*i);
	}
}
