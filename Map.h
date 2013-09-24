#include <utility>
#include <vector>

#ifndef _MAP_H__
#define _MAP_H__


class Map
{
public:
	Map(std::vector<std::string> stringmap,int width,int height);
	~Map(void);

	bool isWall(const std::pair<int,int> & coords);
	bool isGoal(const std::pair<int,int> & coords);

	int getWidth();
	int getHeight();

	std::vector<std::vector<char> >* getOriginalMap();

private:
	std::vector<std::vector<char> > map;
	int width;
	int height;
};

#endif