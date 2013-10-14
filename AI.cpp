#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "GameState.h"
#include "AI.h"
#include <queue>
#include <set>

class GameState;

using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score - ((g1->depth)*1)) < (g2->score - ((g1->depth)*1));
	}
};

vector<GameState*> aisolve(GameState * gs) {
	priority_queue<GameState*,vector<GameState*>,lex_compare> queue;
	set<string> visited;

	visited.insert(gs->hash());
	queue.push(gs);

	while(!queue.empty()) {
		GameState* next = queue.top(); 
		queue.pop();

		if(next->isSolution()) {
			//Solution found. Return the GameStates in order.
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
		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
			if(visited.find(g->hash()) == visited.end()) {
				visited.insert(g->hash());
				queue.push(g);
			}
		}
	}
	
	//if we are here, something is wrong.
	return vector<GameState*>();
}

