#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>
#include <map>
//#include <unordered_map>
#include <queue>
#include <algorithm>
#include "GameState.h"
#include "Constants.h"
#include "Heuristics.h"
#include "DeadlockDetection.h"
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
	map<bitString,vector<pos> > visited;
	vector<pos> tmp;
	tmp.push_back(gs->player);
	visited[gs->hash()] = tmp;
	queue.push(gs);

#ifdef MEASURE_TIME_YES
	double findNextMovesTime = 0;
	double hashingTime = 0;
	double heuristicTime = 0;
	double deadlockTime = 0;
	double hashPathfindingTime = 0;
	int numGameStatesVisited = 0;
#endif
	//int numGameStatesVisited = 0;
	while(!queue.empty()) {
		//fprintf(stderr, "Starting loop iteration\n");
		GameState* next = queue.top(); 
		queue.pop();
		//numGameStatesVisited++;
		
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
				<< "Hashing: " << hashingTime << endl
				<< "Hash pathfinding: " << hashPathfindingTime << endl
				<< "Heuristics: " << heuristicTime << endl 
				<< "Deadlock detection: " << deadlockTime << endl
				<< "We have searched num gamestates: " << numGameStatesVisited << endl;
#endif

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
#ifdef MEASURE_TIME_YES
		double start = omp_get_wtime();
#endif
		vector<GameState*> nextMoves = next->findNextMoves();
		
		if (nextMoves.size() == 0) {
			//fprintf(stderr, "No next moves\n");
		}
		
#ifdef MEASURE_TIME_YES
		double end = omp_get_wtime();
		findNextMovesTime += (end-start);
#endif
		vector<GameState*>::iterator it;
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
#ifdef MEASURE_TIME_YES
			start = omp_get_wtime();
#endif
			bitString hash = g->hash();
#ifdef MEASURE_TIME_YES
			end = omp_get_wtime();
			hashingTime += (end-start);
#endif
			bool forQueue = false;
			map<bitString,vector<pos> >::iterator iter = visited.find(hash);
			//unordered_map<string,vector<pos> >::iterator iter = visited.find(hash);
			if(visited.end() == iter) {
				//Not in visited. Push.
				//fprintf(stderr,"Not in visited at all\n");



				vector<pos> tmp;
				tmp.push_back(gs->player);
				visited[hash] = tmp;
				//fprintf(stderr,"visited.size() is: %d\n", (int)visited.size());
				
/*#ifdef MEASURE_TIME_YES
				end = omp_get_wtime();
				hashingTime += (end-start);
				start = omp_get_wtime();
#endif*/
#ifdef MEASURE_TIME_YES
				start = omp_get_wtime();
#endif
				if(findDynamicDeadlocks(g, g->src.end)) {
					g->score = -100000000;
#ifdef MEASURE_TIME_YES
					end = omp_get_wtime();
					deadlockTime += (end-start);
#endif
					//delete g;
					//cerr << "Deadlock found in position: " << endl << *g;
				} else {
#ifdef MEASURE_TIME_YES
					end = omp_get_wtime();
					deadlockTime += (end-start);
					start = omp_get_wtime();
#endif
					g->score = heuristicEvenBetter(*g);
#ifdef MEASURE_TIME_YES
					end = omp_get_wtime();
					heuristicTime += (end-start);
#endif
				}

				queue.push(g);
			} else {
#ifdef MEASURE_TIME_YES
					start = omp_get_wtime();
#endif
				//Found in set. Check poses.
				forQueue = true;
				vector<pos> poses = iter->second;
				for(int i = 0;i < poses.size() &&forQueue;i++){
		            pos p = poses[i];
					//if(pathExistsAStar(g->player,p,g->board) != pathExists(g->player,p,g->board)) {
					//	cerr << "SHANENIGANS!!!!!!!!" << endl;
					//}


		            //if (g->player == p || pathExists(g->player,p,g->board)) {
					if (g->player == p || pathExistsAStar(g->player,p,g)) {
		                //Already in visited. Don't add to queue.
		                //fprintf(stderr,"Visited, in the same zone\n");
		                forQueue = false;
						//break;
						//goto f;
		            }
            	}
				//f:
#ifdef MEASURE_TIME_YES
				end = omp_get_wtime();
				hashPathfindingTime += (end-start);
#endif
            	if (forQueue) {
            		//Add to list			

#ifdef MEASURE_TIME_YES
					start = omp_get_wtime();
#endif
					if(findDynamicDeadlocks(g, g->src.end)) {
						g->score = -100000000;
#ifdef MEASURE_TIME_YES
						end = omp_get_wtime();
						deadlockTime += (end-start);
#endif
						//delete g;
						//cerr << "Deadlock found in position: " << endl << *g;
					} else {
#ifdef MEASURE_TIME_YES
						end = omp_get_wtime();
						deadlockTime += (end-start);
						start = omp_get_wtime();
#endif
						g->score = heuristicEvenBetter(*g);
#ifdef MEASURE_TIME_YES
						end = omp_get_wtime();
						heuristicTime += (end-start);
#endif
					}         		
	
            		visited[hash].push_back(g->player);
            		queue.push(g);
            	}
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


vector<GameState*> solution(){
    /*
    for (int i = 0;i<board[0].size();i++) {
		board[0][i] = WALL;
		board[board.size()-1][i] = WALL;
	}
    for (int i = 0;i<board.size();i++) {
		board[i][0] = WALL;
		board[i][board[0].size()-1] = WALL;
	}
	*/

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

string sokoban(){
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
    return answer(solution());
#endif
}



