#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Structs.h"
//#include "PathFinding.h"


pos pos::operator-(pos other) const {
	return {x-other.x, y-other.y};
}
pos pos::operator+(pos other) const {
	return {x+other.x, y+other.y};
}
pos pos::operator-() const {
	return {-x, -y};
}
pos pos::operator*(int factor) const {
	return {x*factor, y*factor};
}
bool pos::operator<(pos other) const {
	if (x == other.x)
		return y-other.y;
    else
    	return x-other.x;
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

