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
    int ownPeerID;
    int neighborID;
    // Socket neighborSocket
    // need some kind of input and output stream to send the data
    // 1 is true for below 0 is false
    // below are the boolean variables
    int peerIsChoking;
    int hasFile;
    int sentHandshake;
};

