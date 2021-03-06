#include <stack>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "PathFinding.h"

#ifndef _PATHFINDING_CPP__
#define _PATHFINDING_CPP__

using namespace std;

pos direction(char ch) {
    switch (ch) {
        case 'U':
            return pos(0,-1);
            break;
        case 'R':
			return pos(1,0);
            break;
        case 'D':
			return pos(0,1);
            break;
        case 'L':
			return pos(-1,0);
            break;
        default:
            return pos(42,42);     //This shouldn't be possible!
            break;
    }
}

char dirs(pos p) {
    if (p.x == 0 && p.y == -1) {
    	return 'U';
    } else if (p.x == 1 && p.y == 0) {
    	return 'R';
    } else if (p.x == 0 && p.y == 1) {
    	return 'D';
    } else if (p.x == -1 && p.y == 0) {
    	return 'L';
    } else {
		return 'A';
	}
}

std::vector<char> moveToPath (GameState * gs, boxMove bm) {

	int h = gs->map->getHeight();
	int w = gs->map->getWidth();
	
	pos startPos, endPos;
	startPos = gs->player;	
	endPos = bm.start*2-bm.end;

	
	std::vector<dirEntry> directions;
	directions.push_back(dirEntry(pos(0,-1),0));
	directions.push_back(dirEntry(pos(1, 0),0));
	directions.push_back(dirEntry(pos(0, 1),0));
	directions.push_back(dirEntry(pos(-1,0),0));
	
	vector<vector<char> > dirMap;
	
	for (int i=0;i<h;i++) {
        vector<char> line;
        for (int j=0;j<w;j++) {
			line.push_back('-');
        }
        dirMap.push_back(line);
    }
	
	dirMap[startPos.y][startPos.x] = 'S';
	
	bool goalReached = false;
	
	stack<pos> q;
	q.push(startPos);
	
	//Search the graph
	while (!q.empty()) {
	    pos curPos = q.top();
	    q.pop();
	    
	    pos d;
	    char dir;

	    std::sort(directions.begin(), directions.end());
	    
	    for (int i = 0;i<4;i++) {
	    	d = directions[i].p;
	        dir = dirs(d);
	        
	        //Check if visited or unreachable
	        char a = dirMap[curPos.y+d.y][curPos.x+d.x];
	        
	        if (a == '-' && (gs->boxes.end() == gs->boxes.find(pos(curPos.x+d.x,curPos.y+d.y))) && 
                        !gs->map->isWall(pos(curPos.x+d.x,curPos.y+d.y))) { //If space is free
	            //Visit
	            dirMap[curPos.y+d.y][curPos.x+d.x] = dirs(d);
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        } else if (a == '.') {
	            //Goal reached!
	            dirMap[curPos.y+d.y][curPos.x+d.x] = dirs(d);
	            goalReached = true;
	            endPos = pos(curPos.x+d.x,curPos.y+d.y);
	            break;
	        }
	    }
	}
	
	if (!goalReached) {
	    //Invalid move
	    return std::vector<char>('X');
	}	
	
	
	//Else
	char finalMove = dirs(endPos*2-bm.start);
	pos curPos = endPos;
	char nd = dirMap[endPos.y][endPos.x];
	std::vector<char> path;
	path.push_back(finalMove);
	while (nd != 'S') {
	    path.push_back(nd);
	    pos pnd = direction(nd);
	    curPos = pos(curPos.x-pnd.x, curPos.y-pnd.y);
	    nd = dirMap[curPos.y][curPos.x];
	}
	
	std::reverse(path.begin(),path.end());
	//Or just let main print them in reverse order.
	
	return path;
}

#endif
