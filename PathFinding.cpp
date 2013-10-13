#include <stack>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "PathFinding.h"
#include "Constants.h"

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

	int h = gs->board.size();
	int w = gs->board[0].size();
	
	pos startPos, endPos;
	startPos = gs->player;	
	endPos = bm.start*2-bm.end;
	
	std::vector<dirEntry> directions;
	directions.push_back(dirEntry(pos(0,-1),0));
	directions.push_back(dirEntry(pos(1, 0),0));
	directions.push_back(dirEntry(pos(0, 1),0));
	directions.push_back(dirEntry(pos(-1,0),0));
	
	vector<vector<char> > dirMap = gs->board;
	
	dirMap[startPos.y][startPos.x] = 'S';
	
	bool goalReached = false;
	
	stack<pos> q;
	q.push(startPos);
	
	//Search the graph
	while (!q.empty()) {
	    pos curPos = q.top();
	    q.pop();
	    
	    /*
	    fprintf(stderr, "dirMap:\n");
		for (int i = 0;i<dirMap.size();i++) {
			for (int j = 0;j<dirMap[i].size();j++) {
				if (endPos.x == j && endPos.y == i) {
					fprintf(stderr, "G");
				} else {
					fprintf(stderr, "%c", dirMap[i][j]);
				}
				
			}
			fprintf(stderr, "\n");
		}*/
	    
	    if (curPos == endPos) {
			//Goal reached!
			//fprintf(stderr, "Entered if statement, as the goal was found!\n");
			goalReached = true;
			break;
	    }
	    
	    pos d;
	    char dir;

		for (int i = 0;i<4;i++) {
			directions[i].weight = (endPos.x-curPos.x)*directions[i].p.x + (endPos.y-curPos.y)*directions[i].p.y;
			directions[i].weight *= (-1);
		}

	    std::sort(directions.begin(), directions.end());
	    
	    for (int i = 0;i<4;i++) {
	    	d = directions[i].p;
	        dir = dirs(d);
	        
	        //Check if visited or unreachable
	        char a = dirMap[curPos.y+d.y][curPos.x+d.x];
	        
	        if ((a == FREE || a == GOAL || a == DEADLOCK)) { //If space is free
	            //Visit
	            dirMap[curPos.y+d.y][curPos.x+d.x] = dirs(d);
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        }
	    }
	}
	
	if (!goalReached) {
	    //Invalid move
	    std::vector<char> ret;
	    ret.push_back('X');
		return ret;
	}	
	
	
	//Else
	char finalMove = dirs(bm.start-endPos);
	pos curPos = endPos;
	char nd = dirMap[endPos.y][endPos.x];
	std::vector<char> path;
	path.push_back(finalMove);
	while (nd != 'S') {
		//fprintf(stderr, "nd = %c\n", nd);
	    path.push_back(nd);
	    pos pnd = direction(nd);
	    curPos = pos(curPos.x-pnd.x, curPos.y-pnd.y);
	    nd = dirMap[curPos.y][curPos.x];
	}
	
	std::reverse(path.begin(),path.end());
	//Or just let main print them in reverse order.
	
	return path;
}

