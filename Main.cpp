#include <iostream>
#include <string>
#include <vector>
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

    cout << "Calling solver..." << endl;
	// Call the solver
    vector<GameState> solution = solve(&gs);
    cout << "Solution returned." << endl;

	string s = answer(solution);

	// Output answer
	std::cout << s << std::endl;
	return 0;
}

