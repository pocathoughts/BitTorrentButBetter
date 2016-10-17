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
#include <boost/multiprecision/cpp_int.hpp> //for uint256_t


class Message
{
private:
    BYTE messageType;
    BYTE payload;
    int messageLength;
    
public:
    Message(byte messageType, byte[] messagePayload);
    uint256_t CreateHandshakeMessage(uint32_t peerID);
    uint32_t GenerateEmptyPayload();
    Peer SendHandshakeMessage(Peer otherPeer);
    Peer SendActualMessage(MessageType type);
    uint256_t GenerateMessage(uint32_t messageLength, MessageType type, unit32_t payload);
    bool hasPayload();
    
    // i have these listed as ints bu they need to be bytes
    int getMessageType();
    int getPayload();
};

// message type enum for reference
enum MessageType {
    
    CHOKE,
    UNCHOKE,
    INTERESTED,
    not_interested,
    have,
    bitfield,
    request,
    piece
    
};


#endif /* Message_hpp */
