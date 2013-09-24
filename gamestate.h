#include <iostream>
#include <vector>
#include <string>

class GameState{
public:
    GameState(vector<string> stringmap,int width,int height);
    ~GameState();
    vector<vector<char> > map;
    int playerX;
    int playerY;
};