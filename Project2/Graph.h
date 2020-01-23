#include <vector>

using namespace std;

class Graph
{
	vector< vector< vector<int> > > edges;

	vector<int> naiveBipMatching();
	vector<int> naiveBipMatchingRec(int, int, int, vector<bool>);
public: 
	Graph(int);
	void addEdge(int, int, int, int);
	void printGraph();
	pair< vector< pair<int, int> >, int > findHighways();
};