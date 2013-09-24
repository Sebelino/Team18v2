#include <iostream>
#include <vector>
#include <string>

class GameState{
public:
    GameState(vector<string> stringmap,int width,int height);
	GameState(vector<vector<char> > stringmap,int width,int height);
    ~GameState();

    vector<vector<char> > map;
	GameState pushBox(const struct move& m);

    int playerX;
    int playerY;
};