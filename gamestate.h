#include <iostream>
#include <vector>
#include <string>

using namespace std; // Bör inte finnas i en headerfil. Flytta till gamestate.cpp?

class GameState{
public:
    GameState(vector<string> stringmap,int width,int height);
	GameState(vector<vector<char> > stringmap,int width,int height);
    ~GameState();

    vector<vector<char> > map;
	GameState pushBox(const struct move& m);
	bool isValid(const struct move& m);

    int playerX;
    int playerY;
};
