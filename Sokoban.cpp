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
//#include "Map.h"


//uncomment the line below to measure time, works in VS.
//other than that please dont touch these macros.
//#define MEASURE_TIME_YES
#ifdef MEASURE_TIME_YES
#include <omp.h>
#endif

using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score) < (g2->score);
	}
};

vector<GameState*> solve(GameState * gs) {
	priority_queue<GameState*,vector<GameState*>,lex_compare> queue;
	set<string> visited;

	visited.insert(gs->hash());
	queue.push(gs);

#ifdef MEASURE_TIME_YES
	double findNextMovesTime = 0;
	double hashingTime = 0;
	double heuristicTime = 0;
	int numGameStatesVisited = 0;
#endif

	while(!queue.empty()) {
		GameState* next = queue.top(); 
		queue.pop();

		//cerr << "NEXT GAMESTATE IS " << endl;
		//cerr << *next;
#ifdef MEASURE_TIME_YES
		numGameStatesVisited++;
#endif


		if(next->isSolution()) {
			//Solution found. Return the GameStates in order.
#ifdef MEASURE_TIME_YES
			cerr << "Solution found! Total time taken for each task: " << endl 
				<< "FindNextMoves: " << findNextMovesTime << endl
				<< "Hashing and inserting: " << hashingTime << endl
				<< "Heuristics: " << heuristicTime << endl 
				<< "We have searched num gamestates: " << numGameStatesVisited << endl;
#endif
				


			vector<GameState*> retv;
			GameState * gsp = next->parent;
			retv.insert(retv.begin(),next);
			while(gsp != NULL) {
				retv.insert(retv.begin(),gsp);
                gsp = gsp->parent;
			}
			return retv;
		}
#ifdef MEASURE_TIME_YES
		double start = omp_get_wtime();
#endif
		vector<GameState*> nextMoves = next->findNextMoves();
#ifdef MEASURE_TIME_YES
		double end = omp_get_wtime();
		findNextMovesTime += (end-start);
#endif
		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
			if(visited.find(g->hash()) == visited.end()) {
#ifdef MEASURE_TIME_YES
				start = omp_get_wtime();
#endif
				visited.insert(g->hash());
#ifdef MEASURE_TIME_YES
				end = omp_get_wtime();
				hashingTime += (end-start);

				start = omp_get_wtime();
#endif
				g->score = heuristicEvenBetter(*g);
#ifdef MEASURE_TIME_YES
				end = omp_get_wtime();
				heuristicTime += (end-start);
#endif
				queue.push(g);
			}
		}
	}
	
	//if we are here, something is wrong.
	cerr << "Answer not found " << endl;
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
	GameState* gs = new GameState(board);
    //cerr << "Initial GameState hash = " << gs.hash() << endl;
    //cerr << "Initial heuristic = " << gs.heuristic() << endl;
    //cerr << "Initial GameState appearence =\n" << gs << endl;

	//call the solver
	vector<char> str;
	//fprintf(stderr,"0\n");
	vector<GameState*> solution = solve(gs);

	//fprintf(stderr,"1\n");
	vector<string> movements(solution.size());
	//fprintf(stderr,"2\n");

	// Output answer.
	//fprintf(stderr,"3\n");
	// Return answer.
    return solution;
}

string sokoban(vector<vector<char> > board){
#ifdef MEASURE_TIME_YES
	double start = omp_get_wtime();
	vector<GameState*> ans = solution(board);
	double end = omp_get_wtime();
	cerr << "Finding answer took " << (end-start )<< endl;

	start = omp_get_wtime();
	string s = answer(ans);
	end = omp_get_wtime();
	cerr << "Lapping ihop answer took " << (end-start ) << endl;

	return s;
#else
    return answer(solution(board));
#endif
}



