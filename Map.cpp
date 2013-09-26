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
	return (map[coords.y][coords.x] == GOAL || map[coords.y][coords.x] == BOX_ON_GOAL);
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
	
	
	bool goals = false;
	//TODO
	for(int i = 1;i < map.size()-1;i++) {
		if (map[i][1] == GOAL) {
			goals = true;
		}
    }    
	if (goals) {
		for(int i = 1;i < map.size()-1;i++) {
			map[i][1] = DEADLOCK;
		}
	}
	
	for(int i = 1;i < map.size()-1;i++) {
		if (map[i][1] == GOAL) {
			goals = true;
		}
    }    
	if (goals) {
		for(int i = 1;i < map.size()-1;i++) {
			map[i][1] = DEADLOCK;
		}
	}
	
	
	
	/*	//Aah, it's tricky to get it right
	for(int i = 1;i < map.size()-1;i++){
        for(int j = 1;j < map[i].size()-1;j++){
            //Do something about map[i][j];
            if (map[i][j] == FREE) {
            	//Only free spaces can be deadlocks
            	

        		if (map[i-1][j] == WALL) {
        			//It's close to a wall, might be deadlock
        			for (int k = 1; map[i-1][j+k] == WALL ;k++) {
        				if (map[i][j+k] == WALL) {
        					
        					for (int l = 1; map[i-1][j-l] == WALL ;l++) {
        						if (map[i][j-l] == WALL) {
        							//Found deadlock
        							for (int m = 0;m<l+k;m++) {
        								map[i][j-l+m] = 'X';
        							}
        							
        						}
        					}
        					
        					
        					
        					
        				}
        			}
        		}	


            }
        }
    }*/
    
    
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

/* Return true if the position renders the game session unsolvable. */
set<pos> Map::getGoals(){
    set<pos> goals;
    for(unsigned int i = 0;i < map.size();i++){
        for(unsigned int j = 0;j < map[i].size();j++){
            pos p (j,i);
            if(isGoal(p)){
                goals.insert(p);
            }
        }
    }
    return goals;
}

