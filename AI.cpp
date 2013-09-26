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

void solve(GameState * gs) {
	priority_queue<GameState> queue;
	set<GameState> visited; //TODO, fixa egen hashfunction typ

	visited.insert(*gs);
	queue.push(*gs);

	while(!queue.empty) {
		GameState next = queue.top();
		queue.pop();

		if(next.isSolution()) {
			//SOLUTION FOUND, DO SOMETHING USEFUL
			//TODO
			break;
		}


		if(next.isSolution()) {
			//SOLUTION FOUND, DO SOMETHING USEFUL
			//TODO
			break;
		}

		vector<GameState> nextMoves = next.findNextMoves();
		vector<GameState>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState g = *it;
			if(visited.find(g) != visited.end()) {
				visited.insert(g);
				queue.push(g);
			}
		}
	}

	//if we are here, solution has been found


	return; //return something, or do something
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
