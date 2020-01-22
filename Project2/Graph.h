#include <vector>

using namespace std;

class Graph
{
	void dfsVisit(int, int, int, int, vector<bool>, vector<int>, vector< pair<vector<int>, pair<int, int> > >&);
	vector<int> findCycle(int, int, vector<int>);
	pair<vector<int>, pair<int, int>> findWhichCycles(vector< vector< pair<vector<int>, pair<int, int> > > >, int, vector<bool>);
public: 
	vector< vector< vector<int> > > edges;

	Graph(int);
	void addEdge(int, int, int, int);
	void printGraph();
	vector< pair<vector<int>, pair<int, int> > > dfsFindCycles(int);
	pair< vector< pair<int, int> >, int > findHighways();
};

