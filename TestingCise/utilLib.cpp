#include "stdafx.h"
#include "utilLib.h"


void utilLib::setNeighborList(int list[5])
{
	for (int i = 0; i < 5; i++)
	{
		neighborList[i] = list[i];
	}
}

utilLib::utilLib()
{
}


utilLib::~utilLib()
{
}


#include <fstream>
#include <vector>
#include <iostream>
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string.h>

using namespace std;

void utilLib::reglog(int logtype, char * time, int peer_ID, int peer2_ID) {
	std::ofstream outfile;
	char logname[] = "log_peer_num.log"; //TO DO NOAH, STRING MANIPULATION, LOG PER PEER ID
	int str;
	outfile.open(logname, std::ios_base::app);
	switch (logtype) {
	case 0: //connection TO another peer	
		outfile << sprintf("%c: Peer %d makes a connection to Peer %d.\n", time, peer_ID, peer2_ID);
	case 1: //connection FROM another peer
		outfile << sprintf("%c: Peer %d is connected from Peer %d.\n", time, peer_ID, peer2_ID);
	case 2: //change of optimistically unchoked neighbor
		outfile << sprintf("%c: Peer %d has the optimistically unchoked neighbor %d.\n", time, peer_ID, peer2_ID);
	case 3: //unchoking
		outfile << sprintf("%c: Peer %d is unchoked by %d.\n", time, peer_ID, peer2_ID);
	case 4: //choking
		outfile << sprintf("%c: Peer %d is choked by %d.\n", time, peer_ID, peer2_ID);
	case 5: //interested
		outfile << sprintf("%c: Peer %d received the 'interested' message from %d.\n", time, peer_ID, peer2_ID);
	case 6: //not interested	
		outfile << sprintf("%c: Peer %d received the 'not interested' message from %d.\n", time, peer_ID, peer2_ID);
	}
}

void utilLib::pnlog(char * time, int peer_ID) { //logging preferred neighbors //TO DO NOAH NEIGHBORLIST TYPE
	std::ofstream outfile;
	char logname[] = "log_peer_num.log";
	outfile.open(logname, std::ios_base::app);
	outfile << sprintf("%c: Peer %d has the preferred neighbors %d \n", time, peer_ID, neighborList);
}

void utilLib::hplog(char * time, int peer_ID, int peer2_ID, int index) { //have piece log
	std::ofstream outfile;
	char logname[] = "log_peer_num.log";
	outfile.open(logname, std::ios_base::app);
	outfile << sprintf("%c: Peer %d received the 'have' message from %d for the piece %d\n", time, peer_ID, peer2_ID, neighborList);
}

void utilLib::dplog(char * time, int peer_ID, int peer2_ID, int index, int num_Pieces) { //downloading piece log
	std::ofstream outfile;
	char logname[] = "log_peer_num.log";
	outfile.open(logname, std::ios_base::app);
	outfile << sprintf("%c: Peer %d has downloaded the piece %d from %d. Now the number of pieces it has is %d.\n", time, peer_ID, index, peer2_ID, num_Pieces);

}

void utilLib::cdlog(char * time, int peer_ID) { //completed download log
	std::ofstream outfile;
	char logname[] = "log_peer_num.log";
	outfile.open(logname, std::ios_base::app);
	outfile << sprintf("%c: Peer %d	has downloaded the complete file.\n", time, peer_ID);
}

char * utilLib::getTime()
{
	time_t now = time(0);
	tm * hms = localtime(&now);
	ostringstream oss;

	oss << "Time : " << 1 + hms->tm_hour << ":"
		<< 1 + hms->tm_min << ":"
		<< 1 + hms->tm_sec << endl;
	std::string s = oss.str();
	char* pString = new char[s.length() + 1];
	std::copy(s.c_str(), s.c_str() + s.length() + 1, pString);

	return pString;
}