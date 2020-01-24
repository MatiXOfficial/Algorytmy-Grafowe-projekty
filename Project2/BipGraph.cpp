#include "BipGraph.h"
#include <iostream>
#include <algorithm>
#include <limits.h>

BipGraph::BipGraph(int v)
{
	VH = (v + 1);
	V = 2 * VH;
	edges.resize(V);
}

void BipGraph::addEdge(int u, int v, int cmin, int cmax)
{
	v = VH + v;
	edges[u].push_back({v, cmin, cmax});
	edges[v].push_back({u, cmin, cmax});
}

vector<int> BipGraph::preMatching()
{
	vector<int> M(V, -1);
	int cmin = INT_MIN, cmax = INT_MAX;
	for (int u = 1; u < V; u++)
	{
		if (u == VH || M[u] != -1)
			continue;
		if (edges[u].empty())
			return {};
		if (edges[u].size() == 1)
		{
			int v = edges[u][0][0];
			if (M[v] == -1)
			{
				cmin = max(cmin, edges[u][0][1]); cmax = min(cmax, edges[u][0][2]);
				if (cmin > cmax)
					return {};
				M[v] = u;
				M[u] = v;
			}
			else if (M[v] != u)
				return {};
		}
	}
	return M;
}

vector<int> BipGraph::hopcroftKarp()
{
	vector<int> M = preMatching();
	if (M.empty())
		return { 0 };
	vector<int> pairLeft(VH, V);
	vector<int> pairRight(V, V);
	vector<int> dist(V + 1);

	for (int u = 1; u < VH; u++)
	{
		int v = M[u];
		if (v != -1)
		{
			pairLeft[u] = v;
			pairRight[v] = u;
		}
	}

	while (bfs(pairLeft, pairRight, dist))
	{
		for (int i = 0; i < VH; i++)
		{
			if (pairLeft[i] == V)
				dfs(i, pairLeft, pairRight, dist);
		}
	}

	vector<int> result = { 0 };
	for (int u = 0; u < VH; u++)
	{
		int v = pairLeft[u];
		if (v != V)
		{
			result[0]++;
			result.push_back(u);
			result.push_back(v);
		}
	}
	return result;
}

bool BipGraph::bfs(const vector<int> &pairLeft, const vector<int> &pairRight, vector<int>& dist)
{
	queue<int> Q;

	for (int i = 0; i < VH; i++)
	{
		if (pairLeft[i] == V)
		{
			dist[i] = 0;
			Q.push(i);
		}
		else dist[i] = INT_MAX;
	}
	dist[V] = INT_MAX;

	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();

		if (dist[u] < dist[V])
		{
			for (auto edge : edges[u])
			{
				int v = edge[0];
				if (dist[pairRight[v]] == INT_MAX)
				{
					dist[pairRight[v]] = dist[u] + 1;
					Q.push(pairRight[v]);
				}
			}
		}
	}
	return (dist[V] != INT_MAX);
}

bool BipGraph::dfs(int u, vector<int>& pairLeft, vector<int>& pairRight, vector<int> &dist)
{
	if (u == V)
		return true;
	for (auto edge : edges[u])
	{
		int v = edge[0];
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

pair< vector< pair<int, int> >, int > BipGraph::findHighways()
{
	auto matching = hopcroftKarp();
	if (matching[0] < VH - 1)
		return make_pair< vector< pair<int, int> >, int >({}, -1);
	for (int i = 2; i < matching.size(); i += 2)
		matching[i] -= VH;
	matching.erase(matching.begin());
	int c = findMinimumCost(matching);
	if (c == -1)
	{
		reverseMatching(matching);
		c = findMinimumCost(matching);
	}
	if (c == -1)
		return make_pair< vector< pair<int, int> >, int >({}, -1);
	pair< vector< pair<int, int> >, int > result({}, c);
	for (int i = 0; i < matching.size(); i += 2)
		result.first.push_back({ matching[i], matching[i + 1] });
	return result;
}

int BipGraph::findMinimumCost(const vector<int> &matching)
{
	int cmin = INT_MIN, cmax = INT_MAX;
	for (int i = 0; i < matching.size(); i += 2)
	{
		int u = matching[i], v = matching[i + 1] + VH, nmin, nmax;
		for (auto edge : edges[u])
		{
			if (edge[0] == v)
			{
				nmin = edge[1];
				nmax = edge[2];
			}
		}
		cmin = max(cmin, nmin);	cmax = min(cmax, nmax);
		if (cmin > cmax)
			return -1;
	}
	return cmin;
}

void BipGraph::reverseMatching(vector<int> &matching)
{
	for (int i = 0; i < matching.size(); i += 2)
	{
		int u = matching[i], v = matching[i + 1] + VH;
		for (auto edge : edges[u])
		{
			if (edge[0] != v)
			{
				v = edge[0];
				break;
			}
		}
		matching[i + 1] = v - VH;
	}
}