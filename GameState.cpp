#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
#include <sstream>
#include <iterator>
#include <queue>
#include "GameState.h"
#include "Constants.h"


using namespace std;

GameState::GameState(Map * canvas, vector<vector<char> > b) {
	board = b;
	map = canvas;
	Map& m = *canvas;
	board = *(m.getOriginalMap());
	setBoxes(&board);	//Soon obsolete
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
	boxes.erase(box_move->start);
	boxes.insert(box_move->end);
	map = prev->map;
	//^ Soon obsolete
	
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
    
}

/*
Private method to set all boxes from a board, and also set player position.
Only called from constructor.
*/
//Soon obsolete
void GameState::setBoxes(vector<vector<char> > * stringmap) {
	vector<vector<char> >& b = *stringmap;
	
	for(unsigned int i = 0; i < b.size(); i++) {
		for(unsigned int j = 0; j < b[i].size(); j++) {
			char c = board[i][j];
			if(c == BOX){
                boxes.insert(pos(j,i));
				//b[i][j] = FREE; // Overwrite dynamic entity.
			}else if(c == BOX_ON_GOAL){
				boxes.insert(pos(j,i));
                //b[i][j] = GOAL;
			}else if(c == PLAYER){
				player = pos(j,i);
                //b[i][j] = FREE;
            }else if(c == PLAYER_ON_GOAL) {
				player = pos(j,i);
				//b[i][j] = GOAL; // Overwrite dynamic entity.
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

	for (int i = 0;i<(int)board.size();i++) {
		for (int j = 0;j<(int)board[i].size();j++) {
			if (board[i][j] == BOX || board[i][j] == GOAL) {
				return false;
			}
		}
	}
	/*
	set<pos>::iterator it;
	for(it = boxes.begin(); it != boxes.end() ; it++) {
		if(!(map->isGoal(*it)))
			return false;
	}
	*/
	return true;
}

/* Returns true iff it is possible to move a box in the way specified in the argument. */
bool GameState::isValid(const struct boxMove & m){
    vector<char> path = moveToPath(this,m);
    if(path[0] == 'X'){
    	fprintf(stderr, "Contained an X!!!!!!!!!!!\n");
        return false;
    }
    if(map->isDeadlock(m.end) || map->isWall(m.end)){
        return false;
    }
    return true;
}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    vector<vector<char> > & m = *(state.map->getOriginalMap());

    for(int i = 0;i < state.map->getHeight();i++){
		for(int j = 0;j < state.map->getWidth();j++){
            const bool printBox = state.boxes.find(pos(j,i)) != state.boxes.end();
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
//Soon obsolete
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
		fprintf(stderr, "dirMap:\n");
		for (int i = 0;i<dirMap.size();i++) {
			for (int j = 0;j<dirMap[i].size();j++) {
				fprintf(stderr, "%c", dirMap[i][j]);
			}
			fprintf(stderr, "\n");
		}
        //____________
        
	    
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

	/*
    set<pos>::iterator it;
    for(it = boxes.begin();it != boxes.end();++it){
        pos b = *it;
        set<boxMove> ms = moves(b);
        set<boxMove>::iterator it2;
        for(it2 = ms.begin();it2 != ms.end();++it2){
            boxMove m = *it2;
            GameState * gs = new GameState(this,&m);
            successors.push_back(gs);
        }
    }
    */
    return successors;
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



