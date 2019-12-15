#include <vector>
using namespace std;

class Graph
{
	vector< vector<int> > edges;

	vector<int> findOuterConnectedOutsideForest(vector<int> forest, vector<bool> isOuter);
	vector<int> findConnectedOuters(vector<int> forest, vector<bool> isOuter);
	int findRoot(int u, vector<int> forest);
	vector<int> findPath(int u, vector<int> forest);
	vector<int> augment(vector<int> M, vector<int> augmentingPath);
	vector<int> findBlossom(vector<int> forest, int x1, int x2);
	vector< vector<int> > contract(vector<int> blossom, vector<int> &M);
	void printVector(vector<int> vec);
	void lift(vector<int>& M, vector<int> blossomVertices, vector< vector<int> > blossomEdges);
public:
	Graph(int v);
	void addEdge(int u, int v);
	void deleteEdge(int u, int v);
	void print();
	vector<int> edmonds();
};

