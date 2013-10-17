
#include <list>
//#include <omp.h>
#include "Heuristics.h"
#include "Constants.h"

using namespace std;



int heuristicEvenBetter(GameState& g) {
	//bool debug = false;

	//if(debug)
	//	cerr << "=========================" << endl;
	vector<pos> boxes;
	vector<pos> goals;
	vector<int> binds; //each index is the goal which is bound to the box which is the value of that index.

	int numPairs = 0;
	int score = 0;
	//double start = omp_get_wtime();
	//do some counting first, find all boxes and goals.
	for(int i = 1; i < g.board.size() -1; i++) {
		for(int j = 1; j < g.board[i].size()-1;j++) {
			if(g.board[i][j] == GOAL || g.board[i][j] == PLAYER_ON_GOAL || g.board[i][j] == BOX_ON_GOAL) {
				goals.push_back(pos(i,j));
			} 

			if(g.board[i][j] == BOX || g.board[i][j] == BOX_ON_GOAL) {
				boxes.push_back(pos(i,j));
			}
		}
	}
	//double end = omp_get_wtime();
	//cerr << "Part 1 took " << (end-start) * 1000000 << endl;
	
	numPairs = goals.size();
	//for(int i = 0; i < numPairs; i++) {
	//	score += heuristicDistance(goals[i],boxes[i]);
	//}
	//start = omp_get_wtime();
	//do the initial assignment, start from the goal side.
	vector<vector<int> > dists(numPairs);
	for(int i = 0; i < numPairs; i++) {
		dists[i].reserve(numPairs);
		//dists[i].reserve(numPairs);
	}
	binds.reserve(numPairs);

	for(int i = 0; i < numPairs; i++) {
		pos& goal = goals[i];
		for(int j = 0; j < numPairs; j++) {
			dists[i].push_back(heuristicDistance(goal,boxes[j]));
			
		}
		binds.push_back(i);
		score += dists[i][i];
	}

	//end = omp_get_wtime();
	//cerr << "Part 2 took " << (end-start) * 1000000 << endl;
	//initial assignments done. Now improve it.

	//note that this is not guranteed to give a 100% perfect assignment. However, it should be pretty good.
	//start = omp_get_wtime();
	
	int numIterations = numPairs/2; //number of times we will improve our bindings at maximum.
								  //note that during each iteration

	/*if(debug) {
		cerr << "Current score is " << score;
		cerr << "Number of pairs is " << numPairs << endl;
		for(int i = 0; i < numPairs;i++) {
			cerr << "Initial bounds : (" << goals[i].x << "," << goals[i].y << ")-(" << boxes[i].x << "," <<boxes[i].y << ")" << endl;
		}
	}*/

	bool *changes = new bool[numPairs];
	bool changeFound = true;
	for(int iter = 0; iter < numIterations && changeFound; iter++) {

		//int variant = numIterations%2;
		changeFound = false;
		
		for(int i = 0; i < numPairs; i++) {
			changes[i] = false;
		}
		
		for(int i = 0; i < numPairs; i++) {
			bool localChange = false;
			//the other pair to try and swap with
			for(int j = 0; j < numPairs; j++) {
				if(!(j < i && !changes[j]))
				if(i != j) { //dont try and swap with self
					//pos& gp = goals[i];
					//pos& bp = boxes[binds[i]];

					//pos& go = goals[j];
					//pos& bo = boxes[binds[j]];

					int curDist = dists[i][binds[i]] + dists[j][binds[j]];
					int newDist = dists[i][binds[j]] + dists[j][binds[i]];
					//if(debug)
					//	cerr << "cureDist " << curDist << ", newDist " << newDist << endl;

					if(newDist < curDist) {
						localChange = true;
						changeFound = true;
						//then swap
						//swap binds
						int tmp = binds[i];
						binds[i] = binds[j];
						binds[j] = tmp;
	
						//update total score
						score = score - (curDist - newDist);

						//if(debug)
						//	cerr << "Updating score to " << score << endl;

					}
				}
			}
			if(localChange) {
				changes[i] = true;
			}

		}
		//if(!changeFound)
		//	break;
	}

	//end = omp_get_wtime();
	//cerr << "Part 3 took " << (end-start) * 1000000 << endl;

	delete[] changes; 
	//cerr << "Heuristic score is " << score << endl;
	//cerr << "Heuristic distance is " << aStarDistance(g) << endl;
	score += aStarDistance(g);

	if(g.parent != NULL) {
		if(g.src.start == g.parent->src.end)  {//same box
			score = score/1.5;
			//if(debug)
			//cerr << "Same Box! g.src = " << g.src.start.x << " " << g.src.start.y << " g.parent.end " 
			//	<< g.parent->src.end.x << g.parent->src.end.y << endl;
		}
	}
	//if(debug)
	//	cerr << "final score is " << score << endl;
	
	return -score;
}

int aStarDistance(GameState& g) {
	return g.depth>>2;
}

/*
Set the heuristic score of the given gamestate.
Note: the reference to the GameState is not a const because two things may
change value in the given gamestate: the score and some boxes that are pushed into
corners will become walls.
*/
void heuristicSmarter(GameState& g) {
	/*
	1.Sort all goals by some heuristic
2.For each goal, find closest box, that is not taken already.
3.Mark box as taken
4.Add to score the distance (or subtract)*/

	//4 classes of goals
	vector<pos> buckets[4]; //puts each goal in a bucket.

	for(int i = 1; i < g.board.size() -1; i++) {
		for(int j = 1; j < g.board[i].size()-1;j++) {
			if(g.board[i][j] == GOAL || g.board[i][j] == PLAYER_ON_GOAL || g.board[i][j] == BOX_ON_GOAL) {
				if(g.board[i][j] == BOX_ON_GOAL && isBoxWall(g,i,j)) {
					g.board[i][j] = WALL; //TODO ganska fult
				} else {
					int tmp = checkGoalClass(g,i,j);
					//if(tmp < 0 || tmp > 3)
					//	cerr << "whining, tmp is " << tmp << endl;
					if(tmp == -1) {
						//deadlock detected
						g.score = -10000000;
						return;
					}

					buckets[tmp].push_back(pos(i,j));
				}
			} 
		}
	}

	

	//step 2
	//complexity of this step is O(Area of board + (numberOfGoals^2 / 2)) TODO not completely sure, see below
	
	//actually this is step 3 and 4 at the same time as well.

	//first get a list of all boxes
	list<pos> allBoxes;
	for(int i = 1; i < g.board.size() -1; i++) {
		for(int j = 1; j < g.board[i].size()-1;j++) {
			if(g.board[i][j] == BOX || g.board[i][j] == BOX_ON_GOAL) {
				allBoxes.push_back(pos(i,j));
			}
		}
	}
	//then find the closest boxes and remove them from the list, while incrementing score.
	int score = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < buckets[i].size(); j++) {
			//for each goal
			//find the minimum distance to a box.
			list<pos>::iterator bit;
			int shortestDist = 10000000; //something very big, I don't think we will have maps this big!
			list<pos>::iterator shortestDistP;
			pos& goalPos = buckets[i][j];
			//cerr << "goalPos is in bucket " << i << ", the vector has size " << buckets[i].size() << endl;
			//cerr << "Coordinates are (" << goalPos.x << "," << goalPos.y << ")" << endl;
			for(bit = allBoxes.begin(); bit != allBoxes.end(); bit++) {
				int herDist = heuristicDistance(goalPos, *bit);
				if(herDist < shortestDist) {
					shortestDistP = bit;
					shortestDist = herDist;
				}
			}
			allBoxes.erase(shortestDistP); //Takes constant time!

			//add some extra score to a box thats far away, to make it more worth to actually push a box into place.


			score += shortestDist;
			//cerr << "Score is now " << score << endl;
		}
	}
	//score is now the total heuristic distance
	

	g.score = -score;
}

/*
* Private function for the heuristic function - return the class of a specific goal.
* Classes are:
* 0 - Goals with 3 walls next to them.
* 1 - Goals with 2 walls next to eachother (not 2 opposite walls)
* 2 - Goals with 1 wall next to them.
* 3 - Goals with 0 walls next to them or 2 opposite walls.
*/
int checkGoalClass(GameState& g, int i, int j) {
	int clas = 0;
	bool topWall = false;
	bool leftWall = false;
	bool rightWall = false;
	bool botWall = false;

	if(g.board[i-1][j] == WALL || (g.board[i-1][j] == BOX_ON_GOAL && isBoxWall(g,i-1,j))) {
		topWall = true;
		clas++;
	}
	if(g.board[i+1][j] == WALL || (g.board[i+1][j] == BOX_ON_GOAL && isBoxWall(g,i+1,j))) {
		botWall = true;
		clas++;
	}
	if(g.board[i][j+1] == WALL || (g.board[i][j+1] == BOX_ON_GOAL && isBoxWall(g,i,j+1))) {
		rightWall = true;
		clas++;
	}
	if(g.board[i][j-1] == WALL || (g.board[i][j-1] == BOX_ON_GOAL && isBoxWall(g,i,j-1))) {
		leftWall = true;
		clas++;
	}

	if((clas == 2) && ((topWall && botWall) || (leftWall && rightWall))) {
		clas = 0; //a so-called pathway
	}

	//if(clas == 4) {
	//	cerr << g.board[i-1][j] << " " << g.board[i+1][j] << " " << g.board[i][j+1] << " " << g.board[i][j-1] << endl;
	//}
		//clas = 3;

	return 3-clas; //reverse, so that high priority gets class 0.

}

/*
* Check if the box on the current position (not checked that it is a box btw)
* has 2 walls next to it so that it cannot be moved. Will do at maximum 4 comparisions.
*/
bool isBoxWall(GameState& g, int i, int j) {
	bool northWall = false;
	if(g.board[i-1][j] == WALL) {
		northWall = true;
	}

	//this might look a bit sketchy but does as few comparisions as possible
	if(northWall) {
		if(g.board[i][j-1] == WALL || g.board[i][j+1] == WALL)
			return true;
		else
			return false;
	} else {
		if(g.board[i+1][j] == WALL) {
			if(g.board[i][j-1] == WALL || g.board[i][j+1] == WALL)
				return true;
			else
				return false;
		} else {
			return false;
		}
	}
}

int heuristicDistance(const pos& p1, const pos& p2) {
	int dist = abs(p1.x-p2.x)+abs(p1.y-p2.y);
	//int extra_dist = 0;
	//if(dist >= DIM_RETURN_DISTANCE)
	//	extra_dist = _DRA[DIM_RETURN_DISTANCE-1];
	//else
	//	extra_dist = _DRA[dist];
	//return dist+extra_dist;
	return dist;
}
