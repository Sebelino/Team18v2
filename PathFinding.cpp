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
	
	vector<char> dirMap(BOARD_SIZE,' ');

	bitString boxes = gs->boxes;
	bitString obstacles = boxes | WALLS;

	dirMap[startPos.y*NR_COLUMNS+startPos.x] = 'S';
	
	bool goalReached = false;
	
	stack<pos> q;
	q.push(startPos);
	
	//Search the graph
	while (!q.empty()) {
	    pos curPos = q.top();
	    q.pop();
	    
	    if (curPos == endPos) {
			//Goal reached!
			goalReached = true;
			break;
	    }
	    
	    pos d;
	    char dir;

		for (int i = 0;i<4;i++) {
			directions[i].weight = (endPos.x-curPos.x)*directions[i].p.x + (endPos.y-curPos.y)*directions[i].p.y;
			directions[i].weight *= (-1);
		}

	    sort(directions.begin(), directions.end());
	    
	    for (int i = 0;i<4;i++) {
	    	d = directions[i].p;
	        dir = dirs(d);
	        
	        //Check if visited or unreachable
	        char a = dirMap[(curPos.y+d.y)*NR_COLUMNS+(curPos.x+d.x)];
	        
	        if (a == FREE && !obstacles.get(curPos.y+d.y,curPos.x+d.x)) { //If space is free
	            //Visit
	            dirMap[(curPos.y+d.y)*NR_COLUMNS+(curPos.x+d.x)] = dirs(d);
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        }
	    }
	}
	
	if (!goalReached) {
	    //Invalid move
	    vector<char> ret;
	    ret.push_back('X');
		return ret;
	}	
	
	
	//Else
	char finalMove = dirs(bm.start-endPos);
	pos curPos = endPos;
	char nd = dirMap[endPos.y*NR_COLUMNS+endPos.x];
	vector<char> path;
	path.push_back(finalMove);
	while (nd != 'S') {
		//fprintf(stderr, "nd = %c\n", nd);
	    path.push_back(nd);
	    pos pnd = direction(nd);
	    curPos = pos(curPos.x-pnd.x, curPos.y-pnd.y);
	    nd = dirMap[curPos.y*NR_COLUMNS+curPos.x];
	}
	
	reverse(path.begin(),path.end());
	//Or just let main print them in reverse order.
	
	return path;
}


//new
bool pathExistsAStar(const pos& p1, const pos& p2, GameState * gs) {
	priority_queue<posScore> q;
	bitString visitedList = GOALS;
	visitedList.clear();
	bitString obstacles = gs->boxes | WALLS;
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
			visitedList.set(m1.y, m1.x);
		}
		if(!obstacles.get(m2.y,m2.x) && !visitedList.get(m2.y,m2.x)) {
			m2.score = aStarDist(m2,p2,start);
			q.push(m2);
			visitedList.set(m2.y, m2.x);
		}
		if(!obstacles.get(m3.y,m3.x) && !visitedList.get(m3.y,m3.x)) {
			m3.score = aStarDist(m3,p2,start);
			q.push(m3);
			visitedList.set(m3.y, m3.x);
		}
		if(!obstacles.get(m4.y,m4.x) && !visitedList.get(m4.y,m4.x)) {
			m4.score = aStarDist(m4,p2,start);
			q.push(m4);
			visitedList.set(m4.y, m4.x);
		}

	}

	//fprintf(stderr, "Returned false from A*\n");
	return false;

}


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


