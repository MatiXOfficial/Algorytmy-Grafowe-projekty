#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<int> V(n);
	vector<int> E(n);
	vector< vector< vector<int> > > edges(n);

	for (int i = 0; i < n; i++)
	{
		cin >> V[i];
		cin >> E[i];

		for (int j = 0; j < E[i]; j++)
		{
			int u, v, cmin, cmax;
			cin >> u >> v >> cmin >> cmax;
			edges[i].push_back({ u, v, cmin, cmax });
		}
	}

	for (int i = 0; i < n; i++)
	{
		Graph G(V[i]);
		for (int j = 0; j < E[i]; j++)
		{
			G.addEdge(edges[i][j][0], edges[i][j][1], edges[i][j][2], edges[i][j][3]);
		}
		auto highways = G.findHighways();
		cout << highways.second << endl;
		if (highways.second != -1)
		{
			for (auto highway : highways.first)
			{
				cout << highway.first << " " << highway.second << endl;
			}
		}
	}
}