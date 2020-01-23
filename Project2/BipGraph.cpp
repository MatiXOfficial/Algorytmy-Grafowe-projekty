#include "BipGraph.h"
#include <iostream>
#include <algorithm>

BipGraph::BipGraph(int v)
{
	edges.resize(v);
}

void BipGraph::addEdge(int u, int v, int min, int max)
{
	edges[u].push_back({v, min, max});
}

vector<int> BipGraph::naiveMatching()
{
	auto result = naiveMatchingRec(1, INT_MIN, INT_MAX, vector<bool>(edges.size(), false));
	if (!result.empty())
	{
		int c = result[result.size() - 1];
		result.pop_back();
		result.insert(result.begin(), c);
	}
	return result;
}

vector<int> BipGraph::naiveMatchingRec(int u, int cmin, int cmax, vector<bool> visited)
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
		vector<int> result = naiveMatchingRec(u + 1, nmin, nmax, visited);
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