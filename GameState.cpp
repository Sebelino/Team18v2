#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
#include <sstream>
#include <iterator>
#include "GameState.h"
#include "Constants.h"

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
	boxes.insert(box_move->end);
	player = box_move->start;
	map = prev->map;
}

/*
Private method to set all boxes from a board, and also set player position.
Only called from constructor.
*/
void GameState::setBoxes(vector<vector<char> > * stringmap) {
	vector<vector<char> >& board = *stringmap;
	
	for(int i = 0; i < board.size(); i++) {
		for(int j = 0; j < board[i].size(); j++) {
			char c = board[i][j];
			if(c == BOX) {
				boxes.insert({i,j});
			} else if(c == PLAYER || c == PLAYER_ON_GOAL) {
				player = {i,j};
			}
		}
	}
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
    if(hash() < other.hash())
        return true;
    else
        return false;
}


/* Returns true iff it is possible to move a box in the way specified in the argument. */
bool GameState::isValid(const struct boxMove & m){
    vector<char> path = moveToPath(this,m);
    if(path[0] == 'X'){
        return false;
    }
    if(map->isDeadlock(m.end)){
        return false;
    }
    return true;
//    // Testing relative positions.
//    if(!(abs(m.start.x-m.end.x) == 1 && abs(m.start.y-m.end.y) == 0
//      || abs(m.start.x-m.end.x) == 0 && abs(m.start.y-m.end.y) == 1)){
//        return false;
//    }
//    // Testing bounds.
//    if(min(min(min(m.start.x,m.end.x),m.start.y),m.end.y) < 0
//    || max(m.start.y,m.end.y) > map->getHeight()
//    || max(m.start.x,m.end.x) > map[m.start.y].getHeight()){
//        return false;
//    }
//    // Testing starting position chars.
//    if(!(boxes.find(pos(m.start.y,m.start.x)) == '$' || map[m.start.y][m.start.x] == '*')){
//        return false;
//    }
//    // Testing destination position chars.
//    if(!(map[m.start.y][m.start.x] == ' ' || map[m.start.y][m.start.x] == '.')){
//        return false;
//    }
//    return true;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(int i = 0;i < state.map->getHeight();i++){
		for(int j = 0;j < state.map->getWidth();j++){
            stream << state.map->getOriginalMap();
        }
        stream << endl;
    }
    return stream;
}

/* Returns the set of moves possible to make for the box in position boxPos. */
set<boxMove> GameState::moves(pos boxPos){
    set<boxMove> moveSet;
    boxMove up; up.start = boxPos; up.end = boxPos+(pos){0,-1};
    boxMove down; down.start = boxPos; down.end = boxPos+(pos){0,1};
    boxMove left; left.start = boxPos; left.end = boxPos+(pos){-1,0};
    boxMove right; right.start = boxPos; right.end = boxPos+(pos){1,0};
    if(isValid(up)){
        moveSet.insert(up);
    }
    if(isValid(down)){
        moveSet.insert(down);
    }
    if(isValid(left)){
        moveSet.insert(left);
    }
    if(isValid(right)){
        moveSet.insert(right);
    }
    return moveSet;
}

/* Returns a set of all succeeding states. */
set<GameState> GameState::findNextMoves(){
    set<GameState> successors;
    for(pos b : boxes){
        set<boxMove> ms = moves(b);
        for(boxMove m : ms){
            GameState gs(this,&m);
            successors.insert(gs);
        }
    }
    return successors;
}

/* Slightly misleading name because it is meant to be used for sorting GameStates in a std::set, not
 * in a hash datastructure. */
//TODO: Ingen garanti för att alla GameStates har en unik hash.
unsigned long long GameState::hash() const{
    unsigned long long hash = 0;
    set<pos>::iterator it;
    int multiplier = 1;
    for(it = boxes.begin();it != boxes.end();++it){
        pos p = *it;
        hash += multiplier*(p.x*map->getOriginalMap()->size()+p.y);
        multiplier++;
    }
    return hash;
}

