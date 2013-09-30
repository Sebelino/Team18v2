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
	boxMove s;
	s.start.x = -1;
	s.start.y = -1;
	s.end.x = -1;
	s.end.y = -1;
	src = s;
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
	src = *box_move;
}

/*
Private method to set all boxes from a board, and also set player position.
Only called from constructor.
*/
void GameState::setBoxes(vector<vector<char> > * stringmap) {
	vector<vector<char> >& board = *stringmap;
	
	for(unsigned int i = 0; i < board.size(); i++) {
		for(unsigned int j = 0; j < board[i].size(); j++) {
			char c = board[i][j];
			if(c == BOX){
                boxes.insert(pos(j,i));
				board[i][j] = FREE; // Overwrite dynamic entity.
			}else if(c == BOX_ON_GOAL){
				boxes.insert(pos(j,i));
                board[i][j] = GOAL;
			}else if(c == PLAYER){
				player = pos(j,i);
                board[i][j] = FREE;
            }else if(c == PLAYER_ON_GOAL) {
				player = pos(j,i);
				board[i][j] = GOAL; // Overwrite dynamic entity.
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
    return heuristic() < other.heuristic();
}

/* Returns true if the gamestate is a solution */
bool GameState::isSolution() {
	set<pos>::iterator it;
	for(it = boxes.begin(); it != boxes.end() ; it++) {
		if(!(map->isGoal(*it)))
			return false;
	}
	return true;
}

/* Returns true iff it is possible to move a box in the way specified in the argument. */
bool GameState::isValid(const struct boxMove & m){
    vector<char> path = moveToPath(this,m);
    if(path[0] == 'X'){
        return false;
    }
    if(map->isDeadlock(m.end) || map->isWall(m.end)){
        return false;
    }
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
    return true;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    vector<vector<char> > & m = *(state.map->getOriginalMap());
    for(int i = 0;i < state.map->getHeight();i++){
		for(int j = 0;j < state.map->getWidth();j++){
            const bool printBox = state.boxes.find(pos(j,i)) != state.boxes.end();
            if(printBox){
                stream << "$";
            }else if(state.player.x == j && state.player.y == i){
                stream << "@";
            }else{
                stream << m[i][j];
            }
        }
        stream << endl;
    }
    return stream;
}

/* Returns the set of moves possible to make for the box in position boxPos. */
set<boxMove> GameState::moves(pos boxPos){
    set<boxMove> moveSet;
    boxMove up; up.start = boxPos; up.end = boxPos+pos(0,-1);
    boxMove down; down.start = boxPos; down.end = boxPos+pos(0,1);
    boxMove left; left.start = boxPos; left.end = boxPos+pos(-1,0);
    boxMove right; right.start = boxPos; right.end = boxPos+pos(1,0);
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
vector<GameState> GameState::findNextMoves(){
    vector<GameState> successors;
    set<pos>::iterator it;
    for(it = boxes.begin();it != boxes.end();++it){
        pos b = *it;
        set<boxMove> ms = moves(b);
        set<boxMove>::iterator it2;
        for(it2 = ms.begin();it2 != ms.end();++it2){
            boxMove m = *it2;
            GameState gs(this,&m);
            successors.push_back(gs);
        }
    }
    return successors;
}

int GameState::heuristic() const{
    set<pos> goals = map->getGoals();
    if(boxes.size() != goals.size()){
        cerr << "Assertion failed: boxes.size() == goals.size()" << endl;
    }
    return goals.size()-boxes.size();
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
        int posHash = p.x*map->getOriginalMap()->size()+p.y;
        hash += multiplier*posHash;
        multiplier *= 2;
    }
    return hash;
}



