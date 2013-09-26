#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Map.h"
#include "Structs.h"


class GameState{
public:
	GameState(Map * canvas);
	GameState(GameState * prev, struct boxMove * box_move);
    //GameState(std::vector<std::string> stringmap,int width,int height);
	//GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

	GameState pushBox(const struct boxMove& m);
	bool isValid(const struct boxMove& m);
    std::set<boxMove> moves(pos boxPos);
    std::set<GameState> findNextMoves();
    bool operator<(GameState other) const;

    pos player;
	std::set<pos> boxes;
	Map * map;

private:
	void setBoxes(std::vector<std::vector<char> >* stringmap);
	
};

#endif

