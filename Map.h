#ifndef _MAP_H__
#define _MAP_H__

#include <utility>
#include <vector>
#include <string>
#include "Structs.h"
#include "PathFinding.h"


class Map {

public:
	Map(std::vector<std::string> stringmap, int width, int height);
	~Map(void);

	bool isWall(pos coords);
	bool isGoal(pos coords);

	int getWidth();
	int getHeight();
	void findStaticDeadLocks();
    bool isDeadlock(pos p);

	std::vector<std::vector<char> >* getOriginalMap();

private:
	std::vector<std::vector<char> > map;
	int width;
	int height;
};

#endif
