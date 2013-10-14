#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "Sokoban.h"
#include "Tester.h"
#include "Constants.h"

using namespace std;

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

/**
 * Perform unit tests and acceptance tests:
 * ./sokoban test
 * Verify that the solution to maps/test is correct:
 * ./sokoban verify < maps/test
 * Verify that the solution to maps/test is correct (detailed):
 * ./sokoban verifyd < maps/test
 * Run the program normally:
 * ./sokoban < maps/test
 */
int main(int argc, char **argv) {
    vector<vector<char> > board = readBoard();
    if(argc == 2 && argv[1] == string("test")){
        unitTest();
    }else if(argc == 2 && argv[1] == string("verify")){
        verify(board,false);
    }else if(argc == 2 && argv[1] == string("verifyd")){
        verify(board,true);
    }else{
        cout << sokoban(board) << endl;
    }
    return 0;
}
