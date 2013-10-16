#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Structs.h"
//#include "PathFinding.h"

pos::pos(char x, char y) {
	this->x = x;
	this->y = y;
}

pos::pos() {
	
}

pos pos::operator-(pos other) const {
	pos p;
	p.x = x-other.x;
	p.y = y-other.y;
	return p;
}
pos pos::operator+(pos other) const {
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
pos pos::operator*(char factor) const {
	pos p;
	p.x = x*factor;
	p.y = y*factor;
	return p;
}
bool pos::operator<(pos other) const {
	if (x == other.x)
		return y-other.y;
    else
    	return x-other.x;
}
bool pos::operator==(pos other) const {
	return (x == other.x && y == other.y);
}


bool boxMove::operator<(boxMove other) const {
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


posScore::posScore(int x,int y) {
	this->x = x;
	this->y = y;
}
	
posScore::posScore(int x, int y, int score) {
	this->x = x;
	this->y = y;
	this->score = score;
}

bool posScore::operator<(const posScore& other) const {
	return score > other.score;
}

