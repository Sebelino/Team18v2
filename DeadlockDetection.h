#ifndef _DEADLOCKDETECTION_H__
#define _DEADLOCKDETECTION_H__

#include <vector>

class GameState;

void findStaticDeadLocks(std::vector<std::vector<char> > &map);
bool findDynamicDeadlocks(GameState * gs, pos dst)

#endif
