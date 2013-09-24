#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
#include "GameState.h"
#include "Constants.cpp"

using namespace std;

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
GameState::GameState(GameState * prev, struct boxMove * box_move) {
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



// Destructor.
GameState::~GameState(){}

// Returns the state that results from pushing a box.
/*
GameState GameState::pushBox(const struct move & m){
    vector<vector<char> > stringmap = map;
    stringmap[m.startY][m.startX] = ' ';
    stringmap[m.endY][m.endX] = '$';
    return GameState(stringmap,0,0);

}*/



//kommentera please
/*
bool GameState::isValid(const struct boxMove & m){
    // Testing relative positions.
    if(!(abs(m.start.first-m.end.first) == 1 && abs(m.start.second-m.end.second) == 0
      || abs(m.start.first-m.end.first) == 0 && abs(m.start.second-m.end.second) == 1)){
        return false;
    }
    // Testing bounds.
    if(min(min(min(m.start.first,m.end.first),m.start.second),m.end.second) < 0
		|| max(m.start.second,m.end.second) > map->getHeight()
    || max(m.start.first,m.end.first) > map[m.start.second].getHeight()){
        return false;
    }
    // Testing starting position chars.
    if(!(map[m.start.second][m.start.first] == '$' || map[m.start.second][m.start.first] == '*')){
        return false;
    }
    // Testing destination position chars.
    if(!(map[m.start.second][m.start.first] == ' ' || map[m.start.second][m.start.first] == '.')){
        return false;
    }
    return true;
}*/

// ToString for a game state.
/*
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(unsigned int i = 0;i < state.map->getHeight();i++){
		for(unsigned int j = 0;j < state.map->getOriginalMap()->at(i)[j];j++){
            stream << state.map[i][j];
        }
        stream << endl;
    }
    return stream;
}
*/


//set<boxMove> moves(pair<int,int> boxPos){
//    boxMove up ();
//}

// Returns a set of all succeeding states.
set<GameState> findNextMoves(){
    set<GameState> successors;
//    for (x,y) in map.boxes:
//        set<boxMove> ms = moves(x,y);
//        if isValid(move)
    return successors;
}
