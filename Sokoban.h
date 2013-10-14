#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "GameState.h"
#include "Constants.h"

#ifndef _SOKOBAN_H__
#define _SOKOBAN_H__
std::vector<GameState*> solve(GameState * gs);
std::string sokoban(std::vector<std::vector<char> > board);
std::vector<GameState*> solution(std::vector<std::vector<char> > board);
#endif

