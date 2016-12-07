#pragma once
#include <bitset>
#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>
typedef std::bitset<8> OURBYTE;
class Byte {
public:
	Byte();
	~Byte();


private:
};
class utilLib
{
public:
	int neighborList[5];
	void setNeighborList(int list[5]);
	utilLib();
	~utilLib();

	void reglog(int logtype, char * time, int peer_ID, int peer2_ID);

	void pnlog(char * time, int peer_ID);

	void hplog(char * time, int peer_ID, int peer2_ID, int index);

	void dplog(char * time, int peer_ID, int peer2_ID, int index, int num_Pieces);

	void cdlog(char * time, int peer_ID);

	char * getTime();

	typedef std::bitset<8> OURBYTE;
	enum MessageType
	{
		CHOKE, //0
		UNCHOKE,//1
		INTERESTED, //2
		NOTINTERESTED, //3
		HAVE, //4
		BITFIELD, //5
		REQUEST, //6
		PIECE //7
	};//
	int GetMessageTypeFromStream(std::vector<OURBYTE> stream)
	{
		//it's in the 5th byte
		//printByteStream(a);
		int val = 0;
		OURBYTE inv = (stream[4]);
		val +=  (int)inv.to_ulong();
		return val;
	}
	//calling setByte(2,1) makes //00000000 into //00000100
	void setBit(OURBYTE &byte, int bitNum, bool val)
	{
		byte.set(bitNum, val);
		
	}


	void setAllBitsTo1(OURBYTE &byte)
	{
		byte.set(0, 1);
		byte.set(1, 1);
		byte.set(2, 1);
		byte.set(3, 1);
		byte.set(4, 1);
		byte.set(5, 1);
		byte.set(6, 1);
		byte.set(7, 1);
	}
	//used for integer calcs
	OURBYTE reverseBitsInByte(OURBYTE byte)
	{
		OURBYTE rev;
		for (int i = 0; i < 8; i++)
		{
			rev[i] = byte[7 - i];
		}
		return rev;
	}
	OURBYTE GetByteFromChar(char c)
	{
		OURBYTE b(c);
		return b;  // implicit cast to unsigned long long
	}
	std::vector<OURBYTE> GetByteStreamFromString(char * str)
	{
		//std::cout << "called byte stream to string\n";
		std::vector<OURBYTE> word;
		word.reserve(33);
		for (int i = 0; i < strlen(str); i++)
		{
			//sneaky catch for 0s
			if (str[i] == '$')
			{
				OURBYTE b(0); //was b
				std::cout << "is this the problem\n";
				word.push_back(b);
				continue;
			}
			OURBYTE b(str[i]);
			std::cout << "or is it this: ";
			word.push_back(b);
			//std::cout << word.size() << std::endl;
		}
		return word;
	}
	char GetCharFromByte(OURBYTE b)
	{
		unsigned long i = b.to_ulong();
		//if (i <= CHAR_MAX)
		return static_cast<char>(i);
		return NULL;
	}

	//try to avoid this it throws seg faults sometimes but not all the time dont ask me lawl
	char * GetStringFromByteStream(std::vector<OURBYTE> stream)
	{
		char * str = new char[43]; //could be a long stream
		int index = 0;
		OURBYTE b;
		for (std::vector<OURBYTE>::iterator i = stream.begin(); i < stream.end(); i++)
		{
			char c = GetCharFromByte((*i));
			str[index] = c;
			//sneaky catch for 0s
			if (c == 0)
			{
				str[index] = '$';
			}
			index++;
		}
		str[index] = '\0';
		return str;
	}
	int GetIntFromByteStream(std::vector<OURBYTE> stream)
	{
		int val = 0;
		int modifier = 1;
		//go backwards
		for (std::vector<OURBYTE>::iterator i = stream.end() - 1; i > stream.begin() - 1; i--)
		{
			OURBYTE inv = reverseBitsInByte((*i));
			val += modifier * (int)inv.to_ulong();
			modifier *= 256;
		}
		return val;
	}
	std::vector<OURBYTE> GetByteStreamFromInt(int s)
	{
		std::vector<OURBYTE> intStream;
		std::bitset<32> integer(s); //this is the int to its raw 32 bit form
		//for each byte
		for (int i = 0; i < 4; i++)
		{
			OURBYTE thisByte;
			//for each bit in said byte
			for (int j = 0; j < 8; j++)
			{
				setBit(thisByte, j, integer[8 * (3 - i) + (7 - j)]);
			}
			intStream.push_back(thisByte);
		}
		return intStream;
	}

	//for debugging
	void printBitsInByte(OURBYTE byte)
	{
		std::cout << ("Bits: ");
		std::cout << byte;
		std::cout << ("\n");
	}
	//for debugging
	void printByteStream(std::vector<OURBYTE> byteStream)
	{
		int index = 0;
		for (std::vector<OURBYTE>::iterator i = byteStream.begin(); i < byteStream.end(); i++)
		{
			std::cout << "Bits in byte ";
			std::cout << index;
			std::cout << " ";
			std::cout << (*i);
			std::cout << ("\n");
			index++;
		}
	}
	//helper function
	//converts the bit representation into bytes
	std::vector<OURBYTE> convertBitsToBytes(std::vector<int> bits) {
		//we should be taking in a bit vector of 32 bits
		std::vector<OURBYTE> bytes;
		OURBYTE firstByte;
		OURBYTE secondByte;
		OURBYTE thirdByte;
		OURBYTE fourthByte;
		int counter = 0;
		for (int i = 0; i < 8; i++) {
			setBit(firstByte, i, bits[counter]);
			counter++;
		}
		for (int i = 0; i < 8; i++) {
			setBit(secondByte, i, bits[counter]);
			counter++;
		}
		for (int i = 0; i < 8; i++) {
			setBit(thirdByte, i, bits[counter]);
			counter++;
		}
		for (int i = 0; i < 8; i++) {
			setBit(fourthByte, i, bits[counter]);
			counter++;
		}
		bytes.push_back(firstByte);
		bytes.push_back(secondByte);
		bytes.push_back(thirdByte);
		bytes.push_back(fourthByte);
		return bytes;
	}
};

