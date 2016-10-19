#include "stdafx.h"
#include "Peer.h"
#include "Message.h"

Peer::Peer()
{
    // lolz love the name
    optomisticallyUnchokedNeighbor = NULL;
    ownPeerID = 0;
    std::vector<Peer*> otherPeers;
}

void Peer::SendHandshakeMessage(Peer * otherPeer)
{
    // need to create handshake message first
    HandshakeMessage* hsm = new HandshakeMessage(ownPeerID);
    
    // TODO
    // now find way to actually send it, need the output stream
    
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

void Peer::ReceiveHandshakeMessage(Message m)
{
    std::vector<OURBYTE> payload = m.getPayload();
    // TODO
    // need the inputstream to get this or have it handled elsewhere
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
