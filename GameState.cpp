#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "gamestate.h"
#include "constants.cpp"

using namespace std;

struct move{
    pair<int,int> start;
    pair<int,int> end;
};

GameState::GameState(Map * canvas) {
	map = canvas;
	Map& m = *canvas;
	vector<vector<char> > * board = m.getOriginalMap();
	setBoxes(board);
}

/*
Create a new GameState from a previous GameState, assuming a box was moved
from box_move.start to box_move.end. Also updates player position to box_move.start.
*/
GameState::GameState(GameState * prev, struct move * box_move) {
	boxes = prev->boxes;
	boxes.erase(box_move->start);
	boxes.insert(pair<int,int>(box_move->end));
	player = pair<int,int>(box_move->start);
	map = prev->map;
}

/*
Provate method to set all boxes from a board, and also set player position.
Only called from constructor.
*/
void GameState::setBoxes(vector<vector<char> > * stringmap) {
	vector<vector<char> >& board = *stringmap;
	
	for(unsigned int i = 0; i < board.size(); i++) {
		for(unsigned int j = 0; j < board[i].size(); j++) {
			char c = board[i][j];
			if(c == BOX) {
				boxes.insert(pair<int,int>(i,j));
			} else if(c == PLAYER || c == PLAYER_ON_GOAL) {
				player = pair<int,int>(i,j);
			}
		}
	}
}

/*
GameState GameState::pushBox(const struct move & m){
    vector<vector<char> > stringmap = map;
    stringmap[m.startY][m.startX] = ' ';
    stringmap[m.endY][m.endX] = '$';
    return GameState(stringmap,0,0);
}*/
GameState::~GameState(){}
/*
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(unsigned int i = 0;i < state.map.size();i++){
        for(unsigned int j = 0;j < state.map[i].size();j++){
            stream << state.map[i][j];
        }
        stream << endl;
    }
    return stream;
}
*/