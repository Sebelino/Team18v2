#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "GameState.h"
#include "AI.h"
#include <queue>
#include <set>

class GameState;
class Heuristic; //TODO!!!!

using namespace std;

vector<GameState> solve(GameState * gs) {
    cout << *gs << std::endl;
	priority_queue<GameState> queue;
	set<unsigned long long> visited; //TODO, fixa egen hashfunction typ

	visited.insert(gs->hash());
	queue.push(*gs);

	while(!queue.empty()) {
		GameState next = queue.top();
		queue.pop();

		if(next.isSolution()) {
			//backtrack through the parent members of the gamestates to place full
			//solution into a vector
			vector<GameState> retv;
			GameState * gsp = next.parent;
			retv.insert(retv.begin(),next);
			while(gsp != 0) {
				retv.insert(retv.begin(),*gsp);
			}

			return retv;
		}

		vector<GameState> nextMoves = next.findNextMoves();
        cout << "findnextmo=" << nextMoves.size() << endl;
        for(int i = 0;i < nextMoves.size();i++){
            cout << nextMoves[i] << endl;
        }
		vector<GameState>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState g = *it;
			if(visited.find(g.hash()) != visited.end()) {
				g.parent = &next;
				visited.insert(g.hash());
				queue.push(g);
			}
		}
	}

	//if we are here, solution has been found


	return vector<GameState>(); //return something, or do something
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
