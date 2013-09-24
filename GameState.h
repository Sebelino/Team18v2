#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Map.h"

#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

struct boxMove{
    std::pair<int,int> start;
    std::pair<int,int> end;
};

class GameState{
public:
	GameState(Map * canvas);
	GameState(GameState * prev, struct boxMove * box_move);
    //GameState(std::vector<std::string> stringmap,int width,int height);
	//GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

	GameState pushBox(const struct boxMove& m);
	bool isValid(const struct boxMove& m);
    std::set<GameState> findNextMoves();

    std::pair<int,int> player;
	std::set<std::pair<int,int> > boxes;
	Map * map;

private:
	void setBoxes(std::vector<std::vector<char> >* stringmap);
	
};

#endif

