//#include <iostream>
//#include <string>
//#include <vector>
//#include <cstdio>
//#include "GameState.h"
//#include "Constants.h"
//#include "PathFinding.h"
//
//using namespace std;
//
//class GameState;
//
////Reads the board from stdin. Similar function as in sokoban.cpp
//vector<vector<char> > readBoard(char &solFirst){
//	vector<vector<char> > board;
//	unsigned int width = 0;
//	vector<char> line;
//	for (char input = getchar(); input != EOF ;input = getchar()) {
//		if (input == '\n') {
//			board.push_back(line);
//			line.clear();
//		} else if (input == 'U'||input == 'R'||input == 'L'||input == 'D') {
//			solFirst = input;
//			return board;
//		} else {
//			line.push_back(input);
//		}
//		if (line.size() > width) {
//			width = line.size();
//		}
//	}
//
//    return board;
//}
//
///**
// * This function reads a level and an (unverified) solution from stdin.
// * It then determines whether the solution is correct or incorrect.
// */
//bool verify() {
//	char input = ' ';
//    vector<vector<char> > board = readBoard(input);
//
//    if (input == ' ') {
//    	//There was no solution input
//    	fprintf(stderr, "No solution provided.\n");
//    	return false;
//    }
//
//	// Create a first gamestate
//	GameState gs = GameState(board);
//	pos player = gs.player;
//	pos dir;
//	
//	//Start reading the solution
//	for (; input != EOF ;input = getchar()) {
//		//fprintf(stderr, "In for loop; input is now %c\n", input);
//		if (input != 'U' && input != 'R' && input != 'L' && input != 'D') {
//			//Non-move input. Could be space or newline
//			continue;
//		}
//		dir = direction(input);
//		//fprintf(stderr, "direction is %d %d\n", dir.x, dir.y);
//		gs.makeMove(dir);
//	}
//	
//	if (gs.isSolution()) {
//		fprintf(stderr, "Solution is correct!\n");
//		fprintf(stderr, "The final state looks like:\n");
//		cerr << gs;
//		return true;
//	} else {
//		fprintf(stderr, "Solution is not correct!\n");
//		fprintf(stderr, "The final state looks like:\n");
//		cerr << gs;
//		return false;
//	}
//}

