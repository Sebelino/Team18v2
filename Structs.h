#ifndef _STRUCTS_H__
#define _STRUCTS_H__

#include <iostream>
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
	int x;
	int y;
	int score;

	posScore(int x,int y);
	posScore(int x, int y, int score);
	bool operator<(const posScore& other) const;
};

#endif

