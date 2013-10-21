#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>
#include <queue>
#include <algorithm>
#include "GameState.h"
#include "Constants.h"
#include "Heuristics.h"
#include "DeadlockDetection.h"


using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score) < (g2->score);
	}
};

vector<GameState*> solve(GameState * gs) {
	priority_queue<GameState*,vector<GameState*>,lex_compare> queue;
	set<bitString> visited;
	visited.insert(gs->hash());
	queue.push(gs);

	//int numGameStatesVisited = 0;
	while(!queue.empty()) {
		//fprintf(stderr, "Starting loop iteration\n");
		GameState* next = queue.top(); 
		queue.pop();
		//numGameStatesVisited++;
		
		//cout << "NEXT GAMESTATE IS " << endl;
		//cerr << *next;
		//cout << next->boxes;

		if(next->isSolution()) {
			//Solution found. Return the GameStates in order.
			//cerr << "We have searched num gamestates: " << numGameStatesVisited << endl;

			vector<GameState*> retv;
			GameState * gsp = next->parent;
			retv.insert(retv.begin(),next);
			while(gsp != NULL) {
				retv.insert(retv.begin(),gsp);
                gsp = gsp->parent;
			}
			return retv;
		}

		vector<GameState*> nextMoves = next->findNextMoves();
		
		if (nextMoves.size() == 0) {
			//fprintf(stderr, "No next moves\n");
		}

		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
			bitString hash = g->hash();
			bool forQueue = false;
			set<bitString>::iterator iter = visited.find(hash);
			//unordered_map<string,vector<pos> >::iterator iter = visited.find(hash);
			if(visited.end() == iter) {
				//Not in visited. Push.
				//fprintf(stderr,"Not in visited at all\n");
				visited.insert(hash);
				//fprintf(stderr,"visited.size() is: %d\n", (int)visited.size());
				if(findDynamicDeadlocks(g, g->src.end)) {
					g->score = -100000000;
					//delete g;
					//cerr << "Deadlock found in position: " << endl << *g;
				} else {
					g->score = heuristicEvenBetter(*g);
				}
				queue.push(g);
			}
		}
	}
	
	//if we are here, something is wrong.
	cerr << "Answer not found " << endl;
	return vector<GameState*>();
}



void sokoban(){
    for (int i = 0;i<NR_COLUMNS;i++) {
    	WALLS.set(0,i);
    	WALLS.set(NR_ROWS-1,i);
	}
    for (int i = 0;i<NR_ROWS;i++) {
    	WALLS.set(i,0);
    	WALLS.set(i,NR_COLUMNS-1);
	}

	// Create gamestate
	GameState* gs = new GameState(PLAYER_START);
    //cerr << "Initial GameState hash = " << gs.hash() << endl;
    //cerr << "Initial heuristic = " << gs.heuristic() << endl;
    //cerr << "Initial GameState appearence =\n" << gs << endl;

	//call the solver
	vector<char> str;
	vector<GameState*> solution = solve(gs);
	vector<string> movements(solution.size());

    vector<char> segment;
    for(int i = 0;i+1 < solution.size();i++){
        segment = moveToPath(solution[i],solution[i+1]->src);
        for(int j = 0;j < segment.size();j++){
			putchar(segment[j]);
        }
    }
	putchar('\n');
}



