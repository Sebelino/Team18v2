#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "AI.h"

using namespace std;

string answer(vector<GameState> path){
    string directions = "";
    vector<char> segment;
    for(int i = 0;i < path.size()-1;i++){
        segment = moveToPath(&path[0],path[1].src);
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
    cout << "Initial GameState hash = " << gs.hash() << endl;
    cout << "Initial heuristic = " << gs.heuristic() << endl;
    cout << gs;
	
	map.findStaticDeadLocks();

	//call the solver
	string s = "U R R U";
	vector<char> str;
	
	fprintf(stderr,"0\n");
	
	vector<GameState*> solution = solve(&gs);
	
	fprintf(stderr,"1\n");
	
	vector<string> movements(solution.size());
	
	fprintf(stderr,"2\n");

	// Output answer
	for (int i = 0;i<(int)solution.size()-1; i++) {
        cout << "i=" << i << std::endl;
        cout << "solutionsize=" << solution.size() << std::endl;
		str = moveToPath((solution[i]),solution[i+1]->src);
		for (int j = 0;j<str.size(); j++) {
			printf("%c",str[j]);
		}
	}
	putchar('\n');
	
	fprintf(stderr,"3\n");

	return 0;
}

