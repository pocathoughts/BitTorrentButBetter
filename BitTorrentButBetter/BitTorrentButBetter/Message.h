//
//  Message.hpp
//  BitTorrentButBetter
//
//  Created by Christine Moore on 10/17/16.
//  Copyright © 2016 Christine Moore. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp

#include <iostream>
#include <stdio.h>
#include <bitset>
using namespace std;
//#include <boost/multiprecision/cpp_int.hpp> //for uint256_t


class Message
{
typedef bitset<8> BYTE;
typedef bitset<256> EIGHTBYTE;
    
private:
    int messageType;
    uint32_t payload;
    int messageLength;
    
public:
    Message(BYTE messageType, BYTE[] payload);
    EIGHTBYTE CreateHandshakeMessage(uint32_t peerID);
    uint32_t GenerateEmptyPayload();
    Peer SendHandshakeMessage(Peer otherPeer);
    Peer SendActualMessage(MessageType type);
    EIGHTBYTE GenerateMessage(uint32_t messageLength, MessageType type, uint32_t payload);
    bool hasPayload();
    uint32_t GenerateBitfieldPayload(ListOfPieces pieces);
    uint32_t GenerateHavePayload(Huh);
    uint32_t GenerateRequestPayload(Huh);
    uint32_t GeneratePiecePayload(Huh);
    
    int getMessageType();
    uint32_t getPayload();
};

// message type enum for reference
enum MessageType {
    
    CHOKE,
    UNCHOKE,
    INTERESTED,
    NOTINTERESTED,
    HAVE,
    BITFIELD,
    REQUEST,
    PIECE
    
};


#endif /* Message_hpp */
