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

GameState::GameState(vector<vector<char> > b) {
    bool found = false;
	numBoxes = 0;
	height = b.size();
	width = b[0].size();

	//allocate map
	board = new char*[height];
	for(int i = 0; i < height; i++) {
		board[i] = new char[width];
	}

	for (int y = 0;y < height;y++) {
        for (int x = 0;x < width;x++) {
			//copy value over
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
			else if(b[y][x] == BOX || b[y][x] == BOX_ON_GOAL) {
				numBoxes++;
			}
			board[y][x] = b[y][x]; //copy board
        }
	}
	//board = b;
	boxMove s;
	s.start.x = -1;
	s.start.y = -1;
	s.end.x = -1;
	s.end.y = -1;
	depth = 0;
	src = s;
    parent = NULL;
    findStaticDeadLocks(board,height,width); 
	score = 0;
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
	depth = prev->depth + 1;
	width = prev->width;
	height = prev->height;
	numBoxes = prev->numBoxes;

	//do a smart copy of the board.
	int row1 = box_move->start.y;
	int row2 = box_move->end.y;

	//allocate new rows
	board = new char*[height];
	for(int i = 0; i < height; i++) {
		if(i == row1 || i == row2) {
			board[i] = new char[width];
			for(int j = 0; j < width; j++) { //copy entire row
				board[i][j] = prev->board[i][j];
			}
		} else {
			board[i] = prev->board[i]; //only copy the pointer!
		}
	}

	//these changes should be safe to do, since we have allocated new rows for src.start.y and src.end.y.
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
}


// Destructor.
GameState::~GameState(){}

/* Just a way to sort the GameStates. */
bool GameState::operator<(GameState other) const {
    return score < other.score;
}

/* Returns true if the gamestate is a solution */
bool GameState::isSolution() {
	for (int i = 0;i<height;i++) {
		for (int j = 0;j<width;j++) {
			if (board[i][j] == BOX || board[i][j] == GOAL) {
				return false;
			}
		}
	}
	return true;
}

vector<vector<char> > GameState::getCopyOfBoard() {
	vector<vector<char> > cBoard(height);
	for(int i = 0; i < height; i++) {
		cBoard[i].reserve(width);
	}

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			cBoard[i][j] = board[i][j];
		}
	}

	return cBoard;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
	for(int i = 0;i < state.height;i++){
		for(int j = 0;j < state.width;j++){
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
    return stream;
}

/* Returns a set of all succeeding states. */
vector<GameState*> GameState::findNextMoves(){
	vector<GameState*> successors;
	vector<boxMove> moves;
	
	vector<vector<char> > dirMap = getCopyOfBoard();
	
	vector<pos> directions;
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1, 0));
	directions.push_back(pos(0, 1));
	directions.push_back(pos(-1,0));
	
	queue<pos> q;
	//fprintf(stderr, "player pos: %d, %d\n", player.x, player.y);
	//fprintf(stderr, "dirMap size is:%d x %d\n", (int)dirMap.size(), (int)dirMap[0].size());
	//fprintf(stderr, "f1\n");
	dirMap[player.y][player.x] = 'V';

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
	        //dir = dirs(d);
	        
	        //Check if visited or unreachable
	        a = dirMap[curPos.y+d.y][curPos.x+d.x];
	        
	        if ((a == FREE || a == GOAL || a == DEADLOCK)) { //If space is free
	            //Visit
	            dirMap[curPos.y+d.y][curPos.x+d.x] = 'V';
	            q.push(pos(curPos.x+d.x, curPos.y+d.y));
	        } else if (a == BOX || a == BOX_ON_GOAL) {	//If there is a box here
	        	b = board[curPos.y+d.y+d.y][curPos.x+d.x+d.x];
				if (b == FREE || b == GOAL) {
	        		bm.start = curPos+d;
	        		bm.end = bm.start+d;
	        		moves.push_back(bm);
	        	}
	        }
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

int GameState::heuristic() const{
    int score;
	for (int i = 0;i<height;i++) {
    	for (int j = 0;j<width;j++) {
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
    int bsize = (height-2)*(width-2);
    hash.reserve(bsize+2);
    //hash.push_back(player.x);
    //hash.push_back(player.y);
    //char i = 1;
    unsigned char i = 128;
    int position = 0;
    char ch = 0;
    for(int y = 1;y < height-1;y++){
        for(int x = 1;x < width-1;x++){
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

