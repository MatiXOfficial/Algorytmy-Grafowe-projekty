#include <iostream>
#include <string>
#include <vector>

#include "BipGraph.h"
#include "Graph.h"

using namespace std;

int main()
{
	string name;
	cin >> name;

	int n, k;
	cin >> n >> k;

	for (int i = 0; i < k; i++)
	{
		int clerk;
		cin >> clerk;
	}

	int m;
	cin >> m;

	vector< vector<int> > appointments;
	for (int i = 0; i < m; i++)
	{
		int u, v;
		cin >> u >> v;
		appointments.push_back({ u, v });
	}

	//////////////////////////////////////////////
	// Hopcroft-Karp Algorithm
	/*BipGraph G(n);
	for (auto appointment : appointments)
	{
		G.addEdge(appointment[0], appointment[1]);
	}
	vector<int> result = G.hopcroftKarp();*/
	////////////////////////////////////////////// 
	// Edmonds (Blossom) Algorithm
	Graph G(n);
	for (auto appointment : appointments)
	{
		G.addEdge(appointment[0], appointment[1]);
	}
	vector<int> result = G.edmonds();
	//////////////////////////////////////////////

	cout << result[0] << endl;
	for (int i = 1; i < result.size(); i += 2)
		cout << result[i] << " " << result[i + 1] << endl;
}