#pragma once

#include <iostream>
#include <stdio.h>
#include "utilLib.h"


class Message
{
private:
	std::vector<OURBYTE> messageLength; // 4 bytes
	OURBYTE messageTypeField; //1 byte
	std::vector<OURBYTE> messagePayload; //variable length, (0s if empty)
	utilLib * lib;
	std::vector<OURBYTE> fullMessage;
	std::vector<OURBYTE> DetermineMessageLength();
	void GenerateActualMessageByteStream(int type);
	std::vector<OURBYTE> GeneratePayload(int type);
	bool hasPayload(int messageType);


public:
	Message(int type);
	std::vector<OURBYTE> GetActualMessageByteStream();
};
class HandshakeMessage
{
public:
	
	HandshakeMessage(int _peerID);
	std::vector<OURBYTE> GetHandshakeMessageByteStream();
private:
	std::vector<OURBYTE> fullMessage; //32 bytes
	void GenerateHandshakeMessageByteStream(std::vector<OURBYTE> _header, std::vector<OURBYTE> _peerID);
	utilLib * lib;
};

