
#include <list>

#include "Heuristics.h"
#include "Constants.h"

using namespace std;

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
					buckets[checkGoalClass(g,i,j)].push_back(pos(i,j));
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
	int extra_dist = 0;
	if(dist >= DIM_RETURN_DISTANCE)
		extra_dist = _DRA[DIM_RETURN_DISTANCE-1];
	else
		extra_dist = _DRA[dist];
	return dist+extra_dist;
}
