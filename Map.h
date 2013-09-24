#include <utility>
#include <vector>
#include <string>

#ifndef _MAP_H__
#define _MAP_H__


struct pos {
    int x;
    int y;
    pos operator-(pos other) const {
    	return {x-other.x, y-other.y};
    }
    pos operator+(pos other) const {
    	return {x+other.x, y+other.y};
    }
    pos operator-() const {
    	return {-x, -y};
    }
    pos operator*(int factor) const {
    	return {x*factor, y*factor};
    }
    bool operator<(pos other) const {
    	if (x == other.x) {
    		return y-other.y;
        else
	    	return x-other.x;
    }
};

class Map
{
public:
	Map(std::vector<std::string> stringmap, int width, int height);
	~Map(void);

	bool isWall(pos coords);
	bool isGoal(pos coords);

	int getWidth();
	int getHeight();

	std::vector<std::vector<char> >* getOriginalMap();

private:
	std::vector<std::vector<char> > map;
	int width;
	int height;
};

#endif
