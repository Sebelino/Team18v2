//Denna klarar Kattis tester på 0,03s

#include <stack>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 1004

using namespace std;

using std::stack;

struct pos {
    int x;
    int y;
};

struct dirEntry {
	pos p;
	int weight;
	bool operator<(dirEntry other) const {
		return weight > other.weight;
	}
};


char map[SIZE][SIZE][2];

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


int main(int argc, char **argv)
{
    int x=0, y=0;
    char inp;
    pos startPos = {-1,-1};
    pos goalPos = {-1,-1};

	std::vector<dirEntry> directions;
	directions.push_back({{0,-1},0});
	directions.push_back({{1, 0},0});
	directions.push_back({{0, 1},0});
	directions.push_back({{-1,0},0});
	
	/*
	directions[0].p = {0,-1};
	directions[1].p = {1, 0};
	directions[2].p = {0, 1};
	directions[3].p = {-1,0};
	*/

    //Fill with walls
    for (int i=0;i<SIZE;i++) {
        for (int j=0;j<SIZE;j++) {
            map[i][j][0] = '#';
            map[i][j][1] = '-';
        }
    }

    bool emptyGoalFound = false;

    // Read the board
	while ((inp = getchar()) != EOF) {
	    if (inp == '\n') {  //New row
	        y++;
	        x = 0;
	        continue;
	    } else if (inp == '+') {    //Player already on goal; we're done!
	        printf("\n");
	        return 0;
	    } else if (inp == '@') {    //Player starting position
	        startPos = {x,y};
	    } else if (inp == '.') {
		    goalPos = {x,y};
	        emptyGoalFound = true;
	    }
	    map[x][y][0] = inp;
	    x++;
	}
	
	if (!emptyGoalFound) {
	    printf("no path\n");
	    return 0;
	}
	
	map[startPos.x][startPos.y][1] = 'S';
	
	bool goalReached = false;
	
	stack<pos> q;
	q.push(startPos);
	
	//Search the graph
	while (!q.empty()) {
	    pos curPos = q.top();
	    q.pop();
	    
	    pos d;
	    char dir;
	    
	    /*for (int i = 0;i<4;i++) {
	    	directions[i].weight = (goalPos.x-curPos.x)*directions[i].p.x + (goalPos.y-curPos.y)*directions[i].p.y;
	    }*/
	    
	    std::sort(directions.begin(), directions.end());
	    
	    //Debug
	    for (int i = 0;i<4;i++) {
	    	fprintf(stderr, "d = {%i, %i}, w = %i\n", directions[i].p.x, directions[i].p.y, directions[i].weight);
	    }
	    
	    
	    for (int i = 0;i<4;i++) {
	    	d = directions[i].p;
	        dir = dirs(d);
	        //d = direction(dir);
	    	//fprintf(stderr, "d = {%i, %i}\n", d.x, d.y);
	        
	        //Check if visited or unreachable
	        char a = map[curPos.x+d.x][curPos.y+d.y][0];
	        char b = map[curPos.x+d.x][curPos.y+d.y][1];
	        if (a == ' ' && b == '-') {
	            //Visit
	            map[curPos.x+d.x][curPos.y+d.y][1] = dirs(d);
	            q.push({curPos.x+d.x, curPos.y+d.y});
	        } else if (a == '.') {
	            //Goal reached!
	            map[curPos.x+d.x][curPos.y+d.y][1] = dirs(d);
	            goalReached = true;
	            goalPos = {curPos.x+d.x,curPos.y+d.y};
	            break;
	        }
	    }
	}
	
	if (!goalReached) {
	    //Couldn't reach the goal
	    printf("no path\n");
	    return 0;
	}
	
	        //Print the map and dirmap
        for (int i = 0;i<10;i++) {
            for (int j = 0;j<10;j++) {
                fprintf(stderr, "%c", map[j][i][0]);
            }
            fprintf(stderr, "\n");
        }
 
        for (int i = 0;i<10;i++) {
            for (int j = 0;j<10;j++) {
                fprintf(stderr, "%c", map[j][i][1]);
            }
            fprintf(stderr, "\n");
        }
	
	//Else
	pos curPos = goalPos;
	char nd = map[goalPos.x][goalPos.y][1];
	char path[SIZE*SIZE];
	int i = 0;
	while (nd != 'S') {
	    path[i] = nd;
	    pos pnd = direction(nd);
	    curPos = {curPos.x-pnd.x, curPos.y-pnd.y};
	    nd = map[curPos.x][curPos.y][1];
	    i++;
	}

	// Output answer
	i--;
	for (;i > 0;i--) {
	    printf("%c ",path[i]);
	}
	printf("%c\n",path[0]);
	
	return 0;
}

