#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <bitset>
#include "Structs.h"
#include "Constants.h"
//#include "PathFinding.h"


pos::pos(char x, char y) {
	this->x = x;
	this->y = y;
}

pos::pos() {
	
}

pos pos::operator-(const pos& other) const {
	pos p;
	p.x = x-other.x;
	p.y = y-other.y;
	return p;
}
pos pos::operator+(const pos& other) const {
    pos p;
	p.x = x+other.x;
	p.y = y+other.y;
	return p;
}
pos pos::operator-() const {
	pos p;
	p.x = -x;
	p.y = -y;
	return p;
}
pos pos::operator*(const char factor) const {
	pos p;
	p.x = x*factor;
	p.y = y*factor;
	return p;
}
bool pos::operator<(const pos& other) const {
	if (x == other.x)
		return y-other.y;
    else
    	return x-other.x;
}
bool pos::operator==(const pos& other) const {
	return (x == other.x && y == other.y);
}



bool boxMove::operator<(const boxMove& other) const {
	if(start.x == other.start.x){
        if(start.y == other.start.y){
            if(end.x == other.end.x){
                return end.y-other.end.y;
            }else{
                return end.x-other.end.x;
            }
        }else{
            return start.y-other.end.y;
        }
    }else{
        return start.x-other.end.x;
    }
}



posScore::posScore(char x, char y) {
	this->x = x;
	this->y = y;
}
	
posScore::posScore(char x, char y, int score) {
	this->x = x;
	this->y = y;
	this->score = score;
}

bool posScore::operator<(const posScore& other) const {
	return score > other.score;
}



//bitString
//FIRSTBIT is the number that has a binary representation of
//a one followed by 31 zeroes.
//Constructor
//Construct a vector with capacity at least 'size' bits
bitString::bitString(int size) {
	data = std::vector<int>();
	data.reserve(size/sizeof(int)+1);
}

//Functions
//Get the bit at row i, column j in the matrix
bool bitString::get(int i, int j) {
	int arrIndex = i*NR_ROWS+j;
	return get();
}

bool bitString::get(int i) {
	int vecIndex = i / sizeof(int);
	int intIndex = i % sizeof(int);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	return data[vecIndex] & bitmask;
}

//Get the bit at row i, column j in the matrix. Return def if out of bounds
bool bitString::getSafe(int i, int j, bool def) {
	if (i < 0 || i > NR_COLUMNS || j < 0 || j > NR_ROWS) {
		return def;
	} else {
		return get(i, j);
	}
}

//Insert a 32-bit integer at the back
void bitString::insert32(int value) {
	data.push_back(value);
}

//Set the bit at row i, column j in the matrix to 1
void bitString::set(int i, int j) {
	int arrIndex = i*NR_ROWS+j;
	int vecIndex = arrIndex / sizeof(int);
	int intIndex = arrIndex % sizeof(int);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	data[vecIndex] = data[vecIndex] | bitmask;
}

//Set the bit at row i, column j in the matrix to 0
void bitString::reset(int i, int j) {
	int arrIndex = i*NR_ROWS+j;
	int vecIndex = arrIndex / sizeof(int);
	int intIndex = arrIndex % sizeof(int);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	bitmask = ~bitmask;	//Invert bits
	data[vecIndex] = data[vecIndex] & bitmask;
}


//Operators

//Bitwise AND
bitString bitString::operator&(const bitString& other) const {
	bitString res = bitString(data.size());
	for (int i = 0;i<data.size();i++) {
		res.data[i] = data[i] & other.data[i];
	}
}

//Bitwise OR
bitString bitString::operator|(const bitString& other) const {
	bitString res = bitString(data.size());
	for (int i = 0;i<data.size();i++) {
		res.data[i] = data[i] | other.data[i];
	}
}

//Bitwise XOR
bitString bitString::operator^(const bitString& other) const {
	bitString res = bitString(data.size());
	for (int i = 0;i<data.size();i++) {
		res.data[i] = data[i] ^ other.data[i];
	}
}

//Bitwise NOT
bitString bitString::operator~() const {
	bitString res = bitString(data.size());
	for (int i = 0;i<data.size();i++) {
		res.data[i] = ~data[i];
	}
}

//Less-than comparator
bool bitString::operator<(const bitString& other) const {
	for (int i = 0;i<data.size();i++) {
		if (data[i] < other.data[i]) {
			return true;
		} else if (data[i] > other.data[i]) {
			return false;
		} else {
			continue;
		}
	}
	//If we are here, they are equal
	return false;
}

//Greater-than comparator
bool bitString::operator>(const bitString& other) const {
	for (int i = 0;i<data.size();i++) {
		if (data[i] > other.data[i]) {
			return true;
		} else if (data[i] < other.data[i]) {
			return false;
		} else {
			continue;
		}
	}
	//If we are here, they are equal
	return false;
}

//Equals
bool bitString::operator==(const bitString& other) const {
	for (int i = 0;i<data.size();i++) {
		if (data[i] == other.data[i]) {
			continue;
		} else {
			return false;
		}
	}
	//If we are here, they are equal
	return true;
}

ostream& operator<<(ostream &strm, const bitString& bs) {
    std::ostream& stream = strm;
    bitset<sizeof(int)> bitRepr;
    int i;
    for (i = 0;i<data.size()-1;i++) {
    	bitRepr = bitset<sizeof(int)>(data[i]);
    	stream << bitRepr;
    }
    for (int j = i*sizeof(int);j<BOARD_SIZE;j++) {
    	stream << (int)(bs.get(j));
    }
    stream << endl;
    return stream;
}





