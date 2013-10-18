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
	
	//Set globals:
	NR_ROWS = board.size();
	NR_COLUMNS = board[0].size();
	BOARD_SIZE = NR_ROWS*NR_COLUMNS;
	
	//new stuff: Construct bitStrings out of the board
	unsigned int bsWalls = 0, bsBoxes = 0, bsGoals = 0;
	unsigned int bitPos = FIRSTBIT;
	for (int i = 0;i<board.size();i++) {
		for (int j = 0;j<board[i].size();j++) {
			//fprintf(stderr, "%c", board[i][j]);
			
			switch (board[i][j]) {
				case FREE:
				case PLAYER:
					break;		
				case DEADLOCK:
					//Can't happen now anyway
					break;	
				case PLAYER_ON_GOAL:
				case GOAL:
					bsGoals += bitPos;
					break;
				case BOX:
					bsBoxes += bitPos;
					break;
				case BOX_ON_GOAL:
					bsGoals += bitPos;
					bsBoxes += bitPos;
				case WALL:
					bsWalls += bitPos;
			}
			
			bitPos = bitPos >> 1;
			if (bitPos == 0) {
				bitPos = FIRSTBIT;
				WALLS.insert32(bsWalls);
				BOXES.insert32(bsBoxes);
				GOALS.insert32(bsGoals);
				DEADLOCKS.insert32(0);
				fprintf(stderr, "\nPushing back %u, %u and %u\n", bsWalls, bsBoxes, bsGoals);

				
				bsWalls = 0;
				bsBoxes = 0;
				bsGoals = 0;
			}
		}
		fprintf(stderr, "\n");
	}
	
	WALLS.insert32(bsWalls);
	BOXES.insert32(bsBoxes);
	GOALS.insert32(bsGoals);
	DEADLOCKS.insert32(0);
	fprintf(stderr, "\nPushing back %u, %u and %u\n", bsWalls, bsBoxes, bsGoals);
	
	for (int k = 0;k<4;k++){
		fprintf(stderr, "\nPushed back each time: %u, %u and %u\n", WALLS.data[k], BOXES.data[k], GOALS.data[k]);
	}

	//Test print:
	cerr << "WALLS bitString looks like this:\n" << WALLS << endl;
	cerr << "BOXES bitString looks like this:\n" << BOXES << endl;
	cerr << "GOALS bitString looks like this:\n" << GOALS << endl;
	cerr << "DEADLOCKS bitString looks like this:\n" << DEADLOCKS << endl;

    return board;
}

/**
 * Perform unit tests and acceptance tests:
 * ./sokoban test
 * Verify that the solution to maps/test is correct:
 * ./sokoban verify < maps/test
 * Verify that the solution to maps/test is correct (detailed):
 * ./sokoban verifyd < maps/test
 * Test deadlocks:
 * ./sokoban deadlocks < maps/test
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
    }else if(argc == 2 && argv[1] == string("deadlocks")){
        testDeadlocks(board);
    }else{
        cout << sokoban(board) << endl;
    }
    return 0;
}
