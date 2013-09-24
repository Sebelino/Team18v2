#include "Map.h"
#include "Constants.cpp"

using namespace std;
/*
A Map represents the static parts of the map.
*/



/**
 * Creates a Map from a vector of strings. Width and height are set but has 
 * nothing to do with map initialization.
 */
Map::Map(vector<string> stringmap,unsigned int width, unsigned int height) {
    for(unsigned int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(unsigned int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[j][i]); //not sure about order of i and j - but want first coordinate to be column.
        }
        map.push_back(line);
    }

	this->width = width;
	this->height = height;
}

Map::~Map(void) {
}

/**
 * Takes a position on the board as input, and answers true if it is a wall,
 * as defined by constans.cpp, false otherwise.
 */
bool Map::isWall(const pair<int,int>& coords ) {
	return (map[coords.first][coords.second] == WALL);
}

/**
 * Takes a position on the board as input, and answers true if it is a goal,
 * as defined by constans.cpp, false otherwise.
 */
bool Map::isGoal(const pair<int,int>& coords ) {
	return (map[coords.first][coords.second] == GOAL || map[coords.first][coords.second] == BOX_ON_GOAL);
}

/* Getters for width and height */
unsigned int Map::getWidth() { return width; }
unsigned int Map::getHeight() { return height; }

/* Get a pointer to the starting map/board */
vector<vector<char> >* Map::getOriginalMap() {
	return &map;
}
