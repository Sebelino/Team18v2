#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <bitset>
#include <sstream>
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
//Constructors

bitString::bitString(char w, char h) {
	width = w;
	height = h;
}

bitString::bitString() {
	width = WORD_SIZE;
	height = WORD_SIZE;
}

bitString::bitString(const bitString& src) {
	data = src.data;
	width = src.width;
	height = src.height;
}


//Functions
void bitString::reserve(int cap) {
	data.reserve(cap);
}

//Alter the dimension sizes
void bitString::resize(int w, int h) {
	width = w;
	height = h;
}

//Get the bit at position i
bool bitString::get(int i) {
	int vecIndex = i / (WORD_SIZE);
	int intIndex = i % (WORD_SIZE);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	return data[vecIndex] & bitmask;
}

//Get the bit at row i, column j in the matrix
bool bitString::get(int i, int j) {
	int arrIndex = i*width+j;
	return get(arrIndex);
}

//Get the bit at row i, column j in the matrix. Return def if out of bounds
bool bitString::getSafe(int i, int j, bool def) {
	if (i < 0 || i >= height || j < 0 || j >= width) {
		return def;
	} else {
		return get(i, j);
	}
}

//Insert a 32-bit integer at the back
void bitString::insert32(unsigned int value) {
	data.push_back(value);
}

//Set the bit at row i, column j in the matrix to 1
void bitString::set(int i, int j) {
	int arrIndex = i*width+j;
	int vecIndex = arrIndex / (WORD_SIZE);
	int intIndex = arrIndex % (WORD_SIZE);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	data[vecIndex] = data[vecIndex] | bitmask;
}

//Set the bit at row i, column j in the matrix to 0
void bitString::reset(int i, int j) {
	int arrIndex = i*width+j;
	int vecIndex = arrIndex / (WORD_SIZE);
	int intIndex = arrIndex % (WORD_SIZE);
	unsigned int bitmask = FIRSTBIT >> intIndex;
	bitmask = ~bitmask;	//Invert bits
	data[vecIndex] = data[vecIndex] & bitmask;
}

//Set the bit at row i, column j in the matrix to val
void bitString::setTo(int i, int j, bool val) {
	if (val) {
		set(i,j);
	} else {
		reset(i,j);
	}
}

//Set all bits to 0
void bitString::clear() {
	for (int i = 0;i<data.size();i++) {
		data[i] = 0;
	}
}

int bitString::sum() {
	int sum = 0;
	int size = width*height;
	for (int i = 0;i<size;i++) {
		sum += get(i);
	}
	return sum;
}


//Operators

//Bitwise AND
bitString bitString::operator&(const bitString& other) const {
	bitString res = bitString(width,height);
	for (int i = 0;i<data.size();i++) {
		res.data.push_back(data[i] & other.data[i]);
	}
	return res;
}

//Bitwise OR
bitString bitString::operator|(const bitString& other) const {
	bitString res = bitString(width,height);
	for (int i = 0;i<data.size();i++) {
		res.data.push_back(data[i] | other.data[i]);
	}
	return res;
}

//Bitwise XOR
bitString bitString::operator^(const bitString& other) const {
	bitString res = bitString(width,height);
	for (int i = 0;i<data.size();i++) {
		res.data.push_back(data[i] ^ other.data[i]);
	}
	return res;
}

//Bitwise NOT
bitString bitString::operator~() const {
	bitString res = bitString(width,height);
	for (int i = 0;i<data.size();i++) {
		res.data.push_back(~data[i]);
	}
	return res;
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

bitString& bitString::operator=(const bitString& src) {
	data = src.data;
	width = src.width;
	height = src.height;
	return *this;
}


inline int ilog2(register unsigned int x) {
    register unsigned int l=0;
    if(x >= 1<<16) { x>>=16; l|=16; }
    if(x >= 1<<8) { x>>=8; l|=8; }
    if(x >= 1<<4) { x>>=4; l|=4; }
    if(x >= 1<<2) { x>>=2; l|=2; }
    if(x >= 1<<1) l|=1;
    return l;
}

std::vector<pos> bitString::getPosVector(int am) {
	std::vector<pos> res = std::vector<pos>();
	res.reserve(am);
	for (int i = 0;i<data.size();i++) {
		unsigned int curInt = data[i];
		for (int n = 0; curInt != 0; n++, curInt &= (curInt - 1)) {
			int log = ilog2(curInt & ~(curInt-1));
			int linearPos = i*WORD_SIZE+WORD_SIZE-log-1;
			res.push_back(pos(linearPos/width, linearPos % width));
		}
	}
	return res;
}

std::string bitString::toString() {
    std::string output;
    output.reserve(data.size()*(WORD_SIZE+1));
    
    std::bitset<WORD_SIZE> bitRepr;
    int size = width*height;
    int i;
    for (i = 0;i<data.size()-1;i++) {
    	bitRepr = std::bitset<WORD_SIZE>(data[i]);
    	output.append(bitRepr.to_string());
    }
    for (int j = i*WORD_SIZE;j<size;j++) {
    	output.push_back((char)(get(j)+'0'));
    }
    
    for (i = width;i<output.length();i+=(width+1)) {
    	output.insert(i,"\n");
    }

    return output;
}

std::ostream& operator<<(std::ostream &strm, bitString& bs) {
    std::ostream& stream = strm;
    
    std::string output;
    output.reserve(bs.data.size()*(WORD_SIZE+1));
    
    std::bitset<WORD_SIZE> bitRepr;
    int size = bs.width*bs.height;
    int i;
    for (i = 0;i<bs.data.size()-1;i++) {
    	bitRepr = std::bitset<WORD_SIZE>(bs.data[i]);
    	output.append(bitRepr.to_string());
    }
    for (int j = i*WORD_SIZE;j<size;j++) {
    	output.push_back((char)(bs.get(j)+'0'));
    }
    
    for (i = bs.width;i<output.length();i+=(bs.width+1)) {
    	output.insert(i,"\n");
    }
    
    stream << output << std::endl;
    
    return stream;
}






