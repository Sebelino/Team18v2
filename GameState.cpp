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
#include "DeadlockDetection.h"

using namespace std;

GameState::GameState(pos player_start) {
	player = player_start;
	boxes = BOXES;
	boxMove s;
	s.start.x = -1;
	s.start.y = -1;
	s.end.x = -1;
	s.end.y = -1;
	depth = 0;
	src = s;
    parent = NULL;
	findStaticDeadLocks();
	score = 0;
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
	boxes = prev->boxes;
	
	//fprintf(stderr, "Before moving box: Number of boxes: %d\n", sumbefore);
	//Alter box configuration
	//fprintf(stderr, "Before set: %d\n", boxes.get(src.end.y, src.end.x));
	boxes.set(src.end.y, src.end.x);
	boxes.reset(src.start.y, src.start.x);

}


// Destructor.
GameState::~GameState(){}


/* Returns true if the gamestate is a solution */
bool GameState::isSolution() {

	//If the boxes are in the same places as the goals.
	if (boxes == GOALS) {
		return true;
	} else {
		return false;
	}

}

/* Enables you to do cout << gamestate;. */
ostream& operator<<(ostream &strm, const GameState &state) {
	std::ostream& stream = strm;
	bitString b = state.boxes;
    stream << b.toString() << endl;
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

	int npx, npy;
	vector<pos> vvec = visitedList.getPosVector(BOARD_SIZE);
	npx = vvec[0].x;
	npy = vvec[0].y;
	normPlayer = (npx << 16) + npy;

    return successors;
}


/**
 * Format:
 * "<player.x><player.y><segment>...<segment>"
 * where <segment>...<segment> constitue a bit map for all boxes.
 **/
bitString GameState::hash() const {
	bitString hash = boxes;
	hash.insert32(normPlayer);
	return boxes;
}

