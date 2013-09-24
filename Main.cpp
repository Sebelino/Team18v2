#include <iostream>
#include <string>
#include <vector>
#include "GameState.h"
//#include "AI.cpp"


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

	// Create gamestate
	GameState gs = GameState(board,width,height);

	//call the solver
	string s = "U R R U";

	// Output answer
	std::cout << s << std::endl;
	return 0;
}
