#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <queue>

Graph::Graph(int v)
{
	edges.resize(v);
}

void Graph::addEdge(int u, int v)
{
	edges[u].push_back(v);
	edges[v].push_back(u);
}

void Graph::addEdgeCheck(int u, int v)
{
	if (find(edges[u].begin(), edges[u].end(), v) == edges[u].end())
		edges[u].push_back(v);
	if (find(edges[v].begin(), edges[v].end(), u) == edges[v].end())
		edges[v].push_back(u);
}

void Graph::deleteEdge(int u, int v)
{
	for (int i = 0; i < edges[u].size(); i++)
		if (edges[u][i] == v)
		{
			edges[u].erase(edges[u].begin() + i);
			break;
		}
	for (int i = 0; i < edges[v].size(); i++)
		if (edges[v][i] == u)
		{
			edges[v].erase(edges[v].begin() + i);
			break;
		}
}

void Graph::print()
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

void Graph::printVector(vector<int> vec)
{
	for (auto el : vec)
		cout << el << " ";
	cout << endl;
}

vector<int> Graph::findGreedyMatching()
{
	vector<int> M(edges.size(), -1);
	for (int u = 0; u < edges.size(); u++)
	{
		if (M[u] == -1)
		{
			for (auto v : edges[u])
			{
				if (M[v] == -1)
				{
					M[u] = v;
					M[v] = u;
					break;
				}
			}
		}
	}
	return M;
}

vector<int> Graph::findOuterConnectedOutsideForest(const vector<int> &forest, const vector<bool> &isOuter)
{
	for (int u = 0; u < edges.size(); u++)
		if (forest[u] > -1 && isOuter[u])
		{
			for (auto v : edges[u])
			{
				if (forest[v] == -1)
					return { u, v };
			}
		}
	return {};
}

vector<int> Graph::findConnectedOuters(const vector<int> &forest, const vector<bool> &isOuter)
{
	for (int u = 0; u < edges.size(); u++)
	{
		if (forest[u] > -1 && isOuter[u])
		{
			for (auto v : edges[u])
			{
				if (forest[v] > -1 && isOuter[v])
					return { u, v };
			}
		}
	}
	return {};
}

vector<int> Graph::findPath(int u, const vector<int>& forest)
{
	if (forest[u] == u)
		return { u };
	vector<int> result = findPath(forest[u], forest);
	result.push_back(u);
	return result;
}

vector<int> Graph::augment(vector<int> &M, const vector<int> &augmentingPath)
{
	for (int i = 0; i < augmentingPath.size() - 1; i += 2)
	{
		M[augmentingPath[i]] = augmentingPath[i + 1];
		M[augmentingPath[i + 1]] = augmentingPath[i];
	}
	return M;
}

vector<int> Graph::findBlossom(const vector<int> &forest, const vector<int> &p1, const vector<int> &p2)
{
	int i = 0; 
	while (i < p1.size() && i < p2.size() && p1[i] == p2[i])
		i++;
	i--;
	vector<int> result = { p1[i] };
	for (int j = i + 1; j < p1.size(); j++)
		result.push_back(p1[j]);
	for (int j = p2.size() - 1; j > i; j--)
		result.push_back(p2[j]);
	return result;
}

vector< vector<int> > Graph::contract(const vector<int>& blossom, vector<int>& M)
{
	vector< vector<int> > addedEdges;
	int y = blossom[0];
	for (int i = 1; i < blossom.size(); i++)
	{
		int u = blossom[i];
		for (int j = 0; j < edges[u].size(); j++)
		{
			int v = edges[u][j];
			if (v != blossom[i - 1] && v != blossom[(i + 1) % blossom.size()])
			{
				addEdgeCheck(y, v);
				addedEdges.push_back({ u, v });
			}
		}
	}
	return addedEdges;
}

void Graph::lift(vector<int> &M, const vector<int> &blossomVertices, const vector< vector<int> > &addedEdges)
{
	int y = blossomVertices[0];
	for (auto edge : addedEdges)
	{
		int u = edge[0], v = edge[1];
		if (M[y] == v)
		{
			M[y] = -2;
			M[u] = v;
			M[v] = u;
		}
	}

	int start = 0;
	while (start < blossomVertices.size() && M[blossomVertices[start]] < 0)
		start++;

	for (int j = 1; j < blossomVertices.size() - 1; j += 2)
	{
		int i = (start + j) % blossomVertices.size();
		int u = blossomVertices[i % blossomVertices.size()];
		int v = blossomVertices[(i + 1) % blossomVertices.size()];
		M[u] = v;
		M[v] = u;
	}
}

vector<int> Graph::edmonds()
{
	vector<int> M = findGreedyMatching();
	int tmp = 0;
	for (int i = 0; i < M.size(); i++)
	{
		if (M[i] != -1)
			tmp++;
	}
	tmp /= 2;
	if (tmp < edges.size() / 2)
	{
		vector<int> forest(edges.size());
		vector<bool> isOuter(edges.size());
		for (int i = 0; i < forest.size(); i++)
		{
			if (M[i] == -1)
			{
				forest[i] = i;
				isOuter[i] = true;
			}
			else
				forest[i] = -1;
		}
		vector< vector<int> > blossomsVertices;
		vector< vector< vector<int> > > addedEdges;
		vector< vector<int> > yEdges;

		while (true)
		{
			vector<int> edge = findOuterConnectedOutsideForest(forest, isOuter);
			if (!edge.empty())
			{
				int x = edge[0], y = edge[1];
				for (auto z : edges[y])
				{
					if (forest[z] != -2 && M[z] == y)
					{
						forest[y] = x;
						isOuter[y] = false;

						forest[z] = y;
						isOuter[z] = true;
						break;
					}
				}
			}
			else
			{
				edge = findConnectedOuters(forest, isOuter);
				if (!edge.empty())
				{
					int x1 = edge[0], x2 = edge[1];
					vector<int> p1 = findPath(x1, forest);
					vector<int> p2 = findPath(x2, forest);
					if (p1[0] != p2[0])
					{
						reverse(p2.begin(), p2.end());
						vector<int> augmentingPath = p1;
						for (auto el : p2)
							augmentingPath.push_back(el);
						M = augment(M, augmentingPath);
						for (int i = 0; i < edges.size(); i++)
						{
							if (M[i] == -1)
							{
								forest[i] = i;
								isOuter[i] = true;
							}
							else if (M[i] > -1)
								forest[i] = -1;
						}
					}
					else
					{
						vector<int> B = findBlossom(forest, p1, p2);
						blossomsVertices.push_back(B);
						for (int i = 1; i < B.size(); i++)
						{
							for (int j = 0; j < forest.size(); j++)
							{
								if (forest[j] == B[i])
								{
									forest[j] = B[0];
								}
							}
							forest[B[i]] = -2;
							M[B[i]] = -2;
						}
						addedEdges.push_back(contract(B, M));
					}
				}
				else
				{
					break;
				}
			}
		}
		for (int i = blossomsVertices.size() - 1; i >= 0; i--)
		{
			lift(M, blossomsVertices[i], addedEdges[i]);
		}
	}

	vector<int> result;
	for (int i = 0; i < M.size(); i++)
	{
		if (M[i] > -1)
		{
			result.push_back(i);
			result.push_back(M[i]);
			M[M[i]] = -1;
		}
	}
	result.insert(result.begin(), result.size() / 2);
	return result;
}