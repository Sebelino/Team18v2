#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>

#include <queue>
#include <map>
#include <algorithm>
#include "GameState.h"
#include "Constants.h"
#include "PathFinding.h"
#include "Heuristics.h"

using namespace std;

struct lex_compare {
	bool operator() (GameState*& g1, GameState*& g2) const{
		return (g1->score) < (g2->score);
	}
};

vector<GameState*> solve(GameState * gs) {
	priority_queue<GameState*,vector<GameState*>,lex_compare> queue;
	map<string,vector<pos> > visited;

    vector<pos> initPositions; // TODO: new?
    initPositions.push_back(gs->player);
	visited[gs->hash()] = initPositions;

	queue.push(gs);
cerr << "Pushed, size=" << queue.size() << " " << gs << endl << *gs << endl;

	while(!queue.empty()) {
		GameState* next = queue.top(); 
		queue.pop();
cerr << "Popped, size=" << queue.size() << " " << next << endl << *next << endl;
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
        // for g in next:
        //     if g.hash not in visited:
        //         visited[g.hash] = [g.player]
        //     for p in visited[g.hash]:
        //        if g != p and pathExists(g.player,p,g.board):
        //            visited[g.hash] += p
        //        else:
        //            queue += g
		for(it = nextMoves.begin(); it != nextMoves.end(); it++) {
			GameState* g = *it;
			// If g->hash() not in visited...
			map<string,vector<pos> >::iterator iter = visited.find(g->hash());if(visited.end() == iter){ 
                // ...Insert entry (g->hash) -> (g->player).
                vector<pos> positionList; // TODO: new?
                positionList.push_back(g->player);
                visited[g->hash()] = positionList;
            }
            vector<pos> poses = visited[g->hash()];
            for(int i = 0;i < poses.size();i++){
                pos p = poses[i];
                if(!(g->player == p) && pathExists(g->player,p,g->board)){
                    visited[g->hash()].push_back(g->player);
                }else{
                    queue.push(g);
cerr << "Pushed, size=" << queue.size() << " " << g << endl << *g << endl;
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
    return answer(solution(board));
}



