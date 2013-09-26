#ifndef _MAP_H__
#define _MAP_H__

#include <utility>
#include <vector>
#include <string>

struct pos {
    int x;
    int y;
    pos operator-(pos other) const {
		pos p;
		p.x = x-other.x;
		p.y = y-other.y;
		return p;
    }
    pos operator+(pos other) const {
    	pos p;
		p.x = x+other.x;
		p.y = y+other.y;
		return p;
    }
    pos operator-() const {
		pos p;
		p.x = -x;
		p.y = -y;
		return p;
    }
    pos operator*(int factor) const {
		pos p;
		p.x = x*factor;
		p.y = y*factor;
		return p;
    }
    bool operator<(pos other) const {
    	if (x == other.x)
    		return y-other.y;
        else
	    	return x-other.x;
    }
};

class Map {

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
