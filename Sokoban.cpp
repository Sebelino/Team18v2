#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "AI.h"
#include "Constants.h"
#include "Map.h"
#include "DeadlockDetection.h"

using namespace std;

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

vector<vector<char> > readBoard(){
	vector<vector<char> > board;
	unsigned int width = 0;
	vector<char> line;
	for (char input = getchar(); input != EOF ;input = getchar()) {
		if (input == '\n') {
			board.push_back(line);
			line.clear();
		} else {
			line.push_back(input);
		}
		if (line.size() > width) {
			width = line.size();
		}
	}
	// Add padding to avoid indexing errors.
	for (int i = 0;i<board.size();i++) {
		for (int j = board[i].size();j<width;j++) {
			board[i].push_back(FREE);
		}
	}
    return board;
}

void sokoban(){
    vector<vector<char> > board = readBoard();
    
    for (int i = 0;i<board[0].size();i++) {
		board[0][i] = WALL;
		board[board.size()-1][i] = WALL;
	}
    for (int i = 0;i<board.size();i++) {
		board[i][0] = WALL;
		board[i][board[0].size()-1] = WALL;
	}
	
    //Find static Deadlocks
    //findStaticDeadLocks(board);
    
	// Create gamestate
	GameState gs = GameState(board);
    cerr << "Initial GameState hash = " << gs.hash() << endl;
    cerr << "Initial heuristic = " << gs.heuristic() << endl;
    cerr << "Initial GameState apparence =\n" << gs << endl;

	//call the solver
	vector<char> str;
	fprintf(stderr,"0\n");
	vector<GameState*> solution = solve(&gs);

	fprintf(stderr,"1\n");
	vector<string> movements(solution.size());
	fprintf(stderr,"2\n");

	// Output answer.
    cout << answer(solution) << endl;
	fprintf(stderr,"3\n");
	
}

