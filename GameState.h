#ifndef _GAMESTATE_H__
#define _GAMESTATE_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "Structs.h"
#include "PathFinding.h"


class GameState{
public:
	GameState(std::vector<std::vector<char> > b);
	GameState(GameState * prev, struct boxMove * box_move);
    //GameState(std::vector<std::string> stringmap,int width,int height);
	//GameState(std::vector<std::vector<char> > stringmap,int width,int height);
    ~GameState();

	GameState pushBox(const struct boxMove& m);
	bool isValid(const struct boxMove& m);
    std::set<boxMove> moves(pos boxPos);
    std::vector<GameState*> findNextMoves(); //TODO
    bool operator<(GameState other) const;
    std::string hash() const;
	bool isSolution();
    int heuristic() const;
    bool makeMove(pos dir);

	std::vector<std::vector<char> > board;
    pos player;
	double score; //TODO
	GameState * parent;
	boxMove src;
	
	int depth;

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

