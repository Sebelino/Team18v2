#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "GameState.h"
#include "AI.h"

class GameState;

void solve(GameState gs) {
	return;
}

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


