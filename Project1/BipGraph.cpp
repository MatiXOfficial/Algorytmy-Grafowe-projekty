#include "BipGraph.h"
#include <iostream>
#include <algorithm>

BipGraph::BipGraph(int v)
{
	edges.resize(v);
}

void BipGraph::addEdge(int u, int v)
{
	//if (find(left.begin(), left.end(), v) != left.end() || find(right.begin(), right.end(), u) != right.end())
	//	throw invalid_argument("ERROR: This would make the graph not bipartite.");

	if (find(left.begin(), left.end(), u) == left.end())
		left.push_back(u);
	if (find(right.begin(), right.end(), v) == right.end())
		right.push_back(v);

	edges[u].push_back(v);
	edges[v].push_back(u);
}

void BipGraph::print()
{
	for (int u = 0; u < edges.size(); u++)
	{
		cout << u << " -> ";
		for (auto v : edges[u])
		{
			cout << v << ", ";
		}
		cout << endl;
	}
}

vector<int> BipGraph::hopcroftKarp()
{
	vector<int> pairLeft(edges.size(), edges.size());
	vector<int> pairRight(edges.size(), edges.size());
	vector<int> dist(edges.size() + 1);

	while (bfs(pairLeft, pairRight, dist))
	{
		for (int i = 0; i < left.size(); i++)
		{
			if (pairLeft[left[i]] == edges.size())
				dfs(left[i], pairLeft, pairRight, dist);
		}
	}

	vector<int> result = { 0 };
	for (int u = 0; u < edges.size(); u++)
	{
		int v = pairLeft[u];
		if (v != edges.size())
		{
			result[0]++;
			result.push_back(u);
			result.push_back(v);
		}
	}
	return result;
}

bool BipGraph::bfs(vector<int> pairLeft, vector<int> pairRight, vector<int> &dist)
{
	queue<int> Q;

	for (int i = 0; i < left.size(); i++)
	{
		if (pairLeft[left[i]] == edges.size())
		{
			dist[left[i]] = 0;
			Q.push(left[i]);
		}
		else dist[left[i]] = INT_MAX;
	}
	dist[edges.size()] = INT_MAX;

	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();

		if (dist[u] < dist[edges.size()])
		{
			for (auto v : edges[u])
			{
				if (dist[pairRight[v]] == INT_MAX)
				{
					dist[pairRight[v]] = dist[u] + 1;
					Q.push(pairRight[v]);
				}
			}
		}
	}
	return (dist[edges.size()] != INT_MAX);
}

bool BipGraph::dfs(int u, vector<int> &pairLeft, vector<int> &pairRight, vector<int> dist)
{
	if (u == edges.size())
		return true;
	for (auto v : edges[u])
	{
		if (dist[pairRight[v]] == dist[u] + 1)
		{
			if (dfs(pairRight[v], pairLeft, pairRight, dist) == true)
			{
				pairRight[v] = u;
				pairLeft[u] = v;
				return true;
			}
		}
	}
	dist[u] = INT_MAX;
	return false;
}
