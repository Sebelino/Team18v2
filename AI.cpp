#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "GameState.h"
#include "AI.h"
#include <queue>
#include <set>
#include <unistd.h>         // Hmpf; finns inte på VS.

class GameState;
class Heuristic; //TODO!!!!

using namespace std;

vector<GameState*> solve(GameState * gs) {
    cout << "GS "<< gs << std::endl;
	cout << gs->parent << endl;
	priority_queue<GameState*> queue;
	set<unsigned long long> visited; //TODO, fixa egen hashfunction typ

	visited.insert(gs->hash());
	queue.push(gs);

    cout << "Prepare to enter the loop of delirium!!!11" << endl;
	while(!queue.empty()) {
		cout << "TOP: " << &(queue.top()) << endl;
		GameState* next = queue.top(); 
		cout << "NEXT: " << next << endl;

		if(next->isSolution()) {
            //cout << "next" << &next << endl;
            //cout << "next.parent" << next.parent << endl;
            //cout << "ISSOULUTION ENTERED" << endl;
			//backtrack through the parent members of the gamestates to place full
			//solution into a vector
			vector<GameState*> retv;
			GameState * gsp = next->parent;
			retv.insert(retv.begin(),next);
            cout << "Prepare to enter the loop of eternal self-reproduction!!!11" << endl;
			while(gsp != NULL) {
                //cout << *gsp << ";Parent:\n" << *(gsp->parent) << endl;
				retv.insert(retv.begin(),gsp);
                usleep(50000);
                cout << gsp << endl;
                gsp = gsp->parent;
			}
			return retv;
		}

		vector<GameState*> nextMoves = next->findNextMoves();
        //cout << "findnextmo=" << nextMoves.size() << endl;
		cout << "parent should be " << next << endl;
        for(int i = 0;i < nextMoves.size();i++){
            cout << nextMoves[i] << "     " << nextMoves[i]->parent << endl;
        }
		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
            //cout << "g.hash=" << g.hash() << std::endl;
            //for(set<unsigned long long>::iterator it=visited.begin();it!=visited.end();++it){
            //    cout << "visited:" << *it << endl;
            //}
			if(visited.find(g->hash()) == visited.end()) {
                //cout << "if ENTERED!" << std::endl;
				//g.parent = &next;
				cout << "inserting " << g << " with parent " << g->parent << endl;
				visited.insert(g->hash());
				queue.push(g);
			}
		}
		queue.pop();
	}

	//if we are here, solution has been found
    cout << "WE ARE OUTTA HELL!" << endl;
	return vector<GameState*>(); //return something, or do something
}
/*
int abprune(GameState curGS, int depth, int a, int b, bool maxi)
{
    if (depth == 0) { // || curGS.isEOG()) {
        //fprintf(stderr, "Entered first if. In abprune now with depth %i\n", depth);
        //if (curGS.isDraw()) {
        //    return 0;
        //}
        return 0;
        //return evalHeur(curGS);
    }
 
    //fprintf(stderr, "In abprune now with depth %i\n", depth);
 
    std::set<GameState> s = curGS.findNextMoves();
    std::vector<GameState> lNextStates(s.begin(),s.end());
    
    int vecSize = lNextStates.size();
 
    if (maxi) { //Maximizing player's turn
        for (int i = 0;i<vecSize;i++) {
            a = std::max(a, abprune(lNextStates[i], depth-1, a, b, !maxi));
            if (b <= a) {
                break;
            }
        }
        return a;
    } else {    //Minimizing player's turn
        for (int i = 0;i<vecSize;i++) {
            b = std::min(b, abprune(lNextStates[i], depth-1, a, b, !maxi));
            if (b <= a) {
                break;
            }
        }
        return b;
    }
}

*/
