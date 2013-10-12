#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include "GameState.h"
#include "AI.h"
#include "Sokoban.h"

using namespace std;

/**
 * Returns the hash in a more readable format, e.g.
 * "abd" |->  "97 98 100" */
string readableHash(string hash){
    if(hash.empty()){
        return "";
    }
    stringstream readable;
    readable << (unsigned int)hash[0];
    for(int i = 1;i < hash.length();i++){
        readable << " " << (unsigned int)hash[i];
    }
    return readable.str();
}

void hashAssertEquals(const GameState gs,string expected){
    string hash = gs.hash();
    if(hash != expected){
        cout << "Returned: " << readableHash(gs.hash()) << ", expected: " << expected << endl;
    }
    cout << "GameState passed." << endl;
}

void runTests() {
	vector<vector<char> > board = readBoard();

	const GameState gs = GameState(board);
    hashAssertEquals(gs,"");
}

