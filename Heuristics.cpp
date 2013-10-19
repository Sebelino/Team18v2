
#include <list>
//#include <omp.h>
#include "Heuristics.h"
#include "Constants.h"

using namespace std;

int heuristicEvenBetter(GameState& g) {
	//bool debug = false;

	//if(debug)
	//	cerr << "=========================" << endl;

	//each index is the goal which is bound to the box which is the value of that index.
	//double start = omp_get_wtime();
	//do some counting first, find all boxes and goals.
	vector<pos> boxes = g.boxes.getPosVector(NR_BOXES);
	vector<pos> goals = VEC_GOALS;
	vector<int> binds;
	
	int numPairs = 0;
	int score = 0;
	
	//double end = omp_get_wtime();
	//cerr << "Part 1 took " << (end-start) * 1000000 << endl;
	
	numPairs = goals.size();
	//for(int i = 0; i < numPairs; i++) {
	//	score += heuristicDistance(goals[i],boxes[i]);
	//}
	//start = omp_get_wtime();
	//do the initial assignment, start from the goal side.

	int npsq = numPairs*numPairs;
	vector<int> dists;
	dists.reserve(npsq);
	binds.reserve(numPairs);
	
	for(int i = 0; i < numPairs; i++) {
		pos& goal = goals[i];
		for(int j = 0; j < numPairs; j++) {
			dists.push_back(heuristicDistance(goal,boxes[j]));
		}
		binds.push_back(i);
		score += dists[i*numPairs+i];
	}
	//fprintf(stderr, "Came here1\n");
	
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

					int curDist = dists[i*numPairs+binds[i]] + dists[j*numPairs+binds[j]];
					int newDist = dists[i*numPairs+binds[j]] + dists[j*numPairs+binds[i]];
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
	//fprintf(stderr, "Came here2\n");
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
			//  cerr << "Same Box! g.src = " << g.src.start.x << " " << g.src.start.y << " g.parent.end " 
			//	<< g.parent->src.end.x << g.parent->src.end.y << endl;
		}
	}
	//fprintf(stderr, "Came here3\n");
	//if(debug)
	//	cerr << "final score is " << score << endl;
	
	return -score;
}

inline int aStarDistance(GameState& g) {
	return g.depth>>2;
}


inline int heuristicDistance(const pos& p1, const pos& p2) {
	int dist = abs(p1.x-p2.x)+abs(p1.y-p2.y);
	//int extra_dist = 0;
	//if(dist >= DIM_RETURN_DISTANCE)
	//	extra_dist = _DRA[DIM_RETURN_DISTANCE-1];
	//else
	//	extra_dist = _DRA[dist];
	//return dist+extra_dist;
	return dist;
}

