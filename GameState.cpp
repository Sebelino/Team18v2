#include <iostream>
#include <vector>
#include <string>
#include "gamestate.h"

using namespace std;

struct move{
    int startX;
    int startY;
    int endX;
    int endY;
};

GameState::GameState(vector<string> stringmap,int width,int height){
    for(unsigned int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(unsigned int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[i][j]);
        }
        map.push_back(line);
    }
}
GameState::GameState(vector<vector<char> > stringmap,int width,int height){
    for(unsigned int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(unsigned int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[i][j]);
        }
        map.push_back(line);
    }
}
GameState GameState::pushBox(const struct move & m){
    vector<vector<char> > stringmap = map;
    stringmap[m.startY][m.startX] = ' ';
    stringmap[m.endY][m.endX] = '$';
    return GameState(stringmap,0,0);
}
GameState::~GameState(){}
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(unsigned int i = 0;i < state.map.size();i++){
        for(unsigned int j = 0;j < state.map[i].size();j++){
            stream << state.map[i][j];
        }
        stream << endl;
    }
    return stream;
}

