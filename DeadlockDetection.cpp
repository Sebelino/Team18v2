#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "DeadlockDetection.h"
#include "Constants.h"

using namespace std;

class GameState;

/**
 * Detects dynamic deadlocks on the map, caused by moving boxes. 
 * If a dynamic deadlock is found, the game is over.
 * The function returns true if a deadlock is found.
 * Only local search is necessary.
 */
bool findDynamicDeadlocks(GameState * gs, pos dst) {
	//pos dst = gs->src.end;
	char chdest = gs->board[dst.y][dst.x];
	if (chdest != BOX && chdest != BOX_ON_GOAL) {
		//This shouldn't happen
		fprintf(stderr, "Parameter error in findDynamicDeadlocks()\n");
		return true;
	}
	
	pos curDir;
	std::vector<pos> directions;
	directions.push_back(pos(1,0));
	directions.push_back(pos(1,1));
	directions.push_back(pos(0,1));
	directions.push_back(pos(-1,1));
	directions.push_back(pos(-1,0));
	directions.push_back(pos(-1,-1));
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1,-1));
	char c1, c2, c3, c4, c5, c6, c7, c8, c9;
	
	//First test. Find certain types of deadlocks.
	for (int i = 0;i<8;i+=2) {
		curDir = directions[i];
		c1 = gs->board[dst.y+curDir.y][dst.x+curDir.x];
		if (c1 == BOX || c1 == BOX_ON_GOAL || c1 == WALL) {
			//If there is a neighboring obstacle
			c1 = gs->board[dst.y+curDir.x][dst.x+curDir.y];
			c2 = gs->board[dst.y-curDir.x][dst.x-curDir.y];
			c3 = gs->board[dst.y+curDir.x+curDir.y][dst.x+curDir.y+curDir.x];
			c4 = gs->board[dst.y-curDir.x-curDir.y][dst.x-curDir.y-curDir.x];
			if ((c1 == WALL || c2 == WALL) && (c3 == WALL || c4 == WALL)) {
				//Deadlock found!
				return true;
			}
		}
	}
	
	//New attempt at 3x3 deadlock pattern detection
	curDir = dst-gs->src.start;
	c1 = gs->board[dst.y+curDir.x][dst.x+curDir.y];
	c3 = gs->board[dst.y-curDir.x][dst.x-curDir.y];

	if ((c1 == GOAL || c1 == FREE) && (c3 == GOAL || c3 == FREE)) {
		//Likely not a deadlock, although not certain (TODO)
		//return false for now
		return false;
	}
	
	c5 = gs->board[dst.y+curDir.y*2][dst.x+curDir.x*2];
	if (c5 == FREE || c5 == GOAL || c5 == DEADLOCK) {
		//Only possible deadlock is with dst as bottom middle tile
		c4 = gs->board[dst.y+curDir.x+curDir.y*2][dst.x+curDir.y+curDir.x*2];
		if (c4 == FREE || c4 == GOAL) {
			return false;
		}
		c6 = gs->board[dst.y-curDir.x+curDir.y*2][dst.x-curDir.y+curDir.x*2];
		if (c6 == FREE || c6 == GOAL) {
			return false;
		}
		try {
			c8 = gs->board[dst.y+curDir.y*3][dst.x+curDir.x*3];
		} catch (Exception e) {
			//Out of bounds
			c8 = WALL;
		}
		if (c8 == FREE || c8 == GOAL) {
			return false;
		}
		
		//All non-corner edges are obstacles.
		
		
		
		
		
	} else {
		//TODO
		//c4 = gs->board[dst.y+curDir.x+curDir.y*2][dst.x+curDir.y+curDir.x*2];
		//c6 = gs->board[dst.y-curDir.x+curDir.y*2][dst.x-curDir.y+curDir.x*2];
		
	
	}
	
	
	
	
	
	
	
	
	
	/*
	if (c1 == GOAL || c1 == FREE) {
		//c3 must be an obstacle
		c5 = gs->board[dst.y+curDir.y*2][dst.x+curDir.x*2];
		c6 = gs->board[dst.y-curDir.x+curDir.y*2][dst.x-curDir.y+curDir.x*2];
		
		if (c5 == FREE) {
			//dst cannot be corner box of a deadlock pattern
			if (c6 == FREE || c6 == GOAL) {
				//No deadlock
				return false;
			}
			c4 = gs->board[dst.y+curDir.x+curDir.y*2][dst.x+curDir.y+curDir.x*2];
			c8 = gs->board[dst.y+curDir.y*3][dst.x+curDir.x*3];
			if (c4 == FREE || c4 == GOAL || c8 == FREE || c8 == GOAL) {
				//No deadlock
				return false;
			}

			c7 = gs->board[dst.y+curDir.x+curDir.y*3][dst.x+curDir.y+curDir.x*3];
			c9 = gs->board[dst.y-curDir.x+curDir.y*3][dst.x-curDir.y+curDir.x*3];
			
			if (c7 == FREE || c7 == GOAL) {
				if (c9 == FREE || c9 == GOAL) {
					if (c4 == WALL && c8 == WALL) {
						return true;
					}
					//No deadlock
					return false;
				} else {
					if (c4 == WALL) {
						return true;
					}
					return false;
				}
				
			}
		} else { //If c5 is an obstacle
			
		}
	
	}
	*/

	/*
	//Second test. Test for 3x3 deadlock patterns
	pos nC, nE;

	string s;
	s.reserve(9);
	for (int i = 0;i<8;i++) {
		nC = dst+directions[i];
		for (int j = 0;j<8;j++) {
			nE = dst+curDir+directions[j];
			if (nE.y >= gs->board.size() || nE.y < 0) {
				s.push_back(WALL);
			} else if (nE.x >= gs->board[nE.y].size() || nE.x < 0) {
				s.push_back(WALL);
			} else {
				s.push_back(gs->board[nE.y][nE.x]);
			}
		}
		s.push_back(gs->board[nC.y][nC.x]);
		
		//DETECT DEADLOCKS IN THIS AREA
		
		if (s[8] != GOAL) {
			bool cornersQualify = false;
			bool edgesQualify = false;
			
			if (s[1] == WALL || s[1] == BOX || s[1] == BOX_ON_GOAL) {
				if (s[3] == WALL || s[3] == BOX || s[3] == BOX_ON_GOAL) {
					if (s[5] == WALL || s[5] == BOX || s[5] == BOX_ON_GOAL) {
						if (s[7] == WALL || s[7] == BOX || s[7] == BOX_ON_GOAL) {
							edgesQualify = true;
						}
					}
				}
			}
			
			if ((s[0] == WALL || s[0] == BOX || s[0] == BOX_ON_GOAL) && (s[4] == WALL || s[4] == BOX || s[4] == BOX_ON_GOAL)) {
				cornersQualify = true;
			}
			if ((s[2] == WALL || s[2] == BOX || s[2] == BOX_ON_GOAL) && (s[6] == WALL || s[6] == BOX || s[6] == BOX_ON_GOAL)) {
				cornersQualify = true;
			}
			
			
			if ((s[0] == WALL || s[0] == BOX || s[0] == BOX_ON_GOAL) && (s[2] == WALL || s[2] == BOX || s[2] == BOX_ON_GOAL)) {
				if (s[5] == WALL) {
					cornersQualify = true;
				}
			}
			if ((s[2] == WALL || s[2] == BOX || s[2] == BOX_ON_GOAL) && (s[4] == WALL || s[4] == BOX || s[4] == BOX_ON_GOAL)) {
				if (s[7] == WALL) {
					cornersQualify = true;
				}
			}
			if ((s[4] == WALL || s[4] == BOX || s[4] == BOX_ON_GOAL) && (s[6] == WALL || s[6] == BOX || s[6] == BOX_ON_GOAL)) {
				if (s[1] == WALL) {
					cornersQualify = true;
				}
			}
			if ((s[6] == WALL || s[6] == BOX || s[6] == BOX_ON_GOAL) && (s[0] == WALL || s[0] == BOX || s[0] == BOX_ON_GOAL)) {
				if (s[3] == WALL) {
					cornersQualify = true;
				}
			}
			
			if (cornersQualify && edgesQualify) {
				//Found deadlock
				return true;
			}
		}
		s.clear();
	}
	*/
	
	//If no deadlocks were found: return false.
	return false;
}

/**
 * Detects static deadlocks on the map. 
 * These deadlocks won't change throughout the game.
 */
void findStaticDeadLocks(vector<vector<char> > &map) {
	//TODO

	//Debug print with deadlocks
   /* fprintf(stdout, "Before detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stdout, "%c", map[i][j]);
        }
        fprintf(stdout, "\n");
    }*/
	
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
    /*fprintf(stderr, "After detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stdout, "%c", map[i][j]);
        }
        fprintf(stdout, "\n");
    }*/
}

