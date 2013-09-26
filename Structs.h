#ifndef _STRUCTS_H__
#define _STRUCTS_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>

struct pos {
    int x;
    int y;
    pos operator-(pos other) const;
    pos operator+(pos other) const;
    pos operator-() const;
    pos operator*(int factor) const;
    bool operator<(pos other) const;
};

struct boxMove{
	pos start;
	pos end;
    bool operator<(boxMove other) const;
};

#endif

