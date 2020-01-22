#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <limits.h>

using namespace std;

Graph::Graph(int V)
{
	edges.resize(V + 1);
}

void Graph::addEdge(int u, int v, int min, int max)
{
	edges[u].push_back({ v, min, max });
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

vector< pair<vector<int>, pair<int, int> > > Graph::dfsFindCycles(int start)
{
	vector< pair<vector<int>, pair<int, int> > > result;
	vector<bool> visited(edges.size(), false);
	visited[start] = true;
	vector<int> parents(edges.size(), -1);
	for (auto edge : edges[start])
	{
		int v = edge[0];
		if (v >= start)
		{
			int cmin = edge[1];
			int cmax = edge[2];
			parents[v] = start;
			dfsVisit(start, v, cmin, cmax, visited, parents, result);
		}
	}
	//sort(result.begin(), result.end(), [](pair<vector<int>, int> a, pair<vector<int>, int> b) { return a.first.size() < b.first.size(); });
	return result;
}

void Graph::dfsVisit(int start, int u, int cmin, int cmax, vector<bool> visited, vector<int> parents, vector< pair<vector<int>, pair<int, int> > > &result)
{
	visited[u] = true;
	for (auto edge : edges[u])
	{
		int v = edge[0];
		int nmin = max(cmin, edge[1]);
		int nmax = min(cmax, edge[2]);
		if (v >= start && nmin <= nmax)
		{
			if (v == start)
			{
				vector<int> cycle = findCycle(start, u, parents);
				cycle.push_back(start);
				result.push_back(make_pair(cycle, make_pair(nmin, nmax)));
			}
			if (visited[v] == false)
			{
				parents[v] = u;
				dfsVisit(start, v, nmin, nmax, visited, parents, result);
			}
		}
	}
}

vector<int> Graph::findCycle(int u, int v, vector<int> parents)
{
	if (u == v)
		return { u };

	vector<int> result = findCycle(u, parents[v], parents);
	result.push_back(v);
	return result;
}

pair< vector< pair<int, int> >, int > Graph::findHighways()
{
	vector< vector< pair<vector<int>, pair<int, int> > > > cycles(edges.size());
	for (int i = 1; i < edges.size(); i++)
	{
		cycles[i] = dfsFindCycles(i);
	}
	auto whichCycles = findWhichCycles(cycles, 0, vector<bool>(edges.size(), false));
	if (whichCycles.first.empty())
		return make_pair<vector< pair<int, int> >, int>({}, -1);
	pair< vector< pair<int, int> >, int > result({}, whichCycles.second.first);
	for (int i = 1; i < whichCycles.first.size(); i++)
	{
		int j = whichCycles.first[i];
		if (j != -1)
		{
			auto cycle = cycles[i][j].first;
			for (int k = 0; k < cycle.size() - 1; k++)
			{
				result.first.push_back(make_pair(cycle[k], cycle[k + 1]));
			}
		}
	}
	return result;
}

pair<vector<int>, pair<int, int>> Graph::findWhichCycles(vector< vector< pair<vector<int>, pair<int, int> > > > cycles, int i, vector<bool> usedVertices)
{
	if (i == 0)
	{
		usedVertices[0] = true;
		auto result = findWhichCycles(cycles, 1, usedVertices);
		if (!result.first.empty())
			result.first.insert(result.first.begin(), -1);
		return result;
	}
	if (i == cycles.size())
	{
		for (int i = 1; i < usedVertices.size(); i++)
			if (usedVertices[i] == false)
				return {};
		return make_pair<vector<int>, pair<int, int>>({ -1 }, make_pair(INT_MIN, INT_MAX));
	}
	if (cycles[i].empty() || usedVertices[i] == true)
	{
		auto result = findWhichCycles(cycles, i + 1, usedVertices);
		result.first.insert(result.first.begin(), -1);
		if (result.first.size() > 1)
			return result;
		return {};
	}
	for (int j = 0; j < cycles[i].size(); j++)
	{
		bool toTake = true;
		vector<bool> usedVerticesNew = usedVertices;
		for (int k = 0; k < cycles[i][j].first.size() - 1; k++)
		{
			int u = cycles[i][j].first[k];
			if (usedVerticesNew[u] == true)
			{
				toTake = false;
				break;
			}
			usedVerticesNew[u] = true;
		}
		pair<vector<int>, pair<int, int>> result;
		if (toTake == true)
		{
			result = findWhichCycles(cycles, i + 1, usedVerticesNew);
			result.first.insert(result.first.begin(), j);
			if (result.first.size() > 1 && max(cycles[i][j].second.first, result.second.first) <= min(cycles[i][j].second.second, result.second.second))
			{
			result.second.first = max(cycles[i][j].second.first, result.second.first);
			result.second.second = min(cycles[i][j].second.second, result.second.second);
			return result;
			}
		}
		else
		{
			result = findWhichCycles(cycles, i + 1, usedVertices);
			result.first.insert(result.first.begin(), -1);
			if (result.first.size() > 1)
			{
				return result;
			}
		}
	}
	return {};
}