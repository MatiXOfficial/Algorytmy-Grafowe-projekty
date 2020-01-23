#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <limits.h>

using namespace std;

Graph::Graph(int V)
{
	edges.resize(V + 1);
}

void Graph::addEdge(int u, int v, int cmin, int cmax)
{
	edges[u].push_back({ v, cmin, cmax });
}

void Graph::printGraph()
{
	for (int u = 1; u < edges.size(); u++)
	{
		cout << u << ": ";
		for (auto edge : edges[u])
		{
			cout << "->" << edge[0] << " [" << edge[1] << ", " << edge[2] << "] ";
		}
		cout << endl;
	}
	cout << endl;
}

pair< vector< pair<int, int> >, int > Graph::findHighways()
{
	auto matching = naiveBipMatching();
	if (matching.empty())
		return make_pair< vector< pair<int, int> >, int >({}, -1);
	pair< vector< pair<int, int> >, int > result({}, matching[0]);
	for (int i = 1; i < matching.size(); i += 2)
	{
		result.first.push_back(make_pair(matching[i], matching[i + 1]));
	}
	return result;
}

vector<int> Graph::naiveBipMatching()
{
	auto result = naiveBipMatchingRec(1, INT_MIN, INT_MAX, vector<bool>(edges.size(), false));
	if (!result.empty())
	{
		int c = result[result.size() - 1];
		result.pop_back();
		result.insert(result.begin(), c);
	}
	return result;
}

vector<int> Graph::naiveBipMatchingRec(int u, int cmin, int cmax, vector<bool> visited)
{
	if (u == edges.size())
		return { cmin };
	if (edges[u].empty())
		return {};
	for (auto edge : edges[u])
	{
		int v = edge[0], nmin = max(cmin, edge[1]), nmax = min(cmax, edge[2]);
		if (visited[v] || nmin > nmax)
			continue;
		visited[v] = true;
		vector<int> result = naiveBipMatchingRec(u + 1, nmin, nmax, visited);
		if (result.empty())
		{
			visited[v] = false;
			continue;
		}
		result.insert(result.begin(), { u, v });
		return result;
	}
	return {};
}