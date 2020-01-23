#include <vector>
#include <queue>
using namespace std;

class BipGraph
{
	vector< vector< vector<int> > > edges;

	vector<int> naiveMatchingRec(int, int, int, vector<bool>);
public:
	BipGraph(int);
	void addEdge(int, int, int, int);
	void print();

	vector<int> naiveMatching();
};