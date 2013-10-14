#ifndef _DEADLOCKDETECTION_H__
#define _DEADLOCKDETECTION_H__

#include <vector>
#include "GameState.h"

class GameState;

bool findDynamicDeadlocks(GameState * gs, pos dst);
void findStaticDeadLocks(std::vector<std::vector<char> > &map);

#endif
