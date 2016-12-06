#pragma once
#include "stdafx.h"
#include "utilLib.h"
#include "Message.h"
#include "Peer.h"
using namespace std;

int k;
std::vector<Peer*> allPeers;
void initializePeers(int commandLineInputPeerID);
void initializePeerWithID(int peerID);
int GetPeerIDFromInput(string line);
void startPeerProcess(int peerID);
void checkInput();