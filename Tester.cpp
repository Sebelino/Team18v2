#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <bitset>
#include "GameState.h"
#include "AI.h"
#include "Sokoban.h"

using namespace std;

vector<vector<char> > readFile(string path){
    vector<vector<char> > board;
    string line;
    ifstream file (path.c_str());
    if(file.is_open()){
        while(getline(file,line)){
            vector<char> row;
            for(int i = 0;i < line.length();i++){
                row.push_back(line[i]);
            }
            board.push_back(row);
        }
        file.close();
    }else{
        cerr << "Could not open file: " << path << endl;
    }
    return board;
}

/**
 * Returns the hash in a more readable format, e.g.
 * "abd" |->  "97 98 100" */
string readableHash(string hash){
    if(hash.empty()){
        return "";
    }
    stringstream readable;
    for(int i = 0;i < hash.length();i++){
        readable << "\n" << bitset<8>(hash[i]);
    }
    return readable.str();
}

void hashAssertEquals(string path,string expected){
    const GameState gs = GameState(readFile(path));
    cout << "Testing " << path << "...";
    string hash = readableHash(gs.hash());
    if(hash != expected){
        cout << endl;
        cout << "Returned: " << hash << ", expected: " << expected << endl;
    }else{
        cout << "passed!" << endl;
    }
}

void runTests() {
    hashAssertEquals("maps/test", "\n00000010\n00000001\n00000000\n00000000\n00000000\n00000010\n00000000\n00000000\n00000000");
    hashAssertEquals("maps/test3","\n00000010\n00000001\n00000000\n00000000\n00100000\n00000010\n00000000\n00000000\n00000000");
    hashAssertEquals("maps/test5","\n00000001\n00000000\n00010010\n00000000");
}

