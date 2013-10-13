#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "GameState.h"
#include "AI.h"
#include <queue>
#include <set>
//#include <unistd.h>         // Hmpf; finns inte på VS.

class GameState;
class Heuristic; //TODO!!!!

using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score) < (g2->score);
	}
};


vector<GameState*> solve(GameState * gs) {
    //cerr << "GS "<< gs << std::endl;
	//cerr << gs->parent << endl;

	priority_queue<GameState*,vector<GameState*>,lex_compare> queue;
	set<string> visited; //TODO, fixa egen hashfunction typ

	//cerr << "gs=\n" << *gs << endl;

	visited.insert(gs->hash());
	queue.push(gs);

    //cerr << "Prepare to enter the loop of delirium!!!11" << endl;
	while(!queue.empty()) {
		//cerr << "TOP: " << queue.top() << endl;
		GameState* next = queue.top(); 
		queue.pop();
		//cerr << "NEXT: " << next << endl;
		cerr << "NEXT GAMESTATE:\n" << *next;
		cerr << "Heuristic score: " << (*next).score << endl;

		if(next->isSolution()) {
            //cerr << "next" << &next << endl;
            //cerr << "next.parent" << next.parent << endl;
            //cerr << "ISSOULUTION ENTERED" << endl;
			//backtrack through the parent members of the gamestates to place full
			//solution into a vector
			vector<GameState*> retv;
			GameState * gsp = next->parent;
			retv.insert(retv.begin(),next);
            //cerr << "Prepare to enter the loop of eternal self-reproduction!!!11" << endl;
			while(gsp != NULL) {
                //cerr << *gsp << ";Parent:\n" << *(gsp->parent) << endl;
				retv.insert(retv.begin(),gsp);
                //usleep(50000);
                //cerr << gsp << endl;
                gsp = gsp->parent;
			}
			return retv;
		}
		vector<GameState*> nextMoves = next->findNextMoves();
        //cerr << "findnextmo=" << nextMoves.size() << endl;
		//cerr << "parent should be " << next << endl;
        for(int i = 0;i < nextMoves.size();i++){
            cerr << "NEXTMOVES RETURNED\n" <<*(nextMoves[i]) << endl;
        }
		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) { // ++it eller it++?
			GameState* g = *it;
            //cerr << "g.hash=" << g.hash() << std::endl;
            //for(set<unsigned long long>::iterator it=visited.begin();it!=visited.end();++it){
            //    cout << "visited:" << *it << endl;
            //}
			if(visited.find(g->hash()) == visited.end()) {
                //cerr << "if ENTERED!" << std::endl;
				//g.parent = &next;
				//cerr << "inserting " << g << " with parent " << g->parent << endl;
				visited.insert(g->hash());
				queue.push(g);
			}
		}
		
	}

	//if we are here, solution has been found
    cerr << "WE ARE OUTTA HELL!" << endl;
	return vector<GameState*>(); //return something, or do something
}

