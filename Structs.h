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
    pos operator-(pos other) const;
    pos operator+(pos other) const;
    pos operator-() const;
    pos operator*(char factor) const;
    bool operator<(pos other) const;
    bool operator==(pos other) const;
};

struct boxMove{
	pos start;
	pos end;
    bool operator<(boxMove other) const;
};

#endif

