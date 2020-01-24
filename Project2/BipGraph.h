#include <vector>
#include <queue>
using namespace std;

class BipGraph
{
	int V;
	int VH;
	vector< vector< vector<int> > > edges;

	vector<int> preMatching();
	bool bfs(const vector<int> &, const vector<int> &, vector<int> &);
	bool dfs(int u, vector<int> &, vector<int> &, vector<int> &);
	int findMinimumCost(const vector<int> &);
	void reverseMatching(vector<int> &);
public:
	BipGraph(int v);
	void addEdge(int, int, int, int);

	vector<int> hopcroftKarp();
	pair< vector< pair<int, int> >, int > findHighways();
};