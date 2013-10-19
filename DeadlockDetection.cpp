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

	pos curDir = dst-gs->src.start;
	pos refPos;
	int a, b;
	
	bitString localBoxes(5,4);
	bitString localWalls(5,4);
	bitString localGoals(5,4);
	bitString localDeadlocks(5,4);
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
			}
		}		
	}


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
			if (localWalls.get(1,1) && localWalls.get(2,3)) {
				return true;
			}
			if (localWalls.get(1,3) && localWalls.get(2,1)) {
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
							//goto wallRight2;
							break;
						} else {
							examined.set(i,j+k);
						}
					}
//wallRight2:			
					if (dle1) {
						for (k = 0; (WALLS.get(i-1, j+k) || WALLS.get(i+1, j+k)) && !GOALS.get(i, j+k) ; k--) {
							
							if (WALLS.get(i,j+k)) { //Ran into wall, deadlock possible.
								dle2 = k;
								examined.reset(i,j+k+1);
								//goto wallLeft2;
								break;
							} else {
								examined.set(i,j+k);
								
								if (j+k == 0) {
									fprintf(stderr, "Setting examined at the edge\n");
								}
							
							}
						}
					}
//wallLeft2:			
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
							//goto wallDown2;
							break;
						} else {
							examined.set(i+k,j);
						}
					}
//wallDown2:			
					if (dle1) {
						for (k = 0; (WALLS.get(i+k,j-1) || WALLS.get(i+k,j+1)) && !GOALS.get(i+k,j) ;k--) {
							if (WALLS.get(i+k,j)) {	//Ran into wall, deadlock certain.
								dle2 = k;
								examined.reset(i+k+1,j);
								break;
								//goto WallUp2;
							} else {
								examined.set(i+k,j);
							}
						}
					}
//wallUp2:				
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
    					
    					
    					
    					int l;
    					if (WALLS.get(i-1,j) && WALLS.get(i+1,j)) {
    						if (WALLS.get(i,j-1)) {
	
    							for (l = 1;WALLS.get(i+1,j+l) && WALLS.get(i-1,j+l) && !GOALS.get(i,j+l);l++) {
    								if (!BOXES.get(i,j+l)  &&  !WALLS.get(i,j+l)) {
    									DEADLOCKS.set(i,j+l);
    								}
    							}
    							if (WALLS.get(i+1,j+l) || WALLS.get(i-1,j+l)) {
    								if (!BOXES.get(i,j+l)  &&  !WALLS.get(i,j+l) && !GOALS.get(i,j+l)) {
    									DEADLOCKS.set(i,j+l);
    								}
    							}
    						
    						} else if (WALLS.get(i,j+1)) {
    						
    							for (l = -1;WALLS.get(i+1,j+l) && WALLS.get(i-1,j+l)  && !GOALS.get(i,j+l) ;l--) {
    								if (!BOXES.get(i,j+l)  &&  !WALLS.get(i,j+l)) {
    									DEADLOCKS.set(i,j+l);
    								}
    							}
    							if (WALLS.get(i+1,j+l) || WALLS.get(i-1,j+l)) {
    								if (!BOXES.get(i,j+l)  &&  !WALLS.get(i,j+l) && !GOALS.get(i,j+l)) {
    									DEADLOCKS.set(i,j+l);
    								}
    							}
    						
    						}
    					} else if (WALLS.get(i,j-1) && WALLS.get(i,j+1)) {
    						if (WALLS.get(i-1,j)) {
    						
    							for (l = 1;WALLS.get(i+l,j+1) && WALLS.get(i+l,j-1) && !GOALS.get(i+l,j+l);l++) {
    								if (!BOXES.get(i+l,j+l)  &&  !WALLS.get(i+l,j+l)) {
    									DEADLOCKS.set(i+l,j);
    								}
    							}
    							if (WALLS.get(i+l,j+1) || WALLS.get(i+l,j-1)) {
    								if (!BOXES.get(i+l,j)  &&  !WALLS.get(i+l,j) && !GOALS.get(i+l,j)) {
    									DEADLOCKS.set(i+l,j);
    								}
    							}
    						
    						} else if (WALLS.get(i+1,j)) {
    						
    							for (l = -1;WALLS.get(i+l,j+1) && WALLS.get(i+l,j-1) && !GOALS.get(i+l,j+l);l--) {
    								if (!BOXES.get(i+l,j+l)  &&  !WALLS.get(i+l,j+l)) {
    									DEADLOCKS.set(i+l,j);
    								}
    							}
    							if (WALLS.get(i+l,j+1) || WALLS.get(i+l,j-1)) {
    								if (!BOXES.get(i+l,j)  &&  !WALLS.get(i+l,j) && !GOALS.get(i+l,j)) {
    									DEADLOCKS.set(i+l,j);
    								}
    							}
    						}
    					}	
						
	
	
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
    fprintf(stderr, "After detecting deadlocks:\n");
	for(int i = 0;i < map.size();i++){
        for(int j = 0;j < map[i].size();j++){
        	fprintf(stderr, "%c", map[i][j]);
        }
        fprintf(stderr, "\n");        
    }
    */
    
}

