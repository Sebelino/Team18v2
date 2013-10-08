#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "AI.h"

using namespace std;

string answer(vector<GameState*> path){
    string directions = "";
    vector<char> segment;
    for(int i = 0;i < path.size()-1;i++){
        segment = moveToPath(path[i],path[i+1]->src);
        directions = directions+string(segment.begin(),segment.end());
    }
    return directions;
}

int main(int argc, char **argv) {
	
	// Read the board
	vector<string> board;
	unsigned int width = 0;
	unsigned int height = 0;
	for (string line; getline(cin, line);) {
		board.push_back(line);
		if(line.length() > width)
			width = line.length();
	}
	height = board.size();

    Map map = Map(board,width,height);
	// Create gamestate
	GameState gs = GameState(&map);
    cerr << "Initial GameState hash = " << gs.hash() << endl;
    cerr << "Initial heuristic = " << gs.heuristic() << endl;
    cerr << "Initial GameState apparence =\n" << gs << endl;
	
	map.findStaticDeadLocks();

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

	return 0;
}

