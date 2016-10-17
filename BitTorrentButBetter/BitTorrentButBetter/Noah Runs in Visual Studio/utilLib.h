#pragma once
#include <bitset>
#include <vector>

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
	OURBYTE GetByteFromChar(int bitNum, char c)
	{
		OURBYTE b(c);
		return b;  // implicit cast to unsigned long long
	}
	std::vector<OURBYTE> GetByteStreamFromString(char * str)
	{
		std::vector<OURBYTE> word;
		for (int i = 0; i < strlen(str); i++)
		{
			OURBYTE b(str[i]);
			word.push_back(b);
		}
		return word;
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
				setBit(thisByte, j, integer[8 * i + j]);
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

