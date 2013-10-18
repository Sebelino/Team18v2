#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
//#include <omp.h>
#include <sstream>
#include <iterator>
#include <queue>
#include <bitset>
#include <list>
#include "GameState.h"
#include "Constants.h"
//#include "Heuristics.h"
#include "DeadlockDetection.h"

using namespace std;

GameState::GameState(pos player_start) {
    /*
    bool found = false;
	for (int y = 0;y < b.size();y++) {
        for (int x = 0;x < b[y].size();x++) {
            if(b[y][x] == PLAYER || b[y][x] == PLAYER_ON_GOAL){
            	if(b[y][x] == PLAYER){
            		b[y][x] = FREE;
            	} else if (b[y][x] == PLAYER_ON_GOAL) {
	            	b[y][x] = GOAL;
            	}
                if(!found){
                    player = pos(x,y);
                    found = true;
                }else{
                    cerr << "WTH? The map contains more than 1 player!" << endl;
                    throw 777;
                }
            }
        }
	}
	*/
	player = player_start;
	boxes = BOXES;
	//board = b;
	boxMove s;
	s.start.x = -1;
	s.start.y = -1;
	s.end.x = -1;
	s.end.y = -1;
	depth = 0;
	src = s;
    parent = NULL;
    //findStaticDeadLocks(board);
	findStaticDeadLocks();
	score = 0;
	//cerr << boxes;
	//heuristicEvenBetter(*this);
	
}

/*
Create a new GameState from a previous GameState, assuming a box was moved
from box_move.start to box_move.end. Also updates player position to box_move.start.
*/
GameState::GameState(GameState * prev, struct boxMove * box_move) {
	player = box_move->start;
	src = *box_move;
    parent = prev;
    //board = prev->board;
	depth = prev->depth + 1;
	boxes = prev->boxes;
	
	//fprintf(stderr, "Before moving box: Number of boxes: %d\n", sumbefore);
	//Alter box configuration
	//fprintf(stderr, "Before set: %d\n", boxes.get(src.end.y, src.end.x));
	boxes.set(src.end.y, src.end.x);
	//fprintf(stderr, "After set: %d\n", boxes.get(src.end.y, src.end.x));
	//fprintf(stderr, "Before reset: %d\n", boxes.get(src.start.y, src.start.x));
	boxes.reset(src.start.y, src.start.x);
	//fprintf(stderr, "After reset: %d\n", boxes.get(src.start.y, src.start.x));
	
	/*
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
	*/
	
	//Detect dynamic deadlocks:
	/*
	if(findDynamicDeadlocks(this,src.end)) {
		fprintf(stderr, "Found deadlock in:\n");
		cerr << *this;
	}
	*/

	//	score = -10000000;
	//else
	//double start = omp_get_wtime();
	//findDynamicDeadlocks(this,src.end);
	//double end = omp_get_wtime();
	//cerr << "Deadlock detection took " << (end-start )* 1000000 << endl;

	//start = omp_get_wtime();

	//heuristicEvenBetter(*this);

	//end = omp_get_wtime();
	//cerr << "Heuristic took " << (end-start )* 1000000 << endl;
}


// Destructor.
GameState::~GameState(){}

/* Just a way to sort the GameStates. */
//bool GameState::operator<(GameState other) const {
//    return score < other.score;
//}

/* Returns true if the gamestate is a solution */
bool GameState::isSolution() {

	if (boxes == GOALS) {
		return true;
	} else {
		return false;
	}

	/*
	for (int i = 0;i<(int)board.size();i++) {
		for (int j = 0;j<(int)board[i].size();j++) {
			if (board[i][j] == BOX || board[i][j] == GOAL) {
				return false;
			}
		}
	}
	return true;
	*/
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
	std::ostream& stream = strm;
	/*
    for(int i = 0;i < state.board.size();i++){
		for(int j = 0;j < state.board[i].size();j++){
			if (state.player.x == j && state.player.y == i) {
				if (state.board[i][j] == FREE || state.board[i][j] == DEADLOCK) {
					stream << PLAYER;
				} else if (state.board[i][j] == GOAL) {
					stream << PLAYER_ON_GOAL;
				}
			} else {
				stream << state.board[i][j];
			}
		}
        stream << endl;
    }
    */
    stream << "Cannot print GameStates anymore..." << endl;
    return stream;
}

/* Returns a set of all succeeding states. */
vector<GameState*> GameState::findNextMoves(){
	vector<GameState*> successors;
	vector<boxMove> moves;
	
	//vector<vector<char> > dirMap = board;
	
	//new stuff
	bitString obstacles = boxes | WALLS;
	bitString dlsAndObstacles = obstacles | DEADLOCKS;
	bitString visitedList = obstacles;
	visitedList.set(player.y, player.x);
	bool va, vb, vc;
	//cerr << visitedList << endl;
	//cerr << WALLS << endl;
	//cerr << boxes << endl;
	//cerr << obstacles << endl;
	//cerr << "dlsAndObstacles: \n" << dlsAndObstacles << endl;
	//________
	
	vector<pos> directions;
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1, 0));
	directions.push_back(pos(0, 1));
	directions.push_back(pos(-1,0));
	
	queue<pos> q;
	//fprintf(stderr, "player pos: %d, %d\n", player.x, player.y);
	//fprintf(stderr, "dirMap size is:%d x %d\n", (int)dirMap.size(), (int)dirMap[0].size());
	//fprintf(stderr, "f1\n");

	q.push(player);
	pos curPos;
	pos d;
	char dir;
	char a, b;
	boxMove bm;
	
	//fprintf(stderr, "f2\n");
	//Search the graph
	//cerr << "================================" << endl;
	//double start = omp_get_wtime();

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

	        //new stuff:
	        va = visitedList.get(curPos.y+d.y,curPos.x+d.x);
	        vc = boxes.get(curPos.y+d.y,curPos.x+d.x);
	        if (!va) { //If space is free and unvisited
	            //Visit
	            //fprintf(stderr, "before:Visited nodes: %d\n", visitedList.sum());
	            visitedList.set(curPos.y+d.y,curPos.x+d.x);
	            //fprintf(stderr, "after:Visited nodes: %d\n", visitedList.sum());
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        } else if (vc) {	//If there is a box here...
	        	vb = dlsAndObstacles.get(curPos.y+d.y+d.y,curPos.x+d.x+d.x);
	        	//b = board[curPos.y+d.y+d.y][curPos.x+d.x+d.x];
				if (!vb) { // ...and a free position behind...
					//add move
	        		bm.start = curPos+d;
	        		bm.end = bm.start+d;
	        		moves.push_back(bm);
	        	}
	        }
	        //____________
	        
	    }
	}
	//double end = omp_get_wtime();
	//cerr << "Findnextmoves search took " << (end-start)*1000000 << endl;

	//fprintf(stderr, "f4, moves.size() = %d\n", (int)moves.size());
	//start = omp_get_wtime();
	for (int i = 0;i < (int)moves.size();i++) {
		successors.push_back(new GameState(this,&(moves[i])));
	}
	//end = omp_get_wtime();
	//cerr << "Findnextmoves creating gamestates took " << (end-start)*1000000 << endl;

    return successors;
}

/*
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
*/


/**
 * Format:
 * "<player.x><player.y><segment>...<segment>"
 * where <segment>...<segment> constitue a bit map for all boxes.
 **/
bitString GameState::hash() const {
	return boxes;
}

/*
string GameState::hash(bool s) const {
    string hash;
    int bsize = (board.size()-2)*(board[0].size()-2);
    hash.reserve(bsize+2);
    //hash.push_back(player.x);
    //hash.push_back(player.y);
    //char i = 1;
    unsigned char i = 128;
    int position = 0;
    char ch = 0;
    for(int y = 1;y < board.size()-1;y++){
        for(int x = 1;x < board[y].size()-1;x++){
            if (board[y][x] == BOX || board[y][x] == BOX_ON_GOAL){
            	//If there was a box here
            	//ch += (i << (7-position));
                ch += (i >> position);
            }
            position = (position+1) % 8;
            if(position == 0){
                hash.push_back(ch);
                ch = 0;
            }
        }
    }
    if(position != 0){
        hash.push_back(ch);
    }
    return hash;
}
*/

