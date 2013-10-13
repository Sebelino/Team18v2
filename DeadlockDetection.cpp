#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "DeadlockDetection.h"
#include "Constants.h"


class GameState;

using namespace std;

void findStaticDeadLocks(vector<vector<char> > &map) {
	//TODO

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

