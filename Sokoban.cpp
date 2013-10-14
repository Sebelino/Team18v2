#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>
#include <queue>
#include <algorithm>
#include "GameState.h"
#include "Constants.h"
//#include "Map.h"

using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score - ((g1->depth)/2)) < (g2->score - ((g1->depth)/2));
	}
};

vector<GameState*> solve(GameState * gs) {
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


/** Prints the directions (e.g. "RRURU" ). */
string answer(vector<GameState*> path){
    string directions = "";
    vector<char> segment;
    for(int i = 0;i+1 < path.size();i++){
        segment = moveToPath(path[i],path[i+1]->src);
        directions = directions+string(segment.begin(),segment.end());
    }
    return directions;
}


vector<GameState*> solution(vector<vector<char> > board){
    for (int i = 0;i<board[0].size();i++) {
		board[0][i] = WALL;
		board[board.size()-1][i] = WALL;
	}
    for (int i = 0;i<board.size();i++) {
		board[i][0] = WALL;
		board[i][board[0].size()-1] = WALL;
	}
	
	// Create gamestate
	GameState gs = GameState(board);
    //cerr << "Initial GameState hash = " << gs.hash() << endl;
    //cerr << "Initial heuristic = " << gs.heuristic() << endl;
    //cerr << "Initial GameState appearence =\n" << gs << endl;

	//call the solver
	vector<char> str;
	//fprintf(stderr,"0\n");
	vector<GameState*> solution = solve(&gs);

	//fprintf(stderr,"1\n");
	vector<string> movements(solution.size());
	//fprintf(stderr,"2\n");

	// Output answer.
	//fprintf(stderr,"3\n");
	// Return answer.
    return solution;
}

string sokoban(vector<vector<char> > board){
    return answer(solution(board));
}



