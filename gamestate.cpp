#include <iostream>
#include <vector>
#include <string>

using namespace std;

class GameState{
public:
    GameState(vector<string> stringmap,int width,int height);
    ~GameState();
    vector<vector<char> > map;
    int playerX;
    int playerY;
};
GameState::GameState(vector<string> stringmap,int width,int height){
    for(int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[i][j]);
        }
        map.push_back(line);
    }
}
GameState::~GameState(){}
ostream& operator<<(ostream &strm, const GameState &state) {
    std::ostream& stream = strm;
    for(int i = 0;i < state.map.size();i++){
        for(int j = 0;j < state.map[i].size();j++){
            stream << state.map[i][j];
        }
        stream << endl;
    }
    return stream;
}

int main(){
    vector<string> lines;
    lines.push_back("hej");
    lines.push_back("halloj");
    lines.push_back("yo");
    GameState gs (lines,5,lines.size());
    cout << gs << endl;
    return 0;
}
