#include "Map.h"
#include "Constants.h"

using namespace std;
/*
A Map represents the static parts of the map.
*/



/**
 * Creates a Map from a vector of strings. Width and height are set but has 
 * nothing to do with map initialization.
 */
Map::Map(vector<string> stringmap, int width, int height) {
    for(int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(int j = 0;j < stringmap[i].size();j++){
            line.push_back(stringmap[i][j]); //not sure about order of i and j - but want first coordinate to be column.
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
bool Map::isWall(pos coords) {
	return (map[coords.x][coords.y] == WALL);
}

/**
 * Takes a position on the board as input, and answers true if it is a goal,
 * as defined by constans.cpp, false otherwise.
 */
bool Map::isGoal(pos coords ) {
	return (map[coords.x][coords.y] == GOAL || map[coords.x][coords.y] == BOX_ON_GOAL);
}

/* Getters for width and height */
int Map::getWidth() { return width; }
int Map::getHeight() { return height; }

/* Get a pointer to the starting map/board */
vector<vector<char> >* Map::getOriginalMap() {
	return &map;
}

/**
 * Searches the map for places that once a box is put there, the game is unsolvable.
 */
void findStaticDeadLocks() {
	//TODO
	
	for(int i = 1;i < map.size()-1;i++){
        for(int j = 1;j < map[i].size()-1;j++){
            //Do something about map[i][j];
            
            if (map[i][j] == DEADLOCK || map[i][j] == WALL) {
            	//Already detected as deadlock
            	continue;
            }
            
            
            
        }
    }
	
}






