#include <iostream>
#include <vector>
#include <string>
#include <set>

#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

struct move{
    int startX;
    int startY;
    int endX;
    int endY;
};

class GameState{
public:
    GameState(std::vector<std::string> stringmap,int width,int height);
	GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

    std::vector<std::vector<char> > map;
	GameState pushBox(const struct move& m);
	bool isValid(const struct move& m);
    std::set<GameState> findNextMoves();

    int playerX;
    int playerY;
};

#endif

