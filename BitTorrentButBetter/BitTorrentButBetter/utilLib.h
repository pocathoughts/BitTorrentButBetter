#pragma once
#include <bitset>

class Byte {
	typedef std::bitset<8> BYTE;
public:
	Byte();
	~Byte();
	BYTE byte; //all initialized to 0

	//calling setByte(2,1) makes //00000000 into //00000100
	void setByte(int byteNum, bool val) {
		byte.set(byteNum, val);
	}
private:

};
class utilLib
{
public:
	utilLib();
	~utilLib();

	typedef std::bitset<8> BYTE;
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
};

