#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "DeadlockDetection.h"
#include "Constants.h"

using namespace std;

class GameState;

bool isObstacle(char c) {
	switch (c) {
		case FREE:
		case DEADLOCK:
		case GOAL:
			return false;
		case BOX:
		case BOX_ON_GOAL:
		case WALL:
			return true;
	}
}

bool isOpen(char c) {
	switch (c) {
		case GOAL:
		case FREE:
		case DEADLOCK:
			return true;
		case BOX:
		case BOX_ON_GOAL:
		case WALL:
			return false;
	}
}

/**
 * Detects dynamic deadlocks on the map, caused by moving boxes. 
 * If a dynamic deadlock is found, the game is over.
 * The function returns true if a deadlock is found.
 * Only local search is necessary.
 */
bool findDynamicDeadlocks(GameState * gs, pos dst) {
	//pos dst = gs->src.end;
	//cerr << *gs;
	//fprintf(stderr, "dst is %d, %d\n", dst.x, dst.y);
	
	/*
	char chdest = gs->board[dst.y][dst.x];
	if (chdest != BOX && chdest != BOX_ON_GOAL) {
		//This shouldn't happen
		fprintf(stderr, "Parameter error in findDynamicDeadlocks()\n");
		return true;
	}

	if (chdest == BOX_ON_GOAL) {
		//Cannot handle these cases yet, return false for now (TODO)
		return false;
	}*/
	
	pos curDir = dst-gs->src.start;
	
	/*
	std::vector<pos> directions;
	directions.push_back(pos(1,0));
	directions.push_back(pos(1,1));
	directions.push_back(pos(0,1));
	directions.push_back(pos(-1,1));
	directions.push_back(pos(-1,0));
	directions.push_back(pos(-1,-1));
	directions.push_back(pos(0,-1));
	directions.push_back(pos(1,-1));
	*/
	
	pos refPos;
	
	int a, b;
	char c[4][5];
	
	/*
	for (int i = -2;i<=1;i++) {
		//fprintf(stderr,"i is: %d\n", i);
		for (int j = -2;j<=2;j++) {
			//fprintf(stderr,"j is: %i\n", j);
			a = dst.y-i*curDir.y-j*curDir.x;
			b = dst.x-i*curDir.x-j*curDir.y;
			if (a < 0 || b < 0 || a >= gs->board.size() || b >= gs->board[0].size()) {
				c[i][j] = WALL;
			} else {
				c[i][j] = gs->board[a][b];
			}
		}
	}*/
	
	bitString localBoxes(20);
	bitString localWalls(20);
	bitString localGoals(20);
	bitString localDeadlocks(20);
	localBoxes.insert32(0);
	localWalls.insert32(0);
	localGoals.insert32(0);
	localDeadlocks.insert32(0);
	
	if (curDir == pos(0,1)) {	//If last move was downwards
		//fprintf(stderr,"Downwards move\n");
		refPos = pos(dst.x+2 , dst.y+2);
		for (int i = 0;i<4;i++) {
			for (int j = 0;j<5;j++) {
				a = refPos.y-i;
				b = refPos.x-j;
				
				localBoxes.setTo(i,j,gs->boxes.getSafe(a,b,0));
				localWalls.setTo(i,j,WALLS.getSafe(a,b,1));
				localGoals.setTo(i,j,GOALS.getSafe(a,b,0));
				localDeadlocks.setTo(i,j,DEADLOCKS.getSafe(a,b,0));
				
				/*
				if (a < 0 || b < 0 || a >= gs->board.size() || b >= gs->board[0].size()) {
					c[i][j] = WALL;
				} else {
					c[i][j] = gs->board[a][b];
				}
				*/
			}
		}
	} else if (curDir == pos(0,-1)) {	//If last move was upwards
		//fprintf(stderr,"Upwards move\n");
		refPos = pos(dst.x-2, dst.y-2);
		for (int i = 0;i<4;i++) {
			for (int j = 0;j<5;j++) {
				a = refPos.y+i;
				b = refPos.x+j;
				
				localBoxes.setTo(i,j,gs->boxes.getSafe(a,b,0));
				localWalls.setTo(i,j,WALLS.getSafe(a,b,1));
				localGoals.setTo(i,j,GOALS.getSafe(a,b,0));
				localDeadlocks.setTo(i,j,DEADLOCKS.getSafe(a,b,0));
				
				/*
				if (a < 0 || b < 0 || a >= gs->board.size() || b >= gs->board[0].size()) {
					c[i][j] = WALL;
				} else {
					c[i][j] = gs->board[a][b];
				}
				*/
			}
		}		
	} else if (curDir == pos(1,0)) {	//If last move was to the right
		//fprintf(stderr,"Rightwards move\n");
		refPos = pos(dst.x+2 , dst.y-2);
		for (int i = 0;i<4;i++) {
			for (int j = 0;j<5;j++) {
				a = refPos.y+j;
				b = refPos.x-i;
				
				localBoxes.setTo(i,j,gs->boxes.getSafe(a,b,0));
				localWalls.setTo(i,j,WALLS.getSafe(a,b,1));
				localGoals.setTo(i,j,GOALS.getSafe(a,b,0));
				localDeadlocks.setTo(i,j,DEADLOCKS.getSafe(a,b,0));
				
				/*
				if (a < 0 || b < 0 || a >= gs->board.size() || b >= gs->board[0].size()) {
					c[i][j] = WALL;
				} else {
					c[i][j] = gs->board[a][b];
				}
				*/
			}
		}			
	} else if (curDir == pos(-1,0)) {	//If last move was to the left
		//fprintf(stderr,"Leftwards move\n");
		refPos = pos(dst.x-2 , dst.y+2);
		for (int i = 0;i<4;i++) {
			for (int j = 0;j<5;j++) {
				a = refPos.y-j;
				b = refPos.x+i;
				
				localBoxes.setTo(i,j,gs->boxes.getSafe(a,b,0));
				localWalls.setTo(i,j,WALLS.getSafe(a,b,1));
				localGoals.setTo(i,j,GOALS.getSafe(a,b,0));
				localDeadlocks.setTo(i,j,DEADLOCKS.getSafe(a,b,0));
				/*
				if (a < 0 || b < 0 || a >= gs->board.size() || b >= gs->board[0].size()) {
					c[i][j] = WALL;
				} else {
					c[i][j] = gs->board[a][b];
				}
				*/
			}
		}		
	}
	
	

	
	/*
	//pos dst = gs->src.end;
	//cerr << *gs;
	//fprintf(stderr, "dst is %d, %d\n", dst.x, dst.y);
	

	//First test. Detect 2x2 deadlock patterns.
	if (isObstacle(c[1][2])) {
		if (((c[1][1] == WALL) + (c[2][1] == WALL) + (c[1][3] == WALL) + (c[2][3] == WALL)) > 2) {
			//Deadlock found
			fprintf(stderr, "Returned true from here, 1\n");
			return true;
		} else {
			if (isObstacle(c[1][1]) && isObstacle(c[2][1])) {
				//fprintf(stderr, "Returned true from here, 2\n");
				return true;
			}
			if (isObstacle(c[1][3]) && isObstacle(c[2][3])) {
				//fprintf(stderr, "Returned true from here, 3\n");
				return true;
			}
		}
	}
	
	//New attempt at 3x3 deadlock pattern detection
	if (isOpen(c[2][1]) && isOpen(c[2][3])) {
		//Likely not a deadlock, although not certain (TODO)
		//return false for now
		return false;
	}

	
	if (c[1][2] == FREE || c[1][2] == DEADLOCK) {
		//Only possible deadlock is with dst as bottom middle tile
		if (isOpen(c[1][1])) {
			return false;
		}
		if (isOpen(c[1][3])) {
			return false;
		}
		if (isOpen(c[0][2])) {
			return false;
		}
		
		//All non-corner edges are obstacles.
		if (isOpen(c[2][1])) {
			//c3 is an obstacle
			if (isOpen(c[0][1])) {
				if (c[1][1] != WALL)	 {
					//Box in c[1][1] can move vertically
					return false;
				} else {
					if (isOpen(c[0][3])) {
						if (c[0][2] == WALL) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				//Deadlock
				return true;
			}
		} else {
			//c1 is an obstacle
			if (isOpen(c[0][3])) {
				if (c[1][3] != WALL)	 {
					//Box in c4 can move vertically
					return false;
				} else {
					if (isOpen(c[0][1])) {
						if (c[0][2] == WALL) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				//Deadlock
				return true;
			}
		}
	} else {
		//TODO
		//dst can be bottom corner box or side middle
		if (isOpen(c[2][1])) {
			//c[2][3] must be obstacle. dst can be bottom left
			
			if (isOpen(c[0][3])) {
				//Cannot be deadlock
				return false;
			}
			if (isOpen(c[1][4])) {
				//Cannot be deadlock
				return false;
			}
			if (isOpen(c[0][4])) {
				if (c[1][4] == WALL || isObstacle(c[2][4])) {
					if ((c[0][3] == WALL) || isObstacle(c[0][2])) {
						return true;
					}
				}
			} else {
				return true;
			}
		} else {
			//c[2][1] must be obstacle. dst can be bottom right
			
			if (isOpen(c[0][1])) {
				//Cannot be deadlock
				return false;
			}
			if (isOpen(c[1][0])) {
				//Cannot be deadlock
				return false;
			}
			if (isOpen(c[0][0])) {
				if (c[1][0] == WALL || isObstacle(c[2][0])) {
					if ((c[0][1] == WALL) || isObstacle(c[0][2])) {
						return true;
					}
				}
			} else {
				return true;
			}
		}
	}
	*/
	



	if (!gs->boxes.get(dst.y, dst.x)) {
		//This shouldn't happen
		fprintf(stderr, "Parameter error in findDynamicDeadlocks()\n");
		return true;
	}
	

	if (gs->boxes.get(dst.y, dst.x) && GOALS.get(dst.y, dst.x)) {
		//Cannot handle these cases yet, return false for now (TODO)
		return false;
	}


	bitString localObstacles = localBoxes | localWalls;
	
	
	//First test. Detect 2x2 deadlock patterns. WITH bitString
	if (localObstacles.get(1,2)) {
		int sw1, sw2, sw3, sw4;
		sw1 = (int)localWalls.get(1,1);
		sw2 = (int)localWalls.get(2,1);
		sw3 = (int)localWalls.get(1,3);
		sw4 = (int)localWalls.get(2,3);
		int surWalls = sw1 + sw2 + sw3 + sw4;
		//fprintf(stderr, "surWalls is:%d\n", surWalls);
		if (surWalls > 2) {
			//Deadlock found
			//fprintf(stderr, "Returned true from here, 1\n");
			return true;
		} else {
			if (localObstacles.get(1,1) && localObstacles.get(2,1)) {
				//fprintf(stderr, "Returned true from here, 2\n");
				return true;
			}
			if (localObstacles.get(1,3) && localObstacles.get(2,3)) {
				//fprintf(stderr, "Returned true from here, 3\n");
				return true;
			}
		}
	}
		
	//New attempt at 3x3 deadlock pattern detection. WITH bitString
	if (!localObstacles.get(2,1) && !localObstacles.get(2,3)) {
		//Likely not a deadlock, although not certain (TODO)
		//return false for now
		return false;
	}

	if (!localObstacles.get(1,2)) {
		//Only possible deadlock is with dst as bottom middle tile
		if (localGoals.get(1,2)) {
			//Quick fix. Prevents serious mistakes, but might cause others
			return false;
		}
		if (!localObstacles.get(1,1)) {
			return false;
		}
		if (!localObstacles.get(1,3)) {
			return false;
		}
		if (!localObstacles.get(0,2)) {
			return false;
		}
		
		//All non-corner edges are obstacles.
		if (!localObstacles.get(2,1)) {
			//c3 is an obstacle
			if (!localObstacles.get(0,1)) {
				if (!localWalls.get(1,1)) {
					//Box in c[1][1] can move vertically
					return false;
				} else {
					if (!localObstacles.get(0,3)) {
						if (localWalls.get(0,2)) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				//Deadlock
				return true;
			}
		} else {
			//c1 is an obstacle
			if (!localObstacles.get(0,3)) {
				if (!localWalls.get(1,3))	 {
					//Box in c4 can move vertically
					return false;
				} else {
					if (!localObstacles.get(0,1)) {
						if (localWalls.get(0,2)) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				//Deadlock
				return true;
			}
		}
	} else {
		//TODO
		//dst can be bottom corner box or side middle
		if (!localObstacles.get(2,1)) {
			//c[2][3] must be obstacle. dst can be bottom left
			
			if (!localObstacles.get(0,3)) {
				//Cannot be deadlock
				return false;
			}
			if (!localObstacles.get(1,4)) {
				//Cannot be deadlock
				return false;
			}
			if (!localObstacles.get(0,4)) {
				if (localWalls.get(1,4) || localObstacles.get(2,4)) {
					if (localWalls.get(0,3) || localObstacles.get(0,2)) {
						if (!localGoals.get(1,3)) {
							return true;
						}
					}
				}
			} else {
				if (!localGoals.get(1,3)) {
					return true;
				}
			}
		} else {
			//c[2][1] must be obstacle. dst can be bottom right
			
			if (!localObstacles.get(0,1)) {
				//Cannot be deadlock
				return false;
			}
			if (!localObstacles.get(1,0)) {
				//Cannot be deadlock
				return false;
			}
			
			if (!localObstacles.get(0,0)) {
				if (localWalls.get(1,0) || localObstacles.get(2,0)) {
					if (localWalls.get(0,1) || localObstacles.get(0,2)) {
						if (!localGoals.get(1,1)) {
							return true;
						}
					}
				}
			} else {
				if (!localGoals.get(1,1)) {
					return true;
				}
			}
		}
	}	

	//If no deadlocks were found: return false.
	return false;
}




/**
 * Detects static deadlocks on the map. 
 * These deadlocks won't change throughout the game.
 */
//void findStaticDeadLocks(vector<vector<char> > &map) {
void findStaticDeadLocks() {
	/*
	//Debug print with deadlocks
    fprintf(stdout, "Before detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stdout, "%c", map[i][j]);
        }
        fprintf(stdout, "\n");
    }
    */
    
    int i, j, k;
    
	/*
	static const char EXAMINED = 'e';
	
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
							if (map[i][j+k] != BOX && map[i][j+k] != BOX_ON_GOAL) {
								map[i][j+k] = EXAMINED;
							}
						}
					}
wallRight:			
					if (dle1) {
						for (k = 0; (map[i-1][j+k] == WALL || map[i+1][j+k] == WALL) && (map[i][j+k] != GOAL && map[i][j+k] != BOX_ON_GOAL);k--) {
							if (map[i][j+k] == WALL) {	//Ran into wall, deadlock certain.
								dle2 = k;
								if (map[i][j+k+1] == EXAMINED) {
									map[i][j+k+1] = FREE;
								}
								goto wallLeft;
							} else {
								if (map[i][j+k] != BOX && map[i][j+k] != BOX_ON_GOAL) {
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
							if (map[i+k][j] != BOX && map[i+k][j] != BOX_ON_GOAL) {
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
								if (map[i+k][j] != BOX && map[i+k][j] != BOX_ON_GOAL) {
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
    		
        }
    }
    
    for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	if (map[i][j] == EXAMINED) {
        		map[i][j] = FREE;
        	}
        }
    }

	*/












	
	//WITH bitString
	bitString nonfree = WALLS | GOALS;
	bitString examined = GOALS;
	examined.clear();
	//Aah, it's tricky to get it right
	for (i = 1;i < NR_ROWS-1;i++) {
        for (j = 1;j < NR_COLUMNS-1;j++) {
        	nonfree = nonfree | examined;
        	nonfree = nonfree | DEADLOCKS;
        	if (!nonfree.get(i,j)) { //Only free spaces can be deadlocks
				
				//Horizontal search
				if (WALLS.get(i-1,j) || WALLS.get(i+1,j)) {
					//It's close to a wall, might be deadlock
					
					int dle1 = 0, dle2 = 0;
					//Walk until no wall on the side, or until you run into a wall
					for (k = 0; (WALLS.get(i-1, j+k) || WALLS.get(i+1, j+k)) && !GOALS.get(i, j+k) ; k++) {
						if (WALLS.get(i,j+k)) { //Ran into wall, deadlock possible.
							dle1 = k;
							examined.reset(i,j+k-1);
							goto wallRight2;
						} else {
							examined.set(i,j+k);
						}
					}
wallRight2:			
					if (dle1) {
						for (k = 0; (WALLS.get(i-1, j+k) || WALLS.get(i+1, j+k)) && !GOALS.get(i, j+k) ; k--) {
							
							if (WALLS.get(i,j+k)) { //Ran into wall, deadlock possible.
								dle2 = k;
								examined.reset(i,j+k+1);
								goto wallLeft2;
							} else {
								examined.set(i,j+k);
								
								if (j+k == 0) {
									fprintf(stderr, "Setting examined at the edge\n");
								}
							
							}
						}
					}
wallLeft2:			
					//fprintf(stderr, "HS: dle1 and dle2 have values: %d and %d\n", dle1, dle2);
					if (dle1 && dle2) {
						//Mark deadlock zone
						for (k = dle2+1; k < dle1 ;k++) {
							DEADLOCKS.set(i,j+k);
						}
					}
				}		
				
				//Vertical search
				if (WALLS.get(i,j-1) || WALLS.get(i,j+1)) {
					//It's close to a wall, might be deadlock
					int dle1 = 0, dle2 = 0;
					//Walk until no wall on the side, or until you run into a wall
					for (k = 0; (WALLS.get(i+k,j-1) || WALLS.get(i+k,j+1)) && !GOALS.get(i+k,j) ;k++) {
						if (WALLS.get(i+k,j)) {	//Ran into wall, deadlock possible.
							dle1 = k;
							examined.reset(i+k-1,j);
							goto wallDown2;
						} else {
							examined.set(i+k,j);
						}
					}
wallDown2:			
					if (dle1) {
						for (k = 0; (WALLS.get(i+k,j-1) || WALLS.get(i+k,j+1)) && !GOALS.get(i+k,j) ;k--) {
							if (WALLS.get(i+k,j)) {	//Ran into wall, deadlock certain.
								dle2 = k;
								examined.reset(i+k+1,j);
								goto wallUp2;
							} else {
								examined.set(i+k,j);
							}
						}
					}
wallUp2:				
					//fprintf(stderr, "VS: dle1 and dle2 have values: %d and %d\n", dle1, dle2);
					if (dle1 && dle2) {
						//Mark deadlock zone
						for (k = dle2+1; k < dle1 ;k++) {
							DEADLOCKS.set(i+k,j);
						}
					}
				}
    		}
    		
    		if (WALLS.get(i,j-1) || WALLS.get(i,j+1)) {
    			if (WALLS.get(i-1,j) || WALLS.get(i+1,j)) {
	    			if (!GOALS.get(i,j) && !WALLS.get(i,j)) {
    					DEADLOCKS.set(i,j);
    				}
    			}
    		}
        }
    }
    
    //cerr << "WALLS looks like this: " << WALLS;
    //cerr << "examined looks like this: " << examined;
    //cerr << "DEADLOCKS looks like this: " << DEADLOCKS;

    //Debug print with deadlocks
    /*
    fprintf(stdout, "After detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stderr, "%c", map[i][j]);
        }
        fprintf(stderr, "\n");        
    }
    */
    
}

