// NetworkingProject.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "message.h"
int k;

int main()
{
	HandshakeMessage * h = new HandshakeMessage(1);
	std::vector<OURBYTE> handshakeMessage = h->GetHandshakeMessageByteStream();
	utilLib * lib = new utilLib;
	Message * m = new Message(5);
	std::vector<OURBYTE> actualMessage = m->GetActualMessageByteStream();

	lib->printByteStream(handshakeMessage);

	delete h;
	delete m;
	delete lib;
	while (true);
    return 0;
}

