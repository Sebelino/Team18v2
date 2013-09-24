#include <utility>
#include <vector>
#include <string>

#ifndef _MAP_H__
#define _MAP_H__


class Map
{
public:
	Map(std::vector<std::string> stringmap,unsigned int width, unsigned int height);
	~Map(void);

	bool isWall(const std::pair<int,int> & coords);
	bool isGoal(const std::pair<int,int> & coords);

	unsigned int getWidth();
	unsigned int getHeight();

	std::vector<std::vector<char> >* getOriginalMap();

private:
	std::vector<std::vector<char> > map;
	unsigned int width;
	unsigned int height;
};

#endif
