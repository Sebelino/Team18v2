#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "AI.h"

using namespace std;

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
	
	vector<GameState> solution = solve(&gs);
	vector<string> movements(solution.size());
	
	// Output answer
	for (int i = 0;i<solution.size(); i++) {
		str = moveToPath(&(solution[i]),solution[i+1].src);
		for (int j = 0;j<str.size(); j++) {
			printf("%c",str[j]);
		}
	}
	putchar('\n');

	return 0;
}

