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
    parent = NULL;
}

/*
Create a new GameState from a previous GameState, assuming a box was moved
from box_move.start to box_move.end. Also updates player position to box_move.start.
*/
GameState::GameState(GameState * prev, struct boxMove * box_move) {
	boxes = prev->boxes;
	boxes[box_move->start.y][box_move->start.x] = FREE;
	boxes[box_move->end.y][box_move->end.x] = BOX;
	player = box_move->start;
	map = prev->map;
	src = *box_move;
    parent = prev;
}

/*
Private method to set all boxes from a board, and also set player position.
Only called from constructor.
*/
void GameState::setBoxes(vector<vector<char> > * stringmap) {
	vector<vector<char> >& board = *stringmap;
	
	for(unsigned int i = 0; i < board.size(); i++) {
        vector<bool > line;
		for(unsigned int j = 0; j < board[i].size(); j++) {
			char c = board[i][j];
			if(c == BOX){
                boxes[i].push_back(BOX);
				board[i][j] = FREE; // Overwrite dynamic entity.
			}else if(c == BOX_ON_GOAL){
                boxes[i].push_back(BOX);
                board[i][j] = GOAL;
			}else if(c == PLAYER){
                boxes[i].push_back(FREE);
				player = pos(j,i);
                board[i][j] = FREE;
            }else if(c == PLAYER_ON_GOAL) {
                boxes[i].push_back(FREE);
				player = pos(j,i);
				board[i][j] = GOAL; // Overwrite dynamic entity.
			}else{
                boxes[i].push_back(FREE);
            }
		}
        boxes.push_back(line);
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

/* Returns true if all the boxes are on goals. */
bool GameState::isSolution() {
	for(int i = 0;i < boxes.size();i++){
        for(int j = 0;j < boxes[i].size();j++){
            if(!(map->isGoal(pos(j,i))))
                return false;
        }
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
    return true;
}

/* Returns true iff there is a box on the specified position. */
bool isBox(pos p){
    return boxes[p.y][p.x] == BOX;
}

/* Returns the number of boxes on the field. */
int boxCount(){
    int ctr = 0;
    for(int y = 0;y < boxes.size();y++){
        for(int x = 0;x < boxes[y].size();x++){
            if(isBox(pos(x,y)){
                ctr++;
            }
        }
    }
    return ctr;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    vector<vector<char> > & m = *(state.map->getOriginalMap());

    for(int i = 0;i < state.map->getHeight();i++){
		for(int j = 0;j < state.map->getWidth();j++){
            const bool printBox = state.isBox(pos(j,i));
            if(printBox && state.map->isGoal(pos(j,i))){
                stream << "*";
            }else if(printBox){
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
vector<GameState*> GameState::findNextMoves(){
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
    vector<GameState*> sPointers;
    for(int i = 0;i < successors.size();i++){
        sPointers.push_back(&(successors[i]));
    }
    return sPointers;
}

int GameState::heuristic() const{
    set<pos> goals = map->getGoals();
    if(boxes.size() != goals.size()){
        cerr << "Assertion failed: boxes.size() == goals.size()" << endl;
        cerr << "    boxes.size()=" << boxes.size() << ", goals.size()=" << goals.size() << endl;
    }
    return goals.size()-boxes.size();
}

/* Slightly misleading name because it is meant to be used for sorting GameStates in a std::set, not
 * in a hash datastructure. */
//TODO: Ingen garanti för att alla GameStates har en unik hash.
unsigned long long GameState::hash() const{
cout << "EEEEEEEEE" << map->getWidth() << endl;
    unsigned long long hash = 0;
    set<pos>::iterator it;
    int multiplier = 1;
    for(it = boxes.begin();it != boxes.end();++it){
        pos p = *it;
        int posHash = p.x*map->getOriginalMap()->size()+p.y;
        hash += multiplier*posHash;
        multiplier *= 2;
        cout << "size=" << boxes.size() << endl;
    }
cout << "AAAAAAAA" << map->getWidth() << endl;
    return hash;
}



