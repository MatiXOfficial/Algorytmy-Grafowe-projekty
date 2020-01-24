#include <vector>
#include <queue>
using namespace std;

class BipGraph
{
	vector< vector<int> > edges;
	vector<int> left;
	vector<int> right;

	bool bfs(vector<int> pairLeft, vector<int> pairRight, vector<int> &dist);
	bool dfs(int u, vector<int> &pairLeft, vector<int> &pairRight, vector<int> dist);
public:
	BipGraph(int v);
	void addEdge(int u, int v);
	void print();

	vector<int> hopcroftKarp();
};
