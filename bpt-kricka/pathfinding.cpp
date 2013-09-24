#include <stack>
#include <set>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "../GameState.cpp"
#include "../Map.cpp"

using namespace std;

struct pos {
    int x;
    int y;
    pos operator-(pos other) const {
    	return {x-other.x, y-other.y};
    }
    pos operator+(pos other) const {
    	return {x+other.x, y+other.y};
    }
    pos operator-() const {
    	return {-x, -y};
    }
};

struct dirEntry {
	pos p;
	int weight;
	bool operator<(dirEntry other) const {
		return weight > other.weight;
	}
	
};


pos direction(char ch) {
    switch (ch) {
        case 'U':
            return {0,-1};
            break;
        case 'R':
            return {1,0};
            break;
        case 'D':
            return {0,1};
            break;
        case 'L':
            return {-1,0};
            break;
        default:
            return {42,42};     //This shouldn't be possible!
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
    }
}

void moveToPath (GameState gs, boxMove bm) {

	int h = gs.map->getHeight();
	int w = gs.map->getWidth();
	
	pos startPos, endPos;
	startPos.x = get<0>(gs.player);
	startPos.y = get<1>(gs.player);
	endPos.x = 2*(get<0>(bm.start))-(get<0>(bm.end));
	endPos.y = 2*(get<1>(bm.start))-(get<1>(bm.end));
	
	std::vector<dirEntry> directions;
	directions.push_back({{0,-1},0});
	directions.push_back({{1, 0},0});
	directions.push_back({{0, 1},0});
	directions.push_back({{-1,0},0});
	
	char dirMap[w][h];
	
	for (int i=0;i<w;i++) {
        for (int j=0;j<h;j++) {
            dirMap[i][j] = '-';
        }
    }
	
	dirMap[startPos.x][startPos.y] = 'S';
	
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
	        char a = dirMap[curPos.x+d.x][curPos.y+d.y];
	        
	        if (a == '-' && (gs.boxes.end() == gs.boxes.find(std::pair<int,int>(curPos.x+d.x,curPos.y+d.y))) && !gs.map->isWall(std::pair<int,int>(curPos.x+d.x,curPos.y+d.y))) { //If space is free
	            //Visit
	            dirMap[curPos.x+d.x][curPos.y+d.y] = dirs(d);
	            q.push({curPos.x+d.x, curPos.y+d.y});
	        } else if (a == '.') {
	            //Goal reached!
	            dirMap[curPos.x+d.x][curPos.y+d.y] = dirs(d);
	            goalReached = true;
	            endPos = {curPos.x+d.x,curPos.y+d.y};
	            break;
	        }
	    }
	}
	
	if (!goalReached) {
	    //Invalid move
	    printf("no path\n");
	    return;
	}	
	
	//Else
	pos curPos = endPos;
	char nd = dirMap[endPos.x][endPos.y];
	std::vector<char> path;
	int i = 0;
	while (nd != 'S') {
	    path[i] = nd;
	    pos pnd = direction(nd);
	    curPos = {curPos.x-pnd.x, curPos.y-pnd.y};
	    nd = dirMap[curPos.x][curPos.y];
	    i++;
	}

	// Output answer
	i--;
	for (;i > 0;i--) {
	    printf("%c ",path[i]);
	}
	printf("%c\n",path[0]);
	
	char finalMove = dirs(endPos-{(get<0>(bm.start))-(get<0>(bm.end)),(get<1>(bm.start))-(get<1>(bm.end))});
	printf("%c\n",finalMove);
}


int main(int argc, char **argv)
{
	fprintf(stderr, "Well, it can at least run\n");
	return 0;
}



