#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <sstream>
#include <bitset>
#include "GameState.h"
#include "Constants.h"
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
    for(int y = 1;y < board.size()-1;y++){
        for(int x = 1;x < board[y].size()-1;x++){
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
    string returned = readableHash(gs.hash());
    string expected = hashToBitmap(readFile(path));
    if(returned != expected){
        cout << endl;
        cout << "Returned: " << returned << ", expected: " << expected << endl;
    }else{
        cout << "passed!" << endl;
    }
}

void unitTest() {
    hashAssertEquals("maps/test");
    hashAssertEquals("maps/test2");
    hashAssertEquals("maps/test3");
    hashAssertEquals("maps/test4");
    hashAssertEquals("maps/test5");
    hashAssertEquals("maps/test6");
}

vector<vector<char> > applyDirection(vector<vector<char> > board,char direction){
    vector<vector<char> > b = board;
    int deltaX = 0;
    int deltaY = 0;
    if(direction == 'R'){
        deltaX = 1; deltaY = 0;
    }else if(direction == 'L'){
        deltaX = -1; deltaY = 0;
    }else if(direction == 'D'){
        deltaX = 0; deltaY = 1;
    }else if(direction == 'U'){
        deltaX = 0; deltaY = -1;
    }
    int playerX = -7;
    int playerY = -7;
    for(int y = 0;y < b.size();y++){
        for(int x = 0;x < b[y].size();x++){
            char cell = b[y][x];
            if(cell == PLAYER || cell == PLAYER_ON_GOAL){
                playerX = x;
                playerY = y;
            }
        }
    }
    // 12 disjunct cases; here we go:
    bool inBounds = 0 <= playerY+deltaY*2 && playerY+deltaY*2 < b.size()
        && 0 <= playerX+deltaX*2 && playerX+deltaX*2 < b[0].size();
    char first = b[playerY][playerX];
    char second = b[playerY+deltaY][playerX+deltaX];
    char third = inBounds ? b[playerY+2*deltaY][playerX+2*deltaX] : '!';
    if(first == PLAYER){
        if(second == FREE){
            first = FREE; second = PLAYER;
        }else if(second == GOAL){
            first = FREE; second = PLAYER_ON_GOAL;
        }else if(second == BOX){
            if(third == FREE && third != '!'){
                first = FREE; second = PLAYER; third = BOX;
            }else if(third == GOAL && third != '!'){
                first = FREE; second = PLAYER; third = BOX_ON_GOAL;
            }
        }else if(second == BOX_ON_GOAL){
            if(third == FREE && third != '!'){
                first = FREE; second = PLAYER_ON_GOAL; third = BOX;
            }else if(third == GOAL && third != '!'){
                first = FREE; second = PLAYER_ON_GOAL; third = BOX_ON_GOAL;
            }
        }
    }else if(first == PLAYER_ON_GOAL){
        if(second == FREE){
            first = GOAL; second = PLAYER;
        }else if(second == GOAL){
            first = GOAL; second = PLAYER_ON_GOAL;
        }else if(second == BOX){
            if(third == FREE && third != '!'){
                first = GOAL; second = PLAYER; third = BOX;
            }else if(third == GOAL && third != '!'){
                first = GOAL; second = PLAYER; third = BOX_ON_GOAL;
            }
        }else if(second == BOX_ON_GOAL){
            if(third == FREE && third != '!'){
                first = GOAL; second = PLAYER_ON_GOAL; third = BOX;
            }else if(third == GOAL && third != '!'){
                first = GOAL; second = PLAYER_ON_GOAL; third = BOX_ON_GOAL;
            }
        }
    }
    // TODO: If nothing changed, throw execption.
    b[playerY][playerX] = first;
    b[playerY+deltaY][playerX+deltaX] = second;
    if(third != '!'){
        b[playerY+2*deltaY][playerX+2*deltaX] = third;
    }
    return b;
}

vector<vector<vector<char> > > applyDirections(vector<vector<char> > board,string solution){
    vector<vector<vector<char> > > boards;
    boards.push_back(board);
    for(int i = 0;i < solution.size();i++){
        vector<vector<char> > b = applyDirection(boards[boards.size()-1],solution[i]);
        boards.push_back(b);
    }
    return boards;
}

bool solvedBoard(vector<vector<char> > b){
    for(int y = 0;y < b.size();y++){
        for(int x = 0;x < b[y].size();x++){
            char cell = b[y][x];
            if(cell == '$'){
                return false;
            }
        }
    }
    return true;
}

void verify(vector<vector<char> > board){
    cout << "Testing solution...";
    string returned = sokoban(board);
    vector<vector<vector<char> > > output = applyDirections(board,returned);
    vector<vector<char> > finalBoard = output[output.size()-1];
    if(solvedBoard(finalBoard)){
        cout << "passed! Solution:" << endl;
        for(int y = 0;y < finalBoard.size();y++){
            for(int x = 0;x < finalBoard[y].size();x++){
                char cell = finalBoard[y][x];
                cout << cell;
            }
            cout << endl;
        }
    }else{
        cout << "failed! Incorrect solution:" << endl;
        for(int y = 0;y < finalBoard.size();y++){
            for(int x = 0;x < finalBoard[y].size();x++){
                char cell = finalBoard[y][x];
                cout << cell;
            }
            cout << endl;
        }
    }
}

