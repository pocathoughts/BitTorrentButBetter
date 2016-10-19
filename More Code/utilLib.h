#pragma once
#include <bitset>
#include <vector>
#include <iostream>
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
	utilLib();
	~utilLib();

	typedef std::bitset<8> OURBYTE;
	enum MessageType
	{
		CHOKE,
		UNCHOKE,
		INTERESTED,
		NOTINTERESTED,
		HAVE,
		BITFIELD,
		REQUEST,
		PIECE
	};
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
	OURBYTE GetByteFromChar(char c)
	{
		OURBYTE b(c);
		return b;  // implicit cast to unsigned long long
	}
	std::vector<OURBYTE> GetByteStreamFromString(char * str)
	{
		std::vector<OURBYTE> word;
		for (int i = 0; i < strlen(str); i++)
		{
			//sneaky catch for 0s
			if (str[i] == '$')
			{
				OURBYTE b;
				word.push_back(b);
				continue;
			}
			OURBYTE b(str[i]);
			word.push_back(b);
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
	char * GetStringFromByteStream(std::vector<OURBYTE> stream)
	{
		char * str = new char[stream.size()]; //could be a long stream
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
				setBit(thisByte, j, integer[8 * (3 - i) + (7-j)]);
			}
			intStream.push_back(thisByte);
		}
		return intStream;
	}

	//for debugging
	void printBitsInByte(OURBYTE byte)
	{
		std::printf("Bits: ");
		std::cout << byte;
		std::printf("\n");
	}
	//for debugging
	void printByteStream(std::vector<OURBYTE> byteStream)
	{
		int index = 0;
		for (std::vector<OURBYTE>::iterator i = byteStream.begin(); i < byteStream.end(); i++)
		{
			std::printf("Bits in byte %d:", index);
			std::cout << (*i);
			std::printf("\n");
			index++;
		}
	}
};

