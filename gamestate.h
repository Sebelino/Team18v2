#include <iostream>
#include <vector>
#include <string>

#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

class GameState{
public:
    GameState(std::vector<std::string> stringmap,int width,int height);
	GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

    std::vector<std::vector<char> > map;
	GameState pushBox(const struct move& m);

    int playerX;
    int playerY;
};

#endif
