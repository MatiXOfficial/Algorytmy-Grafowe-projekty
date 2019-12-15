#include <vector>
using namespace std;

class Graph
{
	vector< vector<int> > edges;

	vector<int> findOuterConnectedOutsideForest(const vector<int> &forest, const vector<bool> &isOuter);
	vector<int> findConnectedOuters(const vector<int>& forest, const vector<bool>& isOuter);
	vector<int> findPath(int u, const vector<int> &forest);
	vector<int> augment(vector<int>& M, const vector<int>& augmentingPath);
	vector<int> findBlossom(const vector<int>& forest, const vector<int>& p1, const vector<int>& p2);
	vector< vector<int> > contract(const vector<int> &blossom, vector<int> &M);
	void printVector(vector<int> vec);
	void lift(vector<int>& M, const vector<int>& blossomVertices, const vector< vector<int> >& addedEdges);
public:
	Graph(int v);
	void addEdge(int u, int v);
	void addEdgeCheck(int u, int v);
	void deleteEdge(int u, int v);
	void print();
	vector<int> edmonds();
};

