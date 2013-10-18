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

	pos startPos, endPos;
	startPos = gs->player;	
	endPos = bm.start*2-bm.end;
	
	std::vector<dirEntry> directions;
	directions.push_back(dirEntry(pos(0,-1),0));
	directions.push_back(dirEntry(pos(1, 0),0));
	directions.push_back(dirEntry(pos(0, 1),0));
	directions.push_back(dirEntry(pos(-1,0),0));
	
	vector<vector<char> > dirMap; // = gs->board;
	
	//With bitString, construct a dirMap...
	dirMap = vector<vector<char> >();
	vector<char> inner;
	dirMap.reserve(NR_ROWS);
	for (int i = 0;i<NR_ROWS;i++) {
		inner = vector<char>(NR_COLUMNS, ' ');
		dirMap.push_back(inner);
	}
	
	bitString boxes = gs->boxes;
	bitString obstacles = boxes | WALLS;

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
	        
	        if (a == FREE && !obstacles.get(curPos.y+d.y,curPos.x+d.x)) { //If space is free
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


//new
bool pathExistsAStar(const pos& p1, const pos& p2, GameState * gs) {
	priority_queue<posScore> q;
	//vector<vector<char> > dirMap = board;
	bitString visitedList = GOALS;
	visitedList.clear();
	bitString obstacles = gs->boxes | WALLS;
	
	//dirMap[p1.y][p1.x] = 'V';
	visitedList.set(p1.y,p1.x);
	
	posScore start(p1.x,p1.y,aStarDist(p1,p2,p1));
	q.push(start);

	while(!q.empty()) {
		posScore next = q.top();
		q.pop();
		
		//cerr << "obstacles: " << obstacles << endl;
		//cerr << "visitedList: " << visitedList << endl;

		if(next.x == p2.x && next.y == p2.y) {
			//fprintf(stderr, "Returned true from A*\n");
			return true;
		}

		posScore m1(next.x,next.y-1);
		posScore m2(next.x,next.y+1);
		posScore m3(next.x+1,next.y);
		posScore m4(next.x-1,next.y);
		
		if(!obstacles.get(m1.y,m1.x) && !visitedList.get(m1.y,m1.x)) {
			m1.score = aStarDist(m1,p2,start);
			q.push(m1);
			//dirMap[m1.y][m1.x] = 'V';
			visitedList.set(m1.y, m1.x);
		}
		if(!obstacles.get(m2.y,m2.x) && !visitedList.get(m2.y,m2.x)) {
			m2.score = aStarDist(m2,p2,start);
			q.push(m2);
			//dirMap[m2.y][m2.x] = 'V';
			visitedList.set(m2.y, m2.x);
		}
		if(!obstacles.get(m3.y,m3.x) && !visitedList.get(m3.y,m3.x)) {
			m3.score = aStarDist(m3,p2,start);
			q.push(m3);
			//dirMap[m3.y][m3.x] = 'V';
			visitedList.set(m3.y, m3.x);
		}
		if(!obstacles.get(m4.y,m4.x) && !visitedList.get(m4.y,m4.x)) {
			m4.score = aStarDist(m4,p2,start);
			q.push(m4);
			//dirMap[m4.y][m4.x] = 'V';
			visitedList.set(m4.y, m4.x);
		}

	}

	//fprintf(stderr, "Returned false from A*\n");
	return false;

}

/*
bool pathExistsAStar(const pos& p1,const pos& p2,const vector<vector<char> >& board) {
	priority_queue<posScore> q;
	vector<vector<char> > dirMap = board;
	dirMap[p1.y][p1.x] = 'V';
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
			dirMap[m1.y][m1.x] = 'V';
		}
		c = dirMap[m2.y][m2.x];
		if(IS_PASSABLE_FP(c)) {
			m2.score = aStarDist(m2,p2,start);
			q.push(m2);
			dirMap[m2.y][m2.x] = 'V';
		}
		c = dirMap[m3.y][m3.x];
		if(IS_PASSABLE_FP(c)) {
			m3.score = aStarDist(m3,p2,start);
			q.push(m3);
			dirMap[m3.y][m3.x] = 'V';
		}
		c = dirMap[m4.y][m4.x];
		if(IS_PASSABLE_FP(c)) {
			m4.score = aStarDist(m4,p2,start);
			q.push(m4);
			dirMap[m4.y][m4.x] = 'V';
		}

	}

	return false;

}
*/

inline int aStarDist(const pos& p1,const pos& p2,const  pos& start) {
	int f = abs(p1.x-p2.x) + abs(p1.y-p2.y);
	int h = abs(p1.x-start.x) + abs(p1.y-start.y);
	return f + (h>>1);
}

inline int aStarDist(const posScore& p1,const pos& p2,const posScore& start) {
	int f = abs(p1.x-p2.x) + abs(p1.y-p2.y);
	int h = abs(p1.x-start.x) + abs(p1.y-start.y);
	return f + (h>>1);
}


