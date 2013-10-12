#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
#include <sstream>
#include <iterator>
#include <queue>
#include <bitset>
#include <list>
#include "GameState.h"
#include "Constants.h"

using namespace std;

GameState::GameState(vector<vector<char> > b) {
    bool found = false;
	for (int y = 0;y < b.size();y++) {
        for (int x = 0;x < b[y].size();x++) {
            char cell = b[y][x];
            if(cell == PLAYER || cell == PLAYER_ON_GOAL){
                if(!found){
                    player = pos(x,y);
                }else{
                    cerr << "WTH? The map contains more than 1 player!" << endl;
                    throw 777;
                }
            }
        }
	}
	board = b;
	boxMove s;
	s.start.x = -1;
	s.start.y = -1;
	s.end.x = -1;
	s.end.y = -1;
	src = s;
    parent = NULL;
	heuristicSmarter();
}

/*
Create a new GameState from a previous GameState, assuming a box was moved
from box_move.start to box_move.end. Also updates player position to box_move.start.
*/
GameState::GameState(GameState * prev, struct boxMove * box_move) {
	player = box_move->start;
	src = *box_move;
    parent = prev;
    board = prev->board;
    
    if (board[src.start.y][src.start.x] == BOX) {
    	board[src.start.y][src.start.x] = FREE;
    } else if (board[src.start.y][src.start.x] == BOX_ON_GOAL) {
		board[src.start.y][src.start.x] = GOAL;
	}
	
	if (board[src.end.y][src.end.x] == GOAL) {
		board[src.end.y][src.end.x] = BOX_ON_GOAL;
	} else if (board[src.end.y][src.end.x] == FREE) {
		board[src.end.y][src.end.x] = BOX;
	}
    heuristicSmarter();
}


// Destructor.
GameState::~GameState(){}

// Returns the state that results from pushing a box.
/*
GameState GameState::pushBox(const struct boxMove & m){
    vector<vector<char> > stringmap = map;
    stringmap[m.startY][m.startX] = ' ';
    stringmap[m.endY][m.endX] = '$';
    return GameState(stringmap,0,0);

}*/

/* Just a way to sort the GameStates. */
bool GameState::operator<(GameState other) const {
    return score < other.score;
}

/* Returns true if the gamestate is a solution */
bool GameState::isSolution() {
	for (int i = 0;i<(int)board.size();i++) {
		for (int j = 0;j<(int)board[i].size();j++) {
			if (board[i][j] == BOX || board[i][j] == GOAL) {
				return false;
			}
		}
	}
	return true;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(int i = 0;i < state.board.size();i++){
		for(int j = 0;j < state.board[i].size();j++){
			stream << state.board[i][j];
		}
        stream << endl;
    }
    return stream;
}


/* Returns a set of all succeeding states. */
vector<GameState*> GameState::findNextMoves(){
	
	vector<GameState*> successors;
	vector<boxMove> moves;
	
	vector<vector<char> > dirMap = board;
	
	vector<pos> directions;
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1, 0));
	directions.push_back(pos(0, 1));
	directions.push_back(pos(-1,0));
	
	queue<pos> q;
	
	dirMap[player.y][player.x] = 'S';

	q.push(player);
	pos curPos;
	pos d;
	char dir;
	char a, b;
	boxMove bm;
	
	//Search the graph
	while (!q.empty()) {
	    curPos = q.front();
	    q.pop();
	    
	    //DEBUG
	    /*
		fprintf(stderr, "dirMap:\n");
		for (int i = 0;i<dirMap.size();i++) {
			for (int j = 0;j<dirMap[i].size();j++) {
				fprintf(stderr, "%c", dirMap[i][j]);
			}
			fprintf(stderr, "\n");
		}
		*/
        
	    
	    for (int i = 0;i<4;i++) {
	    	d = directions[i];
	        dir = dirs(d);
	        
	        //Check if visited or unreachable
	        a = dirMap[curPos.y+d.y][curPos.x+d.x];
	        
	        if ((a == FREE || a == GOAL || a == DEADLOCK)) { //If space is free
	            //Visit
	            dirMap[curPos.y+d.y][curPos.x+d.x] = dirs(d);
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        } else if (a == BOX || a == BOX_ON_GOAL) {	//If there is a box here
	        	b = dirMap[curPos.y+d.y+d.y][curPos.x+d.x+d.x];
	        	if (b == FREE || b == GOAL) {
	        		bm.start = curPos+d;
	        		bm.end = bm.start+d;
	        		moves.push_back(bm);
	        	}
	        }
	    }
	}

	for (int i = 0;i < (int)moves.size();i++) {
		successors.push_back(new GameState(this,&(moves[i])));
	}
	
    return successors;
}

int GameState::heuristic() const{
    int score;
    for (int i = 0;i<board.size();i++) {
    	for (int j = 0;j<board[i].size();j++) {
    		if (board[i][j] == BOX_ON_GOAL) {
    			score += 5;
    		}
    		else if (board[i][j] == BOX) {
    			score -= 5;
    		}
    	}
    }
    return score;
}

/**
 * Format:
 * "<player.x><player.y><segment>...<segment>"
 * where <segment>...<segment> constitue a bit map for all boxes.
 **/
string GameState::hash() const {
    string hash;
    int bsize = board.size()*board[0].size();
    hash.reserve(bsize+2);
    hash.push_back((char)player.x);
    hash.push_back((char)player.y);
    char i = 1;
    int position = 0;
    char ch = 0;
    for(int y = 0;y < board.size();y++){
        for(int x = 0;x < board[y].size();x++){
            if (board[y][x] == BOX || board[y][x] == BOX_ON_GOAL){
                ch += (128 >> (position % 8));
            }
            if(position % 8 == 7){
                hash.push_back(ch);
                ch = 0;
            }
            position++;
        }
    }
    hash.push_back(ch);
    return hash;
}

void GameState::heuristicSmarter() {
	/*
	1.Sort all goals by some heuristic
2.For each goal, find closest box, that is not taken already.
3.Mark box as taken
4.Add to score the distance (or subtract)*/

	//4 classes of goals
	vector<pos> buckets[4]; //puts each goal in a bucket.

	for(int i = 1; i < board.size() -1; i++) {
		for(int j = 1; j < board[i].size()-1;j++) {
			if(board[i][j] == GOAL || board[i][j] == PLAYER_ON_GOAL || board[i][j] == BOX_ON_GOAL) {
				if(board[i][j] == BOX_ON_GOAL && isBoxWall(i,j)) {
					board[i][j] == WALL;
				} else {
					buckets[checkGoalClass(i,j)].push_back(pos(i,j));
				}
			} 
		}
	}

	//step 2
	//complexity of this step is O(Area of board + (numberOfGoals^2 / 2)) TODO not completely sure, see below

	//actually this is step 3 and 4 at the same time as well.

	//first get a list of all boxes
	list<pos> allBoxes;
	for(int i = 1; i < board.size() -1; i++) {
		for(int j = 1; j < board[i].size()-1;j++) {
			if(board[i][j] == BOX || board[i][j] == BOX_ON_GOAL) {
				allBoxes.push_back(pos(i,j));
			}
		}
	}
	//then find the closest boxes and remove them from the list, while incrementing score.
	int score = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < buckets[i].size(); j++) {
			//for each goal
			//find the minimum distance to a box.
			list<pos>::iterator bit;
			int shortestDist = 10000000; //something very big, I don't think we will have maps this big!
			list<pos>::iterator shortestDistP;
			pos& goalPos = buckets[i][j];
			for(bit = allBoxes.begin(); bit != allBoxes.end(); bit++) {
				int herDist = heuristicDistance(goalPos, *bit);
				if(herDist < shortestDist) {
					shortestDistP = bit;
					shortestDist = herDist;
				}
			}
			allBoxes.erase(shortestDistP); //TODO not sure if this is constant or it has to iterate through list...
			score += shortestDist;
		}
	}

	this->score = -score;
}

/*
* Private function for the heuristic function - return the class of a specific goal.
* Classes are:
* 0 - Goals with 3 walls next to them.
* 1 - Goals with 2 walls next to eachother (not 2 opposite walls)
* 2 - Goals with 1 wall next to them.
* 3 - Goals with 0 walls next to them or 2 opposite walls.
*/
int GameState::checkGoalClass(int i, int j) {
	int clas = 0;
	bool topWall = false;
	bool leftWall = false;
	bool rightWall = false;
	bool botWall = false;

	if(board[i-1][j] == WALL || (board[i-1][j] == BOX_ON_GOAL && isBoxWall(i-1,j))) {
		topWall = true;
		clas++;
	}
	if(board[i+1][j] == WALL || (board[i+1][j] == BOX_ON_GOAL && isBoxWall(i+1,j))) {
		botWall = true;
		clas++;
	}
	if(board[i][j+1] == WALL || (board[i][j+1] == BOX_ON_GOAL && isBoxWall(i,j+1))) {
		rightWall = true;
		clas++;
	}
	if(board[i][j-1] == WALL || (board[i][j-1] == BOX_ON_GOAL && isBoxWall(i,j-1))) {
		leftWall = true;
		clas++;
	}

	if((clas == 2) && ((topWall && botWall) || (leftWall && rightWall))) {
		clas = 0; //a so-called pathway
	}

	return 3-clas; //reverse, so that high priority gets class 0.

}

/*
* Check if the box on the current position (not checked that it is a box btw)
* has 2 walls next to it so that it cannot be moved. Will do at maximum 4 comparisions.
*/
bool GameState::isBoxWall(int i, int j) {
	bool northWall = false;
	if(board[i-1][j] == WALL) {
		northWall = true;
	}

	//this might look a bit sketchy but does as few comparisions as possible
	if(northWall) {
		if(board[i][j-1] == WALL || board[i][j+1] == WALL)
			return true;
		else
			return false;
	} else {
		if(board[i+1][j] == WALL) {
			if(board[i][j-1] == WALL || board[i][j+1] == WALL)
				return true;
			else
				return false;
		} else {
			return false;
		}
	}
}

int GameState::heuristicDistance(const pos& p1, const pos& p2) {
	return abs(p1.x-p2.x)+abs(p1.y-p2.y);
}

