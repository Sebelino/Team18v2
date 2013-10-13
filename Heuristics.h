#ifndef _HEURISTICS_H__
#define _HEURISTICS_H__

#include "GameState.h"
#include "Structs.h"


void heuristicSmarter(GameState& g);
int checkGoalClass(GameState& g, int i, int j);
bool isBoxWall(GameState& g, int i, int j);
int heuristicDistance(const pos& p1,const pos& p2);



#endif