#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <queue>

using namespace std;

// Read from stdin.
vector<string> readBoard() {
	vector<string> board;
	for (string line; getline(cin, line);)
		board.push_back(line);
    return board;
}

// Prints out the input lines.
void display(vector<string>& lines) {
    for(vector<string>::const_iterator i=lines.begin();i != lines.end();++i){
        cout << *i << endl;
    }
}

// Returns a pair (x,y) representing the position of (an) @.
// Returns (-1,-1) if no such character exists.
// Returns (-2,-2) if such a character exists on a goal.
pair<int,int> startPosition(vector<string> & board){
    // Får runtime-error om jag ens använder brädstorleken i en beräkning.
    int esrnt = board.size();
    bool sthswrong = (esrnt >= 0);
//    while(1){}
    //Timelimit
    if(sthswrong){
    while(1){}
    }
    for(int i = 0;i < esrnt;i++){
    //Runtime
        for(int j = 0;j < board[i].length();j++){
        //Runtime
            if(board[i][j] == '+'){
                //Runtime
                return make_pair(-2,-2);
            }
            if(board[i][j] == '@'){
                return make_pair(j,i);
            }
        }
    }
    while(1){}
    return make_pair(-1,-1);
}

vector<pair<int,int> > goalPositions(vector<string> & board){
    vector<pair<int,int> > goals;
    for(int i = 0;i < board.size();i++){
        for(int j = 0;j < board[i].length();j++){
            if(board[i][j] == '.'){
                goals.push_back(make_pair(j,i));
            }
        }
    }
    return goals;
}

int matrixWidth(vector<string> lines){
    int max;
    for(int i = 0;i < lines.size();i++){
        if(lines[i].length() > max){
            max = lines[i].length();
        }
    }
    return max;
}

vector<pair<int,int> > successors(vector<string> & board,pair<int,int> node){
    vector<pair<int,int> > ss;
    int diffs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for(int i = 0;i < 4;i++){
        int x = node.first+diffs[i][0];
        int y = node.second+diffs[i][1];
        if(x >= 0
                && x < matrixWidth(board)
                && y >= 0
                && y < board.size()
                && (board[y][x] == ' '
                || board[y][x] == '.')){
            ss.push_back(make_pair(x,y));
        }
    }
    return ss;
}

bool contains(pair<int,int> x,vector<pair<int,int> > v){
    for(int i = 0;i < v.size();i++){
        if(x.first == v[i].first && x.second == v[i].second){
            return true;
        }
    }
    return false;
}

// Generic search algorithm. Returns a path to the goal, if accessible.
// Returns a path with (-1,-1) if there is no such path.
vector<pair<int,int> > pathfinding(vector<string> & board){
    //Timelimit
    queue<vector<pair<int,int> > > q;
    vector<pair<int,int> > rootPath;
    vector<pair<int,int> > goals = goalPositions(board);

    //Timelimit
    int w = matrixWidth(board);
    int h = board.size();
    int visited[w][h]; // 0 |-> Not visited, 1 |-> Fringe, 2 |-> Visited.
    //Timelimit
    for(int i = 0;i < h;i++){
        for(int j = 0;j < w;j++){
            visited[j][i] = 0;
        }
    }
    //Timelimit
    rootPath.push_back(startPosition(board));
    //Runtime
    if(rootPath.back().first == -1 && rootPath.back().second == -1){
        vector<pair<int,int> > nopath;
        nopath.push_back(pair<int,int>(-1,-1));
        return nopath;
    }
    //Runtime
    if(rootPath.back().first == -2 && rootPath.back().second == -2){
        return vector<pair<int,int> >();
    }
    //Runtime
    q.push(rootPath);

    //Runtime
    while(1){
        if(q.empty()){
            vector<pair<int,int> > nopath;
            nopath.push_back(pair<int,int>(-1,-1));
            return nopath;
        }
        vector<pair<int,int> > path = q.front();
        q.pop();
        visited[path.back().first][path.back().second] = 2;
        if(contains(path.back(),goals)){
            return path;
        }
        vector<pair<int,int> > succs = successors(board,path.back());
        for(int i = 0;i < succs.size();i++){
            int x = succs[i].first;
            int y = succs[i].second;
            if(visited[x][y] == 0){
                visited[x][y] = 1;
                vector<pair<int,int> > newPath = path;
                newPath.push_back(succs[i]);
                q.push(newPath);
            }
        }
    }
}

// Returns directions, given a collection of lines that make up the board.
string answer(vector<string>& board){
    vector<pair<int,int> > path = pathfinding(board);
    if(path.size() == 1 && path.front().first == -1 && path.front().second == -1){
        return "no path\n";
    }

    string directions = "";
    for(int i = 0;i < (path.size()<=0 ? 0 : path.size()-1);i++){
        if(path[i+1].first == path[i].first+1 && path[i+1].second == path[i].second){
            directions += 'R';
        }else if(path[i+1].first == path[i].first-1 && path[i+1].second == path[i].second){
            directions += 'L';
        }else if(path[i+1].first == path[i].first && path[i+1].second == path[i].second+1){
            directions += 'D';
        }else if(path[i+1].first == path[i].first && path[i+1].second == path[i].second-1){
            directions += 'U';
        }
    }
    directions += '\n';
    return directions;
}

int main(int argc, char **argv) {
	vector<string> board;
    board = readBoard();
    // Obs. blankrad för triviala fallet.
    cout << answer(board);
    return 0;
}

