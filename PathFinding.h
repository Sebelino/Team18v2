#ifndef _PATHFINDING_H__
#define _PATHFINDING_H__

#include <stack>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "GameState.h"

class GameState;

struct dirEntry {
	pos p;
	int weight;
	bool operator<(dirEntry other) const {
		return weight > other.weight;
	}

	dirEntry(pos p, int weight) {
		this->p = p;
		this->weight = weight;
	}
};

pos direction(char ch);
char dirs(pos p);
std::vector<char> moveToPath (GameState * gs, boxMove bm);
bool pathExists(pos p1,pos p2,std::vector<std::vector<char> > board);
std::vector<char> findPath(pos p1,pos p2,std::vector<std::vector<char> > b);

#endif

