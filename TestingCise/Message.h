#pragma once

#include <iostream>
#include <stdio.h>
#include "utilLib.h"


class Message
{
private:
	//std::vector<OURBYTE> messageLength; // 4 bytes
	//OURBYTE messageTypeField; //1 byte
	//std::vector<OURBYTE> messagePayload; //variable length, (0s if empty)
	utilLib * lib;
	std::vector<OURBYTE> fullActualMessage;
	std::vector<OURBYTE> DetermineMessageLength();
	std::vector<OURBYTE> GeneratePayload(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedLength);
	std::vector<OURBYTE> GenerateEmptyPayload();
	std::vector<OURBYTE> GenerateBitfieldPayload(bool hasPiece, std::vector<bool> bitfield);
	std::vector<OURBYTE> GenerateHavePayload(int index);
	std::vector<OURBYTE> GenerateRequestPayload(int requestedLength);
	//std::vector<OURBYTE> GeneratePiecePayload();
    std::vector<OURBYTE> GeneratePiecePayload(OURBYTE pieceData);
	bool hasPayload(int messageType);
	int messageType;

	void GenerateActualMessageByteStream(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedLength);


public:
	Message(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedIndex);
	std::vector<OURBYTE> GetActualMessageByteStream();
};


class HandshakeMessage
{
public:

	HandshakeMessage(int _peerID);
	std::vector<OURBYTE> GetHandshakeMessageByteStream();
	char * GetHandshakeMessageAsString();
private:
	std::vector<OURBYTE> fullHandshakeMessage; //32 bytes
	void GenerateHandshakeMessageByteStream(std::vector<OURBYTE> _header, std::vector<OURBYTE> _peerID);
	utilLib * lib;
};

//void GenerateActualMessageByteStream(std::vector<OURBYTE> messageLength, std::vector<OURBYTE> messageType, std::vector<OURBYTE> messagePayload);
