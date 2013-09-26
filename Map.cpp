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
    for(unsigned int i = 0;i < stringmap.size();i++){
        vector<char> line;
        for(unsigned int j = 0;j < stringmap[i].size();j++){
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
void Map::findStaticDeadLocks() {
	//TODO
	
	std::vector<pos> directions;
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1, 0));
	directions.push_back(pos(0, 1));
	directions.push_back(pos(-1,0));
	pos curP;


	//Debug print without deadlocks
	for(int i = 1;i < map.size()-1;i++){
        for(int j = 1;j < map[i].size()-1;j++){
        	fprintf(stderr, "%d", map[i][j]);
        }
        fprintf(stderr, "\n");
    }
	
	bool byWall;
	
	for(int i = 1;i < map.size()-1;i++){
        for(int j = 1;j < map[i].size()-1;j++){
            //Do something about map[i][j];
            if (map[i][j] == FREE) {
            	//Only free spaces can be deadlocks
            	byWall = false;
            	for (int k = 0;k<4;k++) {
            		curP = directions[k];
            		
            		if (map[i+curP.x][j+curP.y] == WALL) {
            			//It's close to a wall, might be deadlock
            			byWall = true;
            			
            			
            			
            		}	
            	}	
            }
        }
    }
    
    
    //Debug print with deadlocks
	for(int i = 1;i < map.size()-1;i++){
        for(int j = 1;j < map[i].size()-1;j++){
        	fprintf(stderr, "%d", map[i][j]);
        }
        fprintf(stderr, "\n");
    }
}


/* Return true if the position renders the game session unsolvable. */
bool Map::isDeadlock(pos p){
    if(map[p.y][p.x] == 'X'){
        return true;
    }
    return false;
}

