#ifndef _DEADLOCKDETECTION_H__
#define _DEADLOCKDETECTION_H__

#include <vector>
#include "GameState.h"

class GameState;

bool findDynamicDeadlocks(GameState * gs, pos dst);
void findStaticDeadLocks(char** map, int height, int width);
bool isObstacle(char c);
bool isOpen(char c);

#endif
