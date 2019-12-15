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

vector<bool> Graph::findOuterInner(vector<int> forest, vector<int> depth)	//outer - true, inner - false
{
	vector<bool> result(edges.size());
	for (int i = 0; i < forest.size(); i++)
	{
		if (depth[i] % 2 == 0)
			result[i] = true;
		else
			result[i] = false;
	}
	return result;
}

vector<int> Graph::findOuterConnectedOutsideForest(vector<int> forest, vector<bool> isOuter)
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

vector<int> Graph::findConnectedOuters(vector<int> forest, vector<bool> isOuter)
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

int Graph::findRoot(int u, vector<int> forest)
{
	if (forest[u] == u)
		return u;
	return findRoot(forest[u], forest);
}

vector<int> Graph::findPath(int u, vector<int> forest)
{
	if (forest[u] == u)
		return { u };
	vector<int> result = findPath(forest[u], forest);
	result.push_back(u);
	return result;
}

vector<int> Graph::augment(vector<int> M, vector<int> augmentingPath)
{
	for (int i = 0; i < augmentingPath.size() - 1; i += 2)
	{
		M[augmentingPath[i]] = augmentingPath[i + 1];
		M[augmentingPath[i + 1]] = augmentingPath[i];
	}
	return M;
}

vector<int> Graph::findBlossom(vector<int> forest, int x1, int x2)
{
	vector<int> p1 = findPath(x1, forest);
	vector<int> p2 = findPath(x2, forest);
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

void Graph::contract(vector<int> blossom, vector<int>& M)
{
	int y = blossom[0];
	for (int i = 1; i < blossom.size(); i++)
	{
		int u = blossom[i];
		for (int j = 0; j < edges[u].size(); j++)
		{
			int v = edges[u][j];
			deleteEdge(u, v);
			j--;
			if (find(blossom.begin(), blossom.end(), v) == blossom.end())
			{
				addEdge(y, v);
				if (M[v] == u)
					M[v] = y;
			}
		}
	}
}

void Graph::lift(vector<int> &M, vector<int> blossomVertices, vector< vector<int> > blossomEdges)
{
	int y = blossomVertices[0];
	for (auto x : edges[y])
	{
		deleteEdge(y, x);
	}
	for (int i = 0; i < blossomEdges.size(); i++)
	{
		int u = blossomVertices[i];
		for (auto v : blossomEdges[i])
		{
			addEdge(u, v);
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
	vector<int> M(edges.size(), -1);
	vector<int> forest(edges.size());
	for (int i = 0; i < forest.size(); i++)
		forest[i] = i;
	vector<int> depth(edges.size(), 0);
	vector< vector<int> > blossomsVertices;
	vector< vector < vector<int> > > blossomsEdges;

	while (true)
	{
		vector<bool> isOuter = findOuterInner(forest, depth);
		vector<int> edge = findOuterConnectedOutsideForest(forest, isOuter);
		if (!edge.empty())
		{
			int x = edge[0], y = edge[1];
			for (auto z : edges[y])
			{
				if (M[z] == y)
				{
					forest[y] = x;
					depth[y] = depth[x] + 1;
					forest[z] = y;
					depth[z] = depth[y] + 1;
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
				if (findRoot(x1, forest) != findRoot(x2, forest))
				{
					vector<int> p1 = findPath(x1, forest);
					vector<int> p2 = findPath(x2, forest);
					reverse(p2.begin(), p2.end());
					vector<int> augmentingPath = p1;
					for (auto el : p2)
						augmentingPath.push_back(el);
					M = augment(M, augmentingPath);
					for (int i = 0; i < edges.size(); i++)
					{
						if (M[i] == -1)
							forest[i] = i;
						else if (M[i] > -1)
							forest[i] = -1;
					}
				}
				else
				{
					vector<int> B = findBlossom(forest, x1, x2);
					blossomsVertices.push_back(B);
					vector< vector<int> > blossomEdges;
					for (auto x : B)
					{
						blossomEdges.push_back(edges[x]);
					}
					for (int i = 1; i < B.size(); i++)
					{
						forest[B[i]] = -2;
						M[B[i]] = -2;
					}
					blossomsEdges.push_back(blossomEdges);
					contract(B, M);
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
		lift(M, blossomsVertices[i], blossomsEdges[i]);
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