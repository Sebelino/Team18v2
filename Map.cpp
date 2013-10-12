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
Map::Map(vector<vector<char> > stringmap, int width, int height) {
	map = stringmap;
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
	return (map[coords.y][coords.x] == WALL);
}

/**
 * Takes a position on the board as input, and answers true if it is a goal,
 * as defined by constans.cpp, false otherwise.
 */
bool Map::isGoal(pos coords ) {
	return (map[coords.y][coords.x] == GOAL || map[coords.y][coords.x] == BOX_ON_GOAL || map[coords.y][coords.x] == PLAYER_ON_GOAL);
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

	/*
	//Debug print without deadlocks
	fprintf(stderr, "Before detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stderr, "%c", map[i][j]);
        }
        fprintf(stderr, "\n");
    }
    */
	
	/*
	bool goals = false;
	for(int i = 1;i < map.size()-1;i++) {
		if (map[i][1] == GOAL) {
			goals = true;
		}
    }    
	if (!goals) {
		for(int i = 1;i < map.size()-1;i++) {
			if (map[i][1] == FREE)
				map[i][1] = DEADLOCK;
		}
	}
	
	goals = false;
	for(int i = 1;i < map.size()-1;i++) {
		if (map[i][map[i].size()-2] == GOAL) {
			goals = true;
		}
    }    
	if (!goals) {
		for(int i = 1;i < map.size()-1;i++) {
			if (map[i][map[i].size()-2] == FREE)
				map[i][map[i].size()-2] = DEADLOCK;
		}
	}
	
	
	
	goals = false;
	for(int i = 1;i < map[1].size()-1;i++) {
		if (map[1][i] == GOAL) {
			goals = true;
		}
    }    
	if (!goals) {
		for(int i = 1;i < map[1].size()-1;i++) {
			if (map[1][i] == FREE)
				map[1][i] = DEADLOCK;
		}
	}
	
	goals = false;
	for(int i = 1;i < map[map.size()-2].size()-1;i++) {
		if (map[map.size()-2][i] == GOAL) {
			goals = true;
		}
    }    
	if (!goals) {
		for(int i = 1;i < map[map.size()-2].size()-1;i++) {
			if (map[map.size()-2][i] == FREE)
				map[map.size()-2][i] = DEADLOCK;
		}
	}*/
	
	//Debug print with deadlocks
    fprintf(stdout, "Before detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stdout, "%c", map[i][j]);
        }
        fprintf(stdout, "\n");
    }
	
	static const char EXAMINED = 'e';
	int i, j, k;
	//Aah, it's tricky to get it right
	for (i = 1;i < map.size()-1;i++) {
        for (j = 1;j < map[i].size()-1;j++) {
        	//fprintf(stderr, "pos: %d, %d\n", j, i);
			if (map[i][j] == FREE) {	//Only free spaces can be deadlocks
				
				//Horizontal search
				if (map[i-1][j] == WALL || map[i+1][j] == WALL) {
					//It's close to a wall, might be deadlock
					
					int dle1 = 0, dle2 = 0;
					//Walk until no wall on the side, or until you run into a wall
					for (k = 0; (map[i-1][j+k] == WALL || map[i+1][j+k] == WALL) && (map[i][j+k] != GOAL && map[i][j+k] != BOX_ON_GOAL) ;k++) {
						if (map[i][j+k] == WALL) {	//Ran into wall, deadlock possible.
							dle1 = k;
							if (map[i][j+k-1] == EXAMINED) {
								map[i][j+k-1] = FREE;
							}
							goto wallRight;
						} else {
							if (map[i][j+k] != BOX) {
								map[i][j+k] = EXAMINED;
							}
						}
					}
wallRight:			
					if (dle1) {
						for (k = 0; (map[i-1][j+k] == WALL || map[i+1][j+k] == WALL) && (map[i][j+k] != GOAL && map[i][j+k] != GOAL);k--) {
							if (map[i][j+k] == WALL) {	//Ran into wall, deadlock certain.
								dle2 = k;
								if (map[i][j+k+1] == EXAMINED) {
									map[i][j+k+1] = FREE;
								}
								goto wallLeft;
							} else {
								if (map[i][j+k] != BOX) {
									map[i][j+k] = EXAMINED;
								}
							}
						}
					}
wallLeft:			
					//fprintf(stderr, "HS: dle1 and dle2 have values: %d and %d\n", dle1, dle2);
					if (dle1 && dle2) {
						//Mark deadlock zone
						for (k = dle2+1; k < dle1 ;k++) {
							map[i][j+k] = DEADLOCK;
						}
					}
				}
				
				//Vertical search
		        if (map[i][j-1] == WALL || map[i][j+1] == WALL) {
					//It's close to a wall, might be deadlock
					int dle1 = 0, dle2 = 0;
					//Walk until no wall on the side, or until you run into a wall
					for (k = 0; (map[i+k][j-1] == WALL || map[i+k][j+1] == WALL) && (map[i+k][j] != GOAL && map[i+k][j] != BOX_ON_GOAL) ;k++) {
						if (map[i+k][j] == WALL) {	//Ran into wall, deadlock possible.
							dle1 = k;
							if (map[i+k-1][j] == EXAMINED) {
								map[i+k-1][j] = FREE;
							}
							goto wallDown;
						} else {
							if (map[i+k][j] != BOX) {
								map[i+k][j] = EXAMINED;
							}
						}
					}
wallDown:			
					if (dle1) {
						for (k = 0; (map[i+k][j-1] == WALL || map[i+k][j+1] == WALL) && (map[i+k][j] != GOAL && map[i+k][j] != BOX_ON_GOAL) ;k--) {
							if (map[i+k][j] == WALL) {	//Ran into wall, deadlock certain.
								dle2 = k;
								if (map[i+k+1][j] == EXAMINED) {
									map[i+k+1][j] = FREE;
								}
								goto wallUp;
							} else {
								if (map[i+k][j] != BOX) {
									map[i+k][j] = EXAMINED;
								}
							}
						}
					}
wallUp:				
					//fprintf(stderr, "VS: dle1 and dle2 have values: %d and %d\n", dle1, dle2);
					if (dle1 && dle2) {
						//Mark deadlock zone
						for (k = dle2+1; k < dle1 ;k++) {
							map[i+k][j] = DEADLOCK;
						}
					}
				}
    		}
    		
    		if (map[i][j-1] == WALL || map[i][j+1] == WALL) {
    			if (map[i-1][j] == WALL || map[i+1][j] == WALL) {
    				if (map[i][j] != GOAL && map[i][j] != BOX && map[i][j] != BOX_ON_GOAL && map[i][j] != WALL) {
    					map[i][j] = DEADLOCK;
    				}
    			}
    		}
    		
    		/*
			fprintf(stderr, "After partly detecting deadlocks:\n");
			for(int l = 0;l < map.size();l++){
				for(int m = 0;m < map[i].size();m++){
					fprintf(stderr, "%c", map[l][m]);
				}
				fprintf(stderr, "\n");
			}
			*/
    		
        }
    }
    
    for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	if (map[i][j] == EXAMINED) {
        		map[i][j] = FREE;
        	}
        }
    }
	
    
    //Debug print with deadlocks
    fprintf(stdout, "After detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stdout, "%c", map[i][j]);
        }
        fprintf(stdout, "\n");
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

