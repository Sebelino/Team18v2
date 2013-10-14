#ifndef _DEADLOCKDETECTION_H__
#define _DEADLOCKDETECTION_H__

#include <vector>
#include "GameState.h"

class GameState;

bool findDynamicDeadlocks(GameState * gs);
void findStaticDeadLocks(std::vector<std::vector<char> > &map);

#endif
