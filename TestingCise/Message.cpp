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
//  NOTE
//  throughout the message payload functions there is a field called
//  4 byte piece index field
//  this value is the piece currently being referenced index in the
//  listofPieces array b/c and integer is 4 bytes

#include "Message.h"
#include <algorithm> //needed for reversal of vector

// could be useful
bool Message::hasPayload(int messageType) {
	return (messageType == lib->HAVE || messageType == lib->BITFIELD
		|| messageType == lib->REQUEST || messageType == lib->PIECE);
}


//helper function
//takes integer and makes bit representation of number
std::vector<int> convert(int x) {
	std::vector<int> ret;
	while (x) {
		if (x & 1)
			ret.push_back(1);
		else
			ret.push_back(0);
		x >>= 1;
	}
	int size = ret.size();
	if (size < 32) {
		int counter = 32 - size;
		for (int i = 0; i < counter; i++) {
			ret.push_back(0);
		}
	}
	std::reverse(ret.begin(), ret.end());
	return ret;
}




//start of ActualMessage code

//constructor for the ActualMessage
Message::Message(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedIndex) {
	/*messagePayload = messagePayload;
	messageType = type;
	messageLength = 1 + messagePayload.length;*/
	std::cout << "message constructor begin type: " << type << std::endl;

	lib = new utilLib();
	//std::cout << "message constructor 2\n";
	GenerateActualMessageByteStream(type, hasPiece, listOfPieces, requestedIndex);
	std::cout << "message constructor completion type: " << type << std::endl;

}
Message::~Message()
{
	delete lib;
}

//most central message function
void Message::GenerateActualMessageByteStream(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedIndex)
{
	//std::cout << "generate bytestream\n";
	std::vector<OURBYTE> payload;
	payload.reserve(38);
	payload = GeneratePayload(type, hasPiece, listOfPieces, requestedIndex);
	//std::cout << "generate bytestream 2\n";
	std::vector<OURBYTE> messageLength;
	messageLength.reserve(4);
	messageLength = DetermineMessageLength();
	//std::cout << "generate bytestream 3\n";
	fullActualMessage.reserve(50); //max should be 38 + 4 + 1 = 43
	//messageLength
	for (int i = 0; i < 4; i++)
	{
		OURBYTE currentByte;
		currentByte = messageLength[i];
		fullActualMessage.push_back(currentByte);
	}
	//type byte
	for (int i = 0; i < 1; i++)
	{
		OURBYTE typeByte(type);
		//lib->GetByteStreamFromInt(type)[0]
		fullActualMessage.push_back(typeByte);
	}
	//payload
	for (int i = 0; i < payload.size(); i++)
	{
		OURBYTE currentByte;
		currentByte = payload[i];
		fullActualMessage.push_back(currentByte);
	}
}


//simple getter method
std::vector<OURBYTE> Message::GetActualMessageByteStream()
{
	return fullActualMessage;
}


//TODO
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


//interface of sorts to making the payload
//different message types mean a different type of payload
std::vector<OURBYTE> Message::GeneratePayload(int type, bool hasPiece, std::vector<bool> listOfPieces, int requestedIndex)
{
	std::cout << "gen payload type: " << type << std::endl;
	std::vector<OURBYTE> payload;
	//rewrite this function to return the payload, this is for compilation
	//std::vector<OURBYTE> NEEDSTOBEIMPLEMENTED(0);
	if (type == 0 || type == 1 || type == 2 || type == 3) {
		//choke, unchoke, interested, or not interested message respectively
		payload = GenerateEmptyPayload();
		return payload;
	}
	else if (type == 4) {
		//have message
		payload = GenerateHavePayload(requestedIndex);
		return payload;
	}
	else if (type == 5) {
		//bitfield message
		payload = GenerateBitfieldPayload(hasPiece, listOfPieces);
		return payload;
	}
	else if (type == 6) {
		//request message
		payload = GenerateRequestPayload(requestedIndex);
		return payload;
	}
	else if (type == 7) {
		//piece message
		payload = GeneratePiecePayload(requestedIndex);
		return payload;
	}
	else {
		//invalid type returns and empty vector
		return payload;
	}
}


//in the case of type 0, 1, 2, or 3
std::vector<OURBYTE> Message::GenerateEmptyPayload() {
	//assuming this part of the message is still 32 bits
	//the payload is nothing but zeros
	std::vector<OURBYTE> emptyPayload;

	for (int i = 5; i < 9; i++) {
		emptyPayload.push_back(0);
	}
	return emptyPayload;
}


//in the case of type 4
//A client can send you a series of Have messages,
//one for each piece it has.
//each have message is just for one single piece
std::vector<OURBYTE> Message::GenerateHavePayload(int index) {
	//4 byte piece index field repesenting an index of
	//a piece that the client has
	//turn the integer into a byte representation and then
	//use that as your payload

	//the assumption is that if our index is 6, its bit rep is
	//00000000 00000000 00000000 00000110
	//so the first byte would be 00000000
	//second is 00000000
	//third is 00000000
	//fourth is 00000110

	OURBYTE indexByte;
	std::vector<OURBYTE> havePayload;
	std::vector<int> bitRepresentation;
	bitRepresentation = convert(index);
	havePayload = lib->convertBitsToBytes(bitRepresentation);

	return havePayload;
}


//in the case of type 5
//sent immediately after the handshake message
//succinctly describe the pieces that a peer has.
//each bit represents a piece that a message has
//similar to have message but more encompassing
std::vector<OURBYTE> Message::GenerateBitfieldPayload(bool hasPiece, std::vector<bool> listOfPieces) {
	//std::cout << "gen bitfieldpayload 1\n";
	std::vector<OURBYTE> bitfieldPayload;
	//The payload is a bitfield representing the pieces that have been successfully downloaded.
	//can be of a variable length


		//each bit represents if it has a piece, not each byte
		//so the bitfieldPayload will be bitfield.size() / 8
	//std::cout << "gen bitfieldpayload 2\n";
	int initialNumberOfBytes = listOfPieces.size() / 8;
	//std::cout << "gen bitfieldpayload 3\n";
	bitfieldPayload.reserve(initialNumberOfBytes);

	int counter = 0;
	for (int i = 0; i < initialNumberOfBytes; i++) {
		OURBYTE currentByte;
		for (int j = 0; j < 8; j++) {
			//setting the value of the bits of the currentByte with
			//the value of listOfPieces
			//std::cout << "gen bitfieldpayload 3.1\n";
			lib->setBit(currentByte, j, listOfPieces[counter]);

			//std::cout << "gen bitfieldpayload 3.2\n";
			counter++;
		}
		//lib->printBitsInByte(currentByte);
		bitfieldPayload.push_back(currentByte);
		//std::cout << "push back done, bits: ";
		//lib->printBitsInByte(bitfieldPayload.back());
	}
	//std::cout << "gen bitfieldpayload 5\n";

	return bitfieldPayload;
}


//in the case of type 6
//once you have sent your Interested message and received an
//Unchoke message, you can start requesting pieces!
std::vector<OURBYTE> Message::GenerateRequestPayload(int requestedIndex) {
	//4 byte piece index of the piece you want!
	//4 byte block offset
	//4 byte block length

	//Note that if the piece_length is not evenly divisible by the block
	//request size, the last block of each piece will be smaller than
	//the normal block size, It is likely that at least the last block
	//of the last piece will be smaller than the block request size
	//(and the last piece smaller than the piece_length)

	OURBYTE indexByte;
	std::vector<OURBYTE> requestPayload;
	std::vector<int> bitRepresentation;
	bitRepresentation = convert(requestedIndex);

	//below the payload has the 4 byte piece index
	requestPayload = lib->convertBitsToBytes(bitRepresentation);

	/*
	//next we need to generate the 4byte offset...
	//but how do we determine the offset?
	//NEEDS TO BE IMPLEMENTED, FOR NOW ALL ZEROS

	OURBYTE first;
	OURBYTE second;
	OURBYTE third;
	OURBYTE fourth;

	for(int i = 0; i < 8; i++){
	lib->setBit(first, i, 0);
	lib->setBit(second, i, 0);
	lib->setBit(third, i, 0);
	lib->setBit(fourth, i, 0);
	}

	requestPayload.push_back(first);
	requestPayload.push_back(second);
	requestPayload.push_back(third);
	requestPayload.push_back(fourth);

	//now we need to give the 4 byte block length
	//intenet reccomends 2^14 power
	//which is 00000000 00000100 00000000 00000000

	OURBYTE firstByte;
	OURBYTE secondByte;
	OURBYTE thirdByte;
	OURBYTE fourthByte;

	for(int i = 0; i < 8; i++){
	lib->setBit(firstByte, i, 0);
	lib->setBit(thirdByte, i, 0);
	lib->setBit(fourthByte, i, 0);
	}
	for(int i = 0; i < 8; i++){
	if(i == 5){
	lib->setBit(secondByte, i, 1);
	}
	lib->setBit(secondByte, i, 0);
	}

	requestPayload.push_back(firstByte);
	requestPayload.push_back(secondByte);
	requestPayload.push_back(thirdByte);
	requestPayload.push_back(fourthByte);
	*/

	return requestPayload;
}

//in the case of type 7
//A peer should respond to a Request message with contents of piece
std::vector<OURBYTE> Message::GeneratePiecePayload(int pieceIndex) {
	//4 byte piece index
	//4 byte block offset
	//variable length block containing the raw bytes for the requested piece.
	//The length of this should be the same as the length requested.
	//which we used 2^14 power


	//add in the piece index
	std::vector<OURBYTE> piecePayload;
	std::vector<int> bitRepresentation;
	bitRepresentation = convert(pieceIndex);

	//below the payload has the 4 byte piece index
	piecePayload = lib->convertBitsToBytes(bitRepresentation);

	/*
	//next we need to generate the 4byte offset...
	//but how do we determine the offset?
	//NEEDS TO BE IMPLEMENTED, FOR NOW ALL ZEROS

	OURBYTE first;
	OURBYTE second;
	OURBYTE third;
	OURBYTE fourth;

	for(int i = 0; i < 8; i++){
	lib->setBit(first, i, 0);
	lib->setBit(second, i, 0);
	lib->setBit(third, i, 0);
	lib->setBit(fourth, i, 0);
	}

	piecePayload.push_back(first);
	piecePayload.push_back(second);
	piecePayload.push_back(third);
	piecePayload.push_back(fourth);
	*/

	//now we need to give the 4 byte block length
	//because it said to make it same length as what was requested
	//intenet reccomends 2^14 power
	//which is 00000000 00000100 00000000 00000000

	OURBYTE firstByte;
	OURBYTE secondByte;
	OURBYTE thirdByte;
	OURBYTE fourthByte;

	for (int i = 0; i < 8; i++) {
		lib->setBit(firstByte, i, 0);
		lib->setBit(thirdByte, i, 0);
		lib->setBit(fourthByte, i, 0);
	}
	for (int i = 0; i < 8; i++) {
		if (i == 5) {
			lib->setBit(secondByte, i, 1);
		}
		lib->setBit(secondByte, i, 0);
	}

	piecePayload.push_back(firstByte);
	piecePayload.push_back(secondByte);
	piecePayload.push_back(thirdByte);
	piecePayload.push_back(fourthByte);

	return piecePayload;
}

//End of ActualMessage Code

//Start of Handshake Code

HandshakeMessage::HandshakeMessage(int _peerID)
{
	std::vector<OURBYTE> handshakeHeader; //18 bytes
	std::vector<OURBYTE> zeroBytes; //10 zero bytes
	std::vector<OURBYTE> peerID; //4 bytes
								 //�P2PFILESHARINGPROJ'
	fullHandshakeMessage.reserve(32);
	lib = new utilLib();
	handshakeHeader = lib->GetByteStreamFromString("P2PFILESHARINGPROJ");

	peerID = lib->GetByteStreamFromInt(_peerID);
	GenerateHandshakeMessageByteStream(handshakeHeader, peerID);
	//std::cout << "full handshake message: " << lib->GetStringFromByteStream(fullHandshakeMessage) << std::endl;
}
void HandshakeMessage::GenerateHandshakeMessageByteStream(std::vector<OURBYTE> header, std::vector<OURBYTE> peerID)
{
	//header
	for (int i = 0; i < 18; i++)
	{
		OURBYTE currentByte;
		currentByte = header[i];
		fullHandshakeMessage.push_back(currentByte);
	}
	//zero bits
	for (int i = 0; i < 10; i++)
	{
		OURBYTE zero;
		fullHandshakeMessage.push_back(zero);
	}
	//peer ID
	for (int i = 0; i < 4; i++)
	{
		OURBYTE currentByte;
		currentByte = peerID[i];
		fullHandshakeMessage.push_back(currentByte);
	}
}

std::vector<OURBYTE> HandshakeMessage::GetHandshakeMessageByteStream()
{
	return fullHandshakeMessage;
}
char * HandshakeMessage::GetHandshakeMessageAsString()
{
	return lib->GetStringFromByteStream(fullHandshakeMessage);
}



