#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "Map.h"
#include "Structs.h"
#include "PathFinding.h"

class Map;

class GameState{
public:
	GameState(Map * canvas);
	GameState(GameState * prev, struct boxMove * box_move);
    //GameState(std::vector<std::string> stringmap,int width,int height);
	//GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

	GameState pushBox(const struct boxMove& m);
	bool isValid(const struct boxMove& m);
    std::vector<boxMove> moves(pos boxPos);
    std::vector<GameState*> findNextMoves(); //TODO
    bool operator<(GameState other) const;
    long long unsigned int hash() const;
	bool isSolution();
    int heuristic() const;
    bool isBox(pos p) const;
    int boxCount() const;

    pos player;
    std::vector<std::vector<char> > boxes;
	Map * map;
	int score; //TODO
	GameState * parent;
	boxMove src;

private:
	void setBoxes(std::vector<std::vector<char> >* stringmap);
};
/*
class Hasher {
public:
	Hasher(){};
	bool operator() (GameState gs1, GameState gs2);
}*/


std::ostream& operator<<(std::ostream &strm, const GameState &state);

#endif

