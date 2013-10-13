#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <bitset>
#include "GameState.h"
#include "Constants.h"
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
        readable << endl << bitset<8>(hash[i]);
    }
    return readable.str();
}

string hashToBitmap(vector<vector<char> > board){
    stringstream bitmap;
    for(int y = 0;y < board.size();y++){
        for(int x = 0;x < board[y].size();x++){
            char cell = board[y][x];
            if(cell == PLAYER || cell == PLAYER_ON_GOAL){
                bitmap << endl << bitset<8>((char)x) << endl << bitset<8>((char)y);
            }
        }
    }
    int pos = 0;
    for(int y = 0;y < board.size();y++){
        for(int x = 0;x < board[y].size();x++){
            if(pos % 8 == 0){
                bitmap << endl;
            }
            char cell = board[y][x];
            if(cell == BOX || cell == BOX_ON_GOAL){
                bitmap << "1";
            }else{
                bitmap << "0";
            }
            pos++;
        }
    }
    for(;(pos % 8) > 0;pos++){
        bitmap << "0";
    }
    return bitmap.str();
}

void hashAssertEquals(string path){
    const GameState gs = GameState(readFile(path));
    cout << "Testing " << path << "...";
    string hash = readableHash(gs.hash());
    string expected = hashToBitmap(gs.board);
    if(hash != expected){
        cout << endl;
        cout << "Returned: " << hash << ", expected: " << expected << endl;
    }else{
        cout << "passed!" << endl;
    }
}

void runTests() {
    hashAssertEquals("maps/test");
    hashAssertEquals("maps/test2");
    hashAssertEquals("maps/test3");
    hashAssertEquals("maps/test4");
    hashAssertEquals("maps/test5");
}

