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

#include "Message.hpp"


BYTE getMessageType(){
    return messageType;
}

BYTE[] getPayload(){
    return payload;
}


// could be useful
bool hasPayload(){
    return (messageType == HAVE || messageType == BITFIELD
            || messageType == REQUEST || messageType == PIECE);
}




//constructor
public Message(byte* payload, byte messageType) {
    this.messagePayload = messagePayload;
    this.messageType = messageType;
    messageLength =  1 + messagePayload.length;
}





public NEEDSTORETURNBYTES makeHandshake(int peerID){
    ByteBuffer message = ByteBuffer.allocate(32);
    
    byte[] hello = ("HELLO").getBytes();
    ByteBuffer zeros = ByteBuffer.allocate(23);
    
    message.put(hello);
    message.put(zeros.array());
    message.putInt(peerid);
    
    return message.array();
}



public
