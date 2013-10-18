#ifndef _STRUCTS_H__
#define _STRUCTS_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

struct pos {
    char x;
	char y;
	pos(char x, char y);
	pos();
    pos operator-(const pos& other) const;
    pos operator+(const pos& other) const;
    pos operator-() const;
    pos operator*(const char factor) const;
    bool operator<(const pos& other) const;
    bool operator==(const pos& other) const;
};

struct boxMove{
	pos start;
	pos end;
    bool operator<(const boxMove& other) const;
};

struct posScore {
	char x;
	char y;
	int score;

	posScore(char x,char y);
	posScore(char x, char y, int score);
	bool operator<(const posScore& other) const;
};

struct bitString {
	//Variables
	std::vector<unsigned int> data;

	//Constructor
	bitString(int size);
	bitString();
	bitString(const bitString& src);
	
	//Functions
	bool get(int i);
	bool get(int i, int j);
	bool getSafe(int i, int j, bool def);
	void insert32(unsigned int value);
	void set(int i, int j);
	void reset(int i, int j);
	void setTo(int i, int j, bool val);
	void clear();
	int sum();
	std::vector<pos> getPosVector(int am);
	bitString operator&(const bitString& other) const;
	bitString operator|(const bitString& other) const;
	bitString operator^(const bitString& other) const;
	bitString operator~() const;
	std::string toString();
	bool operator<(const bitString& other) const;
	bool operator>(const bitString& other) const;
	bool operator==(const bitString& other) const;
	bitString& operator=(const bitString& src);
};

inline int ilog2(register unsigned int x);
std::ostream& operator<<(std::ostream &strm, bitString& bs);



#endif

