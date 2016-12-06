// NetworkingProject.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "PeerProcess.h"

int main(int argc, char *argv[])
{
	//comment in to test from visual studio
	//argv[1] = "1002";
	utilLib * lib = new utilLib();
	//starts the process of the passed in value which should look like "1001"
	startPeerProcess(GetPeerIDFromInput(argv[1]));

	/*
	//testing handshake transmission
	HandshakeMessage * h = new HandshakeMessage(7);
	std::vector<OURBYTE> receivedMessage = h->GetHandshakeMessageByteStream();
	//get peerID from end of handshake message
	std::vector<OURBYTE>::const_iterator first = receivedMessage.begin() + 28; //last 4 bytes
	std::vector<OURBYTE>::const_iterator last = receivedMessage.end();
	std::vector<OURBYTE> peerIDPortion(first, last);
	*/
	//int receivedPeerID = lib->GetIntFromByteStream(peerIDPortion);
	 
	//other things that are being tested but commented out for now
	/*
	lib->reglog(0, lib->getTime(), 1001, 1002);
	std:vector<bool> a;
	a.push_back(true);
	for (int i = 1; i < 306; i++) {
		a.push_back(false);
	}

	Message * m = new Message(5, true, a, 3);
	//test things here
	////testing message passing -- currently works with a some hacky $$$$$$ 
	HandshakeMessage * h = new HandshakeMessage(7);
	std::vector<OURBYTE> BS = h->GetHandshakeMessageByteStream();
	char * test = lib->GetStringFromByteStream(BS);
	//std::vector<OURBYTE> BS2 = lib->GetByteStreamFromString(test);
	//char * test2 = lib->GetStringFromByteStream(BS2);

	//starts the process of the passed in value which should look like "1001"
	startPeerProcess(GetPeerIDFromInput(argv[1]));

	//wait for input that looks like "peerProcess 1001 or whatever. The description is ambiguous
	//checkInput();

	*/
	return 0;
}

int GetPeerIDFromInput(string line)
{
	char * message = new char[100];
	bool areWeThereYet = true;
	int count = 0;
	for (int i = 0; i < line.length(); i++)
	{
			message[count] = line.at(i);
			count++;
	}
	message[count] = '\0';
	return atoi(message);
}
void startPeerProcess(int peerID)
{
	initializePeerWithID(peerID);
}
//not sure if we will use this
void checkInput()
{
	string input = "";
	char * message = new char[100];
	while (true)
	{

		while (true)
		{
			cout << "Please enter a valid peerID to start (format: 'peerProcess 1001':\n";
			getline(cin, input);
			stringstream myStream(input);
			if (myStream >> message)
				break;
			cout << "Invalid number, please try again" << endl;
		}
		startPeerProcess(GetPeerIDFromInput(input));
		//this is for testing with a server that was started elsewhere 
	}

	// This code converts from string to number safely.
}
void parsePeerData(std::string line)
{
	char * message1 = new char[100]; //peerID
	char * message2 = new char[100]; //hostName
	char * message3 = new char[100]; //portNum
	char * message4 = new char[100]; //hasFile
	int areWeThereYet = 0;
	int count = 0;
	for (int i = 0; i < line.length(); i++)
	{
		if (i == 0)
		{
			message1[0] = line.at(0);
			count++;
			continue;
		}
		if (line.at(i - 1) == ' ')
		{
			if (areWeThereYet == 0)
			{
				message1[count] = '\0';
			}
			else if (areWeThereYet == 1)
			{
				message2[count] = '\0';

			}
			else if (areWeThereYet == 2)
			{
				message3[count] = '\0';

			}
			else if (areWeThereYet == 3)
			{
				message4[count] = '\0';

			}
			areWeThereYet++;
			count = 0;
		}
		if (areWeThereYet == 0)
		{
			message1[count] = line.at(i);
			count++;
		}
		else if (areWeThereYet == 1)
		{
			message2[count] = line.at(i);
			count++;
		}
		else if (areWeThereYet == 2)
		{
			message3[count] = line.at(i);
			count++;
		}
		else if (areWeThereYet == 3)
		{
			message4[count] = line.at(i);
			count++;
		}
	}
	Peer * p = new Peer(atoi(message1), message2, atoi(message3), atoi(message4), allPeers);
	allPeers.push_back(p);
}
void initializePeers()
{
	std::ifstream loadedFile("PeerInfo.cfg");

	std::string line;
	int index = 0;
	if (loadedFile.is_open())
	{
		while (loadedFile.good())
		{
			std::getline(loadedFile, line);
			parsePeerData(line);
		}
	}
	else
	{
		std::cout << "Failed to open PeerInfo.cfg 404 file not found lol\n";
	}
}
int DetermineIndexOfIDInCFGFile(int peerID)
{
	std::ifstream loadedFile("PeerInfo.cfg");

	std::string line;

	if (loadedFile.is_open())
	{
		char * message1 = new char[100];
		int index = 0;
		while (loadedFile.good())
		{
			int count = 0;
			std::getline(loadedFile, line);
			for (int i = 0; i < line.length(); i++)
			{

				if (i == 0)
				{
					message1[0] = line.at(0);
					count++;
					continue;
				}
				
				if (line.at(i) == ' ')
				{
					message1[count] = '\0';
					break;
				}
				message1[count] = line.at(i);
				count++;

			}
			if (atoi(message1) == peerID)
			{
				//startPeerProcess
				return index;
			}
			index++;
		}
		return -1; //id not found in file
	}
	else
	{
		std::cout << "Failed to open PeerInfo.cfg 404 file not found lol\n";
	}
}
void initializePeerWithID(int peerID)
{
	int indexInCFGFile = DetermineIndexOfIDInCFGFile(peerID);
	if (indexInCFGFile == -1)
	{
		//id not found
		cout << "ID: " << peerID << " not found in cfg file. No peer initialization \n";
		return;
	}
	std::ifstream loadedFile("PeerInfo.cfg");

	std::string line;
	int index = 0;
	if (loadedFile.is_open())
	{
		while (loadedFile.good())
		{
			std::getline(loadedFile, line);
			if (index == indexInCFGFile)
			{
				parsePeerData(line);
			}
			index++;
		}
	}
	else
	{
		std::cout << "Failed to open PeerInfo.cfg 404 file not found lol\n";
	}
}
//starts a putty connection on the given hostname
//void ssh(char * hostname)
//{
//	char * cmd = new char[100];
//	cmd = "putty -ssh ";
//	string s1 = cmd;
//	string s2 = hostname;
//	char * restOfCommand = " -l npresser -pw well i fucked that up";
//	string s3 = restOfCommand;
//	s1 = s1 + s2 + s3;
//	char * a = _strdup(s1.c_str());
//	system(a);
//	//system("putty -ssh lin114-00.cise.ufl.edu -l npresser -pw THISISNOTMYPASSWORD");
//}