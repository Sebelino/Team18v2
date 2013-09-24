#include "Map.h"
#include "constants.cpp"

using namespace std;

/**
 * Creates a Map from a vector of strings. Width and height are set but has 
 * nothing to do with map initialization.
 */
Map::Map(vector<string> stringmap,int width,int height) {
    for(int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[i][j]);
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
	return (map[coords.first][coords.second] == GOAL);
}

/* Getters for width and height */
int Map::getWidth() { return width; }
int Map::getHeight() { return height; }

/* Get a pointer to the starting map/board */
vector<vector<char> >* Map::getOriginalMap() {
	return &map;
}
