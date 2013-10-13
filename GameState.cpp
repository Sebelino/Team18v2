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
#include "Heuristics.h"

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
	heuristicSmarter(*this);
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
    heuristicSmarter(*this);
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
//TODO Remove the vector references once it works; they are just for debugging.
string GameState::hash() const {
    string hash;
    int bsize = board.size()*board[0].size();
    hash.reserve(bsize+2);
    hash.push_back((char)player.x);
    hash.push_back((char)player.y);
    
    /*
    vector<char> vechash;
    vechash.reserve(bsize+2);
    vechash.push_back((char)player.x);
    vechash.push_back((char)player.y);
    */
    
    char i = 1;
    int position = 0;
    char ch = 0;
    
    for(int y = 1;y < (int)board.size()-1;y++){
        for(int x = 1;x < (int)board[y].size()-1;x++){
            if (board[y][x] == BOX || board[y][x] == BOX_ON_GOAL){
            	//If there was a box here
                ch += (i << (7-position));
            }
            position = (position+1) % 8;
            if(position == 0){
                hash.push_back(ch);
                //vechash.push_back(ch);
                ch = 0;
            }
        }
    }
    if(position != 0){
        hash.push_back(ch);
        //vechash.push_back(ch);
    }
    cerr << "hash is: " << hash << endl;
    
    /*
    fprintf(stderr, "vechash is:\n");
    for (int i = 0;i<vechash.size();i++) {
    	fprintf(stderr, "%X, %c or %d\n", vechash[i], vechash[i], vechash[i]);
    }*/
    
    return hash;
}


