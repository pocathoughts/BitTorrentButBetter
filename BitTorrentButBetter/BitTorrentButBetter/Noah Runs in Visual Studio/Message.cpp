#include "stdafx.h"
#include "Message.h"
//
//  Message.cpp
//  BitTorrentButBetter
//
//  Created by Pocahantas Moore on 10/17/16.
//  Copyright � 2016 Christine Moore. All rights reserved.
//
//  Obviously this is not a complete file, i tried my best to make
//  this generic enough that anyone could use this
//
//  Also includes handling the first initial handshake message

#include "Message.h"


int getMessageType() {
	return messageType;
}

uint32_t getPayload() {
	return payload;
}


// could be useful
bool Message::hasPayload(int messageType) {
	return (messageType == lib->HAVE || messageType == lib->BITFIELD
		|| messageType == lib->REQUEST || messageType == lib->PIECE);
}


//constructor
Message::Message(int type) {
	/*messagePayload = messagePayload;
	messageType = type;
	messageLength = 1 + messagePayload.length;*/
	lib = new utilLib();
	GenerateActualMessageByteStream(type);
}

void Message::GenerateActualMessageByteStream(int type)
{
	//messageLength
	std::vector<OURBYTE> payload = GeneratePayload(type);
	std::vector<OURBYTE> messageLength = DetermineMessageLength();
	for (int i = 0; i < 4; i++)
	{
		OURBYTE currentByte;
		currentByte = messageLength[i];
		fullMessage.push_back(currentByte);
	}
	//type byte
	for (int i = 0; i < 1; i++)
	{
		OURBYTE typeByte(type);
		fullMessage.push_back(typeByte);
	}
	//payload
	for (int i = 0; i < payload.size(); i++)
	{
		OURBYTE currentByte;
		currentByte = payload[i];
		fullMessage.push_back(currentByte);
	}
	//the rest should automatically be 0
}
std::vector<OURBYTE> Message::DetermineMessageLength()
{
	//rewrite this function to return the message length, this is for compilation
	std::vector<OURBYTE> NEEDSTOBEIMPLEMENTED;
	OURBYTE thisByte;
	for (int i = 0; i < 4; i++)
	{
		NEEDSTOBEIMPLEMENTED.push_back(thisByte);
	}
	return NEEDSTOBEIMPLEMENTED;
}
std::vector<OURBYTE> Message::GeneratePayload(int type)
{
	//rewrite this function to return the payload, this is for compilation
	std::vector<OURBYTE> NEEDSTOBEIMPLEMENTED(0);
	return NEEDSTOBEIMPLEMENTED;
}

std::vector<OURBYTE> Message::GenerateMessage(uint32_t messageLength, MessageType type, uint32_t payload) {
	for(int i =0; i<4; i++){
		fullMessage[i] = messageLength >> (24-(i*8)); //big endian
	}
	fullMessage[4] = type;
	for(int i =5; i<9; i++){
		fullMessage[i] = messageLength >> (24-((i-5)*8));
	}
}
//
//
std::vector<OURBYTE> Message::GenerateBitfieldPayload(/*ListOfPieces pieces*/) {
		for(int i =5; i<7; i++){
			//fullMessage[i] = pieces;
		}
		fullMessage[7] = 0;
		fullMessage[8] = 0;
}

std::vector<OURBYTE> Message::GenerateHavePayload(/*variable indexPieces*/) {
	for(int i =5; i<9; i++){
		//fullMessage[i] = indexPieces >> (24-((i-5)*8));
	}
}

std::vector<OURBYTE> Message::GenerateRequestPayload(/*variable indexPieces*/) {
	for(int i =5; i<9; i++){
		//fullMessage[i] = indexPieces >> (24-((i-5)*8));
	}
}

std::vector<OURBYTE> Message::GeneratePiecePayload(/*variable indexPieces, variable content*/ ) {
	for(int i =5; i<9; i++){
		//fullMessage[i] = indexPieces >> (24-((i-5)*8));
	}
	//not sure how the content of the piece should be added
	//since the the message should just be 32 bits - Lara
}

HandshakeMessage::HandshakeMessage(int _peerID)
{
	std::vector<OURBYTE> handshakeHeader; //18 bytes
	std::vector<OURBYTE> zeroBytes; //10 zero bytes
	std::vector<OURBYTE> peerID; //4 bytes
	//�P2PFILESHARINGPROJ'
	lib = new utilLib();
	handshakeHeader = lib->GetByteStreamFromString("P2PFILESHARINGPROJ");
	peerID = lib->GetByteStreamFromInt(_peerID);
	GenerateHandshakeMessageByteStream(handshakeHeader, peerID);
}
void HandshakeMessage::GenerateHandshakeMessageByteStream(std::vector<OURBYTE> header, std::vector<OURBYTE> peerID)
{
	//header
	for (int i = 0; i < 18; i++)
	{
		OURBYTE currentByte;
		currentByte = header[i];
		fullMessage.push_back(currentByte);
	}
	//zero bits
	for (int i = 0; i < 10; i++)
	{
		OURBYTE zero;
		fullMessage.push_back(zero);
	}
	//peer ID
	for (int i = 0; i < 4; i++)
	{
		OURBYTE currentByte;
		currentByte = peerID[i];
		fullMessage.push_back(currentByte);
	}
}

std::vector<OURBYTE> HandshakeMessage::GetHandshakeMessageByteStream()
{
	return fullMessage;
}


std::vector<OURBYTE> Message::GetActualMessageByteStream()
{
	return fullMessage;
}
