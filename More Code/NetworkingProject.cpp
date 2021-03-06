// NetworkingProject.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Message.h"
#include "Peer.h"
using namespace std;

int k;
std::vector<Peer*> allPeers;
void initializePeers();
void initializePeerWithID(int peerID);
void checkInput();
void ssh(char * hostname);
int main()
{
	utilLib * lib = new utilLib();

	////testing message passing -- currently works with a some hacky $$$$$$ 
	//HandshakeMessage * h = new HandshakeMessage(7);
	//std::vector<OURBYTE> BS = h->GetHandshakeMessageByteStream();
	//char * test = lib->GetStringFromByteStream(BS);
	//std::vector<OURBYTE> BS2 = lib->GetByteStreamFromString(test);
	//char * test2 = lib->GetStringFromByteStream(BS2);

	//initializePeers();

	//ssh("lin114-00.cise.ufl.edu");
	checkInput();
	return 0;
}
int GetPeerIDFromInput(string line)
{
	char * message = new char[100];
	bool areWeThereYet = false;
	int count = 0;
	for (int i = 1; i < line.length(); i++)
	{

		if (line.at(i - 1) == ' ')
		{
			message[0] = line.at(i);
			areWeThereYet = true;
			count++;
		}
		else if (areWeThereYet)
		{
			message[count] = line.at(i);
			count++;
		}
	}
	message[count] = '\0';
	return atoi(message);
}
void startPeerProcess(int peerID)
{
	initializePeerWithID(peerID);
}
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
void ssh(char * hostname)
{
	char * cmd = new char[100];
	cmd = "putty -ssh ";
	string s1 = cmd;
	string s2 = hostname;
	char * restOfCommand = " -l npresser -pw well i fucked that up";
	string s3 = restOfCommand;
	s1 = s1 + s2 + s3;
	char * a = _strdup(s1.c_str());
	system(a);
	//system("putty -ssh lin114-00.cise.ufl.edu -l npresser -pw THISISNOTMYPASSWORD");
}