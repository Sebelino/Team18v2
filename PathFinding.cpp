#include <stack>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "PathFinding.h"
#include "Constants.h"
#include "GameState.h"

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
	int h = gs->height;
	int w = gs->width;
	
	pos startPos, endPos;
	startPos = gs->player;	
	endPos = bm.start*2-bm.end;
	
	std::vector<dirEntry> directions;
	directions.push_back(dirEntry(pos(0,-1),0));
	directions.push_back(dirEntry(pos(1, 0),0));
	directions.push_back(dirEntry(pos(0, 1),0));
	directions.push_back(dirEntry(pos(-1,0),0));
	
	vector<vector<char> > dirMap = gs->getCopyOfBoard();
	
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

/** Returns the path between p1 and p2 on board b, specified by letters.
 *  Returns ['X'] if no such path exists. */
std::vector<char> findPath(pos p1,pos p2,GameState& gs) {
	int h = gs.height;
	int w = gs.width;
	
	std::vector<dirEntry> directions;
	directions.push_back(dirEntry(pos(0,-1),0));
	directions.push_back(dirEntry(pos(1, 0),0));
	directions.push_back(dirEntry(pos(0, 1),0));
	directions.push_back(dirEntry(pos(-1,0),0));
	
	vector<vector<char> > dirMap = gs.getCopyOfBoard();
	
	dirMap[p1.y][p1.x] = 'S';
	
	bool goalReached = false;
	
	stack<pos> q;
	q.push(p1);
	
	//Search the graph
	while (!q.empty()) {
	    pos curPos = q.top();
	    q.pop();
	    
	    if (curPos == p2) {
			//Goal reached!
			//fprintf(stderr, "Entered if statement, as the goal was found!\n");
			goalReached = true;
			break;
	    }
	    
	    pos d;
	    char dir;

		for (int i = 0;i<4;i++) {
			directions[i].weight = (p2.x-curPos.x)*directions[i].p.x + (p2.y-curPos.y)*directions[i].p.y;
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
	pos curPos = p2;
	char nd = dirMap[p2.y][p2.x];
	std::vector<char> path;
	while (nd != 'S') {
	    path.push_back(nd);
	    pos pnd = direction(nd);
	    curPos = pos(curPos.x-pnd.x, curPos.y-pnd.y);
	    nd = dirMap[curPos.y][curPos.x];
	}
	std::reverse(path.begin(),path.end());
	return path;
}

bool pathExists(pos p1,pos p2,GameState& gs){
    vector<char> path = findPath(p1,p2,gs);
    if(path.size() >= 1 && path[0] == 'X'){
        return false;
    }
    return true;
}

bool pathExistsAStar(const pos& p1,const pos& p2, GameState& gs) {
	priority_queue<posScore> q;
	vector<vector<char> > dirMap = gs.getCopyOfBoard();
	dirMap[p1.y][p1.x] = 'v';
	posScore start(p1.x,p1.y,aStarDist(p1,p2,p1));
	q.push(start);

	while(!q.empty()) {
		posScore next = q.top();
		q.pop();

		if(next.x == p2.x && next.y == p2.y)
			return true;

		posScore m1(next.x,next.y-1);
		posScore m2(next.x,next.y+1);
		posScore m3(next.x+1,next.y);
		posScore m4(next.x-1,next.y);
		char c = dirMap[m1.y][m1.x];
		if(IS_PASSABLE_FP(c)) {
			m1.score = aStarDist(m1,p2,start);
			q.push(m1);
			dirMap[m1.y][m1.x] = 'v';
		}
		c = dirMap[m2.y][m2.x];
		if(IS_PASSABLE_FP(c)) {
			m2.score = aStarDist(m2,p2,start);
			q.push(m2);
			dirMap[m2.y][m2.x] = 'v';
		}
		c = dirMap[m3.y][m3.x];
		if(IS_PASSABLE_FP(c)) {
			m3.score = aStarDist(m3,p2,start);
			q.push(m3);
			dirMap[m3.y][m3.x] = 'v';
		}
		c = dirMap[m4.y][m4.x];
		if(IS_PASSABLE_FP(c)) {
			m4.score = aStarDist(m4,p2,start);
			q.push(m4);
			dirMap[m4.y][m4.x] = 'v';
		}

	}

	return false;

}

int aStarDist(const pos& p1,const pos& p2,const  pos& start) {
	int f = abs(p1.x-p2.x) + abs(p1.y-p2.y);
	int h = abs(p1.x-start.x) + abs(p1.y-start.y);
	return f + (h>>1);
}

int aStarDist(const posScore& p1,const pos& p2,const posScore& start) {
	int f = abs(p1.x-p2.x) + abs(p1.y-p2.y);
	int h = abs(p1.x-start.x) + abs(p1.y-start.y);
	return f + (h>>1);
}


