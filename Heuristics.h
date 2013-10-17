#ifndef _HEURISTICS_H__
#define _HEURISTICS_H__

#include "GameState.h"
#include "Structs.h"


/*
* The DRA (diminishing returns array) is a way of making it more worth to actually push boxes into places,
* rather than pushing them one at a time to each respective goal.
*/

//#ifndef DIM_RETURN_DISTANCE_A
//#define DIM_RETURN_DISTANCE_A
#define DIM_RETURN_DISTANCE 6

static int _DRA[DIM_RETURN_DISTANCE] = {0,5,9,12,14,15};
//static int _DRA[DIM_RETURN_DISTANCE] = {0,9,17,24,30,35,39,42,44,45}; //diminishing returns array, size 10
//#endif

int heuristicEvenBetter(GameState& g);
int aStarDistance(GameState& g);
void heuristicSmarter(GameState& g);
int checkGoalClass(GameState& g, int i, int j);
bool isBoxWall(GameState& g, int i, int j);
int heuristicDistance(const pos& p1,const pos& p2);



#endif