//
//  Message.cpp
//  BitTorrentButBetter
//
//  Created by Christine Moore on 10/17/16.
//  Copyright © 2016 Christine Moore. All rights reserved.
//
//  Obviously this is not a complete file, i tried my best to make
//  this generic enough that anyone could use this
//
//  Also includes handling the first initial handshake message

#include "Message.h"


int getMessageType(){
    return messageType;
}

uint32_t getPayload(){
    return payload;
}


// could be useful
bool hasPayload(){
    return (messageType == HAVE || messageType == BITFIELD
            || messageType == REQUEST || messageType == PIECE);
}


//constructor
Message(BYTE[] payload, byte messageType) {
    this.messagePayload = messagePayload;
    this.messageType = messageType;
    messageLength =  1 + messagePayload.length;
}



EIGHTBYTE CreateHandshakeMessage(uint32_t peerID){


}


Peer SendHandshakeMessage(Peer otherPeer){
    
}

Peer SendActualMessage(MessageType type){

}

EIGHTBYTE GenerateMessage(uint32_t messageLength, MessageType type, uint32_t payload){

}

uint32_t GenerateEmptyPayload(){

}


uint32_t GenerateBitfieldPayload(ListOfPieces pieces){

}


uint32_t GenerateHavePayload(Huh){

}

uint32_t GenerateRequestPayload(Huh){

}

uint32_t GeneratePiecePayload(Huh){

}

