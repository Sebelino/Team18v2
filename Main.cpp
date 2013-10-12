#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "AI.h"
#include "Constants.h"
#include "Map.h"

using namespace std;

string answer(vector<GameState*> path){
    string directions = "";
    vector<char> segment;
    for(int i = 0;i+1 < path.size();i++){
        segment = moveToPath(path[i],path[i+1]->src);
        directions = directions+string(segment.begin(),segment.end());
    }
    return directions;
}

int main(int argc, char **argv) {
	
	// Read the board
	vector<vector<char> > board;
	unsigned int width = 0;
	unsigned int height = 0;

	pos player;

	//Read the Sokoban level from standard input
	vector<char> line;
	for (char input = getchar(); input != EOF ;input = getchar()) {
		if (input == '\n') {
			board.push_back(line);
			line.clear();
		} else {
			if (input == PLAYER || input == PLAYER_ON_GOAL) {
				player = pos(line.size(), board.size());
			}
			line.push_back(input);
		}
		if (line.size() > width) {
			width = line.size();
		}
	}

	//Add padding to avoid indexing errors
	for (int i = 0;i<board.size();i++) {
		for (int j = board[i].size();j<width;j++) {
			board[i].push_back(FREE);
		}
	}
	
	//fprintf(stdout, "player pos: %d, %d\n", player.x, player.y);
	//Removing the player from the matrix describing the level
	if (board[player.y][player.x] == PLAYER) {
		board[player.y][player.x] = FREE;
		//fprintf(stdout, "Change player to free\n");
	} else if (board[player.y][player.x] == PLAYER_ON_GOAL) {
		board[player.y][player.x] = GOAL;
		//fprintf(stdout, "Change player_on_goal to goal\n");
	}
	

	height = board.size();

    Map map(board,width,height);
    map.findStaticDeadLocks();
    
	// Create gamestate
	GameState gs = GameState(board, player);
    //cerr << "Initial GameState hash = " << gs.hash() << endl;
    //cerr << "Initial heuristic = " << gs.heuristic() << endl;
    //cerr << "Initial GameState apparence =\n" << gs << endl;
	
	

	//call the solver
	vector<char> str;
	fprintf(stderr,"0\n");
	vector<GameState*> solution;
	//solution = solve(&gs);
	fprintf(stderr,"1\n");
	//vector<string> movements(solution.size());
	fprintf(stderr,"2\n");

	// Output answer.
    //cout << answer(solution) << endl;
	
	fprintf(stderr,"3\n");

	return 0;
}

