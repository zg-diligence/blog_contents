/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.4.9-4.10
 */
 
/*
 * Abstract:
 *   1. There are four methods to find the shortest path of single source in total:
 *      Bellman-Ford
 *      SPFA, improvement for Bellman-Ford
 *      ASP, linear time to get the result, for directed asyclic graph
 *      Dijkstra, for graph without negative-weight edge.
 *	 2. There are three methods to find all the shortest paths among vertices in total:
 *	    Floyd-Washall, Johnson, and Dynamic Programming based on matrix multiplication.
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

#define NIL -1
#define infinity 0x3f3f3f3f
typedef  vector<vector<pair<int, int>>> GraphList;
typedef  vector<vector<int>> GraphMatrix;

GraphMatrix nextV;  //used in Floyd-Warshall algorithm.
GraphMatrix npreV;  //used in Johnson alogrithm.
vector<int> indegree;    //indegree of all vertices.
vector<int> dist;		 //shortest distance from source vertex to distination vertex.
vector<int> preV;		 //prefix vertex of vertex in the shortest path.

/* print all the shortest paths from the source vertex. */
void PrintPath(int src = 0)
{
	auto size = dist.size();
	GraphMatrix path(size, vector<int>());
	for (int i = 0; i != size; ++i)
	{
		if (i == src)
		{
			cout << src << "->" << i << ":itself." << endl;
			continue;
		}

		if (dist[i] == infinity)
		{
			cout << src << "->" << i << ":infinity." << endl;
			continue;
		}

		/* collect the shortest path. */
		int cur = i;
		path[i].push_back(i);
		while (preV[cur] != src)
		{
			path[i].push_back(preV[cur]);
			cur = preV[cur];
		}
		path[i].push_back(src);

		/* print the shortest path. */
		cout << src << "->" << path[i].front() << "(" << dist[i] << "):";
		for (int k = path[i].size() - 1; k >= 0; --k)
			cout << path[i][k] << " ";
		cout << endl;
	}
}

/* display the pointed shortest path from u to v in Floyd-Warshall. */
void PrintFloyd(const GraphMatrix &dist, int u, int v)
{
	if (u == v)
		cout << u << "->" << v << ":itself" << endl;
	else if (dist[u][v] == infinity)
		cout << u << "->" << v << ":infinity" << endl;
	else
	{
		cout << u << "->" << v << "(" << dist[u][v] << "):";
		int cur = u;
		while (nextV[cur][v] != v)
		{
			cout << cur << " ";
			cur = nextV[cur][v];
		}
		cout << cur << " " << v << endl;
	}
}

/* display the pointed shortest path from u to v in Johnson. */
void PrintJohnson(const GraphMatrix &dist, int u, int v)
{
	vector<int> path;
	if(u == v)
		cout << u << "->" << v << ":itself." << endl;
	else if (dist[u][v] == infinity)
		cout << u << "->" << v << ":infinity" << endl;
	else
	{
		int cur = v;
		path.push_back(v);
		while (npreV[u][cur] != u)
		{
			path.push_back(npreV[u][cur]);
			cur = npreV[u][cur];
		}
		path.push_back(u);

		cout << u << "->" << v << "(" << dist[u][v] << "):";
		for (int k = path.size() - 1; k >= 0; --k)
			cout << path[k] << " ";
		cout << endl;
	}
}

/*
 * Bellman-Ford algorithm:
 *	 the longest shortest path includes at most |V|-1 edges, so length of the shortest path be updated at most |v|-1 times.
 *	 do |V| times of relaxion operation for each vertex, if there is no change at one process, return true.
 *	 if the |V|th time relaxion has change, there is a negative circuit in the graph, reutrn false.
 * Time complexity: 
 *	 O(|V||E|) -- adjacent list.
 *	 O(|V|^3) -- adjacent matrix.
 */
bool Bellman_Ford(const GraphList &graph, int src = 0)
{
	auto size = graph.size();
	vector<bool> visited(size, false);
	dist[src] = 0;
	for (int i = 1; i <= int(size); ++i)
	{
		bool changed = false;
		for (int u = 0; u != size; ++u)
		{
			/* adjcent matrix */
			//for (int v = 0; v != size; ++v)
			//{
			//	if (dist[u] + graph[u][v] < dist[v])
			//	{
			//		dist[v] = dist[u] + graph[u][v];
			//		preV[v] = u;
			//		changed = true;
			//	}//if
			//}//for

			/* adjacent list */
			for(auto item: graph[u])
			{
				int v = item.first;
				if (dist[u] + item.second < dist[v])
				{
					dist[v] = dist[u] + item.second;
					preV[v] = u;
					changed = true;
				}//if
			}//for
		}//for
		if (!changed)	//there is no update.
			return true;
		if (i == size)  //there is a negative circuit.
			return false;
	}//for
	return false;
}

/*
* SPAF algorithm:
*	 improvement for Bellman-Ford algorithm, only updates the influenced verticies.
*	 using a queue to save the updated vertices, then update their adjacent vertices.
*	 record times of each vertex been enqueued, if time of any vertex is over |V|-1, return false.
* Time complexity:
*	 Not better than Bellman-ForFord in average, but perfroms better when the graph is a single list.
*/
bool SPFA(const GraphList &graph, int src = 0)
{
	auto size = graph.size();
	vector<bool> enqueued(size, false);  //mark whether if the vertex is enqueued.
	vector<int> num(size, 0);	//record times of each vertex being enqueued.
	queue<int> q; q.push(src);
	dist[src] = 0; num[src]++; enqueued[src] = true;
	while (!q.empty())
	{
		int u = q.front();
		q.pop(); enqueued[u] = false;

		/* adjacent matrix */
		//for (int v = 0; v != size; ++v)
		//	if (dist[u] + graph[u][v] < dist[v])
		//	{
		//		dist[v] = dist[u] + graph[u][v];
		//		preV[v] = u;
		//		if (!enqueued[v])
		//		{
		//			q.push(v); enqueued[v] = true;
		//			if (++num[v] == size)
		//				return false;
		//		}//if
		//	}//if

		/* adjacent list */
		for(auto item: graph[u])
		{
			int v = item.first;
			if (dist[u] + item.second < dist[v])
			{
				dist[v] = dist[u] + item.second;
				preV[v] = u;
				if (!enqueued[v])
				{
					q.push(v); enqueued[v] = true;
					if (++num[v] == size)
						return false;
				}//if
			}//if
		}//for
	}//while
	return true;
}

/*
 * Directed acyclic graph:
 *	 do topological-sort first to get the sequence(if u-v exits, u is in front of v).
 *	 do relaxtion operation at order of the topological sequence.
 * Time complexity:
 *	 topological:
 *		O(|V|+|E|)
 *	 relaxion：
 *		O(|E|) -- adjacent list
 *		O(|V|^2) -- adjacent matrix
 *	 in total:
 *		O(|V|+|E|) -- adjacent list
 *		O(|V|^2 + |E|) -- adjacent matrix
 */
bool ASP(const GraphMatrix &graph, int src = 0)
{
	/* do tological-sort to get linear sequence. */
	stack<int> s;
	auto size = graph.size();
	vector<int> sequence; //save topological sequence.
	for (int i = 0; i != size; ++i)
		if (indegree[i] == 0)
			s.push(i);
	while(!s.empty())
	{
		auto u = s.top(); s.pop();
		sequence.push_back(u);
		for (int v = 0; v != size; ++v)
			if (graph[u][v] != infinity &&--indegree[v] == 0)
				s.push(v);
	}

	/* check if there is a circuit. */
	if (sequence.size() < size)
		return false;

	/* do relaxion operation at order of the topological sequence. */
	int i = -1;
	dist[src] = 0;
	while(sequence[++i] != src){}
	for (; i != size - 1; ++i)
		for (int j = i+1; j != size; ++j)
		{
			int u = sequence[i], v = sequence[j];
			if (dist[u] + graph[u][v] < dist[v])
			{
				dist[v] = dist[u] + graph[u][v];
				preV[v] = u;
			}//if
		}//for
	return true;
}

/* percolate down element in the heap. */
void PushDown(vector<int> &heap, vector<int> &position, int p, int size)
{
	int cur = p;
	int vertexPos = heap[p];
	int value = dist[heap[p]];
	while(cur * 2 <= size)
	{
		int next = cur * 2;
		if (next != size && dist[heap[next + 1]] < dist[heap[next]])
			next++;
		if (dist[heap[next]] < value)
		{
			heap[cur] = heap[next];
			position[heap[cur]] = cur;
		}
		else
			break;
		cur = next;
	}
	heap[cur] = vertexPos;
	position[heap[cur]] = cur;
}

/* percolate up element in the heap. */
void PushUp(vector<int> &heap, vector<int> &position, int p)
{
	int cur = p;
	int vertexPos = heap[p];
	int value = dist[heap[p]];
	while(cur > 1)
	{
		int next = cur / 2;
		if (value < dist[heap[next]])
		{
			heap[cur] = heap[next];
			position[heap[cur]] = cur;
		}
		else
			break;
		cur = next;
	}
	heap[cur] = vertexPos;
	position[heap[cur]] = cur;
}

/* 
 * Dijkstra algorithm:
 *   it's a greedy algorithm, only applys to non-negative weighted graph.
 *   first to find the nearest vertex and add it into the set, then update all its adjacent vertices.
 *   using min-heap to get the nearest vertex.
 * Time complexity:
 *   O(|E|*lg|V|) -- adjacent list
 *   O(|V|^2) -- adjacent matrix
 *   Note: update the heap after updating all adjacent vertices at one process when using adjacent matrix.
 */
bool Dijkstra(const GraphList &graph, int src = 0)
{
	/* check whether if there is a negative weight edge. */
	for (const auto &arr : graph)
		for (const auto &item : arr)
			if (item.second < 0)
				return false;

	auto size = graph.size();
	vector<int> heap(2); 
	vector<int> position(size); //save position of vertices in the heap.
	int qsize = size; dist[src] = 0;

	/* init the heap. */
	heap[1] = src; position[src] = 1;
	for (int i = 0; i != size; ++i)
	{
		if (i == src)
			continue;
		heap.push_back(i);
		position[i] = heap.size() - 1;
	}

	while (qsize > 0)
	{
		/* get the nearest vertex. */
		int u = heap[1];
		heap[1] = heap[qsize--];
		position[heap[1]] = 1;
		PushDown(heap, position, 1, qsize);

		/* update adjacent vertices. */
		for(auto item: graph[u])
		{
			int v = item.first;
			if(dist[u] + item.second < dist[v])
			{
				preV[v] = u;
				dist[v] = dist[u] + item.second;
				PushUp(heap, position, position[v]);
			}//if
		}//for
	}//while
	return true;
}

/*
 * Floyd-Warshall algorithm:
 *	 to get the shortest path between any two vertices in the graph.
 *	 dynamic programming:
 *		1.A(0)[i][j]=graph[i][j]
 *		2.A(k)[i][j]=min{ A(k-1)[i][j], A(k-1)[i][k]+A(k-1)[k][j] }
 *	 the function return a matrix including distance between all vertices.
 *	 if there is a nagative-weight circuit, return empty matrix.
 * Time complexity:
 *	 O(|V|^3)
 */
GraphMatrix Floyd_Warshall(const GraphMatrix &graph)
{
	int size = graph.size();
	vector<vector<int>> dist = graph;
	nextV.resize(size, vector<int>(size, NIL));
	for (int u = 0; u != size; ++u)
		for (int v = 0; v != size; ++v)
			if (graph[u][v] != infinity)
				nextV[u][v] = v;

	/* dynamic programming. */
	for (int k = 0; k != size; ++k)
		for (int i = 0; i != size; ++i)
			for (int j = 0; j != size; ++j)
				if (dist[i][k] + dist[k][j] < dist[i][j])
				{
					if (dist[i][k] == infinity || dist[k][j] == infinity) //negative weight edge.
						continue;
					dist[i][j] = dist[i][k] + dist[k][j];
					nextV[i][j] = nextV[i][k];
					if (i == j && dist[i][j] < 0)
						return vector<vector<int>>();  //there is a negative weighted circuit.
				}
	return dist;
}

/*
 * Johnson algorithm:
 *	 this algorithm applys to non-negative weighted circuit graph.
 * Algorithm process:
 *	 1.call Bellman-Ford algorithm to check whether if there is a negative-weight circuit.
 *	 2.add one vertex into the graph, call Bellman-Ford algorithm to compute value array h[v].
 *	 3.update weight of all edges to get non-negative weighted graph.
 *	 4.call Dijkstra algorithm for each vertex to get all the shortest paths.
 *	 5.return a matrix including distance between all vertices, else return empty matrix.
 * Time complexity:
 *   O(|V|*|E|*lg|V|) -- using min-heap to implement Dijkstra algorithm.
 *   O(|V|^2 * lg|V|) -- using fibonacci heap to implemant Dijkstra algorithm.
 */
GraphMatrix Johnson(const GraphList &graph)
{ 
	/* check whether if there is a nagative weighted circuit. */
	if (!SPFA(graph, 0))
		return GraphMatrix();

	/* get augmentment matrix. */
	int size = graph.size();
	GraphList devGraph = graph;
	devGraph.push_back(vector<pair<int, int>>());
	for (int i = 0; i != size; ++i)
		devGraph[size].push_back(make_pair(i, 0));

	/* update weight of all edges. */
	preV = vector<int>(size + 1, NIL);
	dist = vector<int>(size + 1, infinity);
	SPFA(devGraph, size);
	vector<int> h = dist;
	for (int u = 0; u != size; ++u)
	{
		auto &arr = devGraph[u];
		for (auto &item : arr)
			item.second += (h[u] - h[item.first]);
	}

	/* call Dijkstra for each vertex. */
	GraphMatrix distMatrix(size, vector<int>(size,infinity));
	for (int u = 0; u != size; ++u)
	{
		preV = vector<int>(size + 1, NIL);
		dist = vector<int>(size + 1, infinity);
		Dijkstra(devGraph, u);
		npreV.push_back(preV);
		for (int v = 0; v != size; ++v)
			if(dist[v] != infinity)
				distMatrix[u][v] = dist[v] + h[v] - h[u];
	}
	return distMatrix;
}

/*
 * Dynamic Programming based on matrix multiplication:
 *   this algorithm applys to non-negative weighted graph.
 *	 Lm[i][j] = min{ Lm-1[i][j], min{Lm-1[i][k] + weight[k][j]} }, k = 0,1,2,3,...,|V|-1
 *	 The first function seems like matrix multiplication, extend a edge based on the newest shortest paths.
 *	 The second function do n-2 times of 'matrix multiplication' operation or using repeat square solution.
 * Time complexity:
 *   O(|V|^4) -- ordinary matrix multiplication.
 *   O(|V|^3 * lg|V|) -- repeat square method.
 */
GraphMatrix ExtendShortestPaths(const GraphMatrix& L, const GraphMatrix &W)
{
	int size = L.size();
	GraphMatrix LL(size, vector<int>(size, infinity));
	for (int i = 0; i != size; ++i)
		for (int j = 0; j != size; ++j)
			for (int k = 0; k != size; ++k)
				if (L[i][k] + W[k][j] < LL[i][j])
				{
					if (L[i][k] == infinity || W[k][j] == infinity) //negative weight edge.
						continue;
					LL[i][j] = L[i][k] + W[k][j];
					//nextV[i][j] = nextV[i][k];
					if (i == j && LL[i][j] < 0)
						return vector<vector<int>>();  //there is a negative weighted circuit.
				}
	return LL;
}

GraphMatrix MatrixMultiply(GraphMatrix &W)
{
	int size = W.size();
	//nextV = GraphMatrix(size, vector<int>(size, NIL));
	//for (int u = 0; u != size; ++u)
	//	for (int v = 0; v != size; ++v)
	//		if (W[u][v] != infinity)
	//			nextV[u][v] = v;

	GraphMatrix L = W;

	/* 1. extend edge one by one. */
	//for(int m = 2; m != size && !L.empty(); ++m)
	//	L = ExtendShortestPaths(L, W);

	/* 2. repeat square method. */
	for (int m = 1; m < size && !L.empty(); m *= 2)
		L = ExtendShortestPaths(L, L); //repeat square method.
	return L;
}

int main()
{
	int num, u, v, w;
	cout << "请输入顶点数:";
	cin >> num;
	GraphMatrix graph(num, vector<int>(num, infinity));	//adjacent matrix.
	GraphList	graph1(num); //adjacent list.
	dist.resize(num, infinity);
	preV.resize(num, NIL);
	indegree.resize(num, 0);

	cout << "请依次输入有向图的边,以0 0 0结束:" << endl;
	while (cin >> u >> v >> w && !(u == 0 && v == 0))
	{
		graph[u][v] = w;
		graph1[u].push_back(make_pair(v, w));
		indegree[v]++;
	}
	for (int i = 0; i != num; ++i)
		graph[i][i] = 0;

	cout << "输入源点:";
	int src; cin >> src;

	/*------------------------------------------------------------*/

	cout << "\n1.Bellman-Ford algorithm:" << endl;
	if (Bellman_Ford(graph1, src))
		PrintPath(src);
	else
		cout << "There is a negative-weight circuit." << endl;

	/*------------------------------------------------------------*/

	cout << "\n2.SPAF algorithm:" << endl;
	preV = vector<int>(num, NIL);
	dist = vector<int>(num, infinity);
	if (SPFA(graph1, src))
		PrintPath(src);
	else
		cout << "There is a negative-weight circuit." << endl;

	/*------------------------------------------------------------*/

	cout << "\n3.ASP algorithm:" << endl;
	preV = vector<int>(num, NIL);
	dist = vector<int>(num, infinity);
	if (ASP(graph, src))
		PrintPath(src);
	else
		cout << "There is a circuit." << endl;

	/*------------------------------------------------------------*/

	cout << "\n4.Dijkstra algorithm:" << endl;
	preV = vector<int>(num, NIL);
	dist = vector<int>(num, infinity);
	if (Dijkstra(graph1, src))
		PrintPath(src);
	else
		cout << "There is a negative-weight edge." << endl;

	/*------------------------------------------------------------*/

	cout << "\n5.Floyd-Warshall algorithm:" << endl;
	auto dist1 = Floyd_Warshall(graph);
	if (dist1.empty())
		cout << "There is a negative-weight circuit." << endl;
	else
	{
		for (int m = 0; m != num; ++m)
		{
			for (int n = 0; n != num; ++n)
				if (dist1[m][n] == infinity)
					cout << "N\t";
				else
					cout << dist1[m][n] << "\t";
			cout << endl;
		}

		cout << "请输入你想查看的路径以0 0结束:" << endl;
		int _src, dest;
		while (cin >> _src >> dest && (_src != 0 || dest != 0))
			PrintFloyd(dist1, _src, dest);
	}

	/*------------------------------------------------------------*/

	cout << "\n6.Johnson algorithm:" << endl;
	preV = vector<int>(num, NIL);
	dist = vector<int>(num, infinity);
	auto dist2 = Johnson(graph1);
	if (dist2.empty())
		cout << "There is a negative-weight circuit." << endl;
	else
	{
		for (int m = 0; m != num; ++m)
		{
			for (int n = 0; n != num; ++n)
				if (dist2[m][n] == infinity)
					cout << "N\t";
				else
					cout << dist2[m][n] << "\t";
			cout << endl;
		}

		cout << "请输入你想查看的路径以0 0结束:" << endl;
		int _src, dest;
		while (cin >> _src >> dest && (_src != 0 || dest != 0))
			PrintJohnson(dist2, _src, dest);
	}

	/*------------------------------------------------------------*/

	cout << "\n7.Matrix Multiply:" << endl;
	auto dist3 = MatrixMultiply(graph);
	if (dist3.empty())
		cout << "There is a negative-weight circuit." << endl;
	else
	{
		for (int m = 0; m != num; ++m)
		{
			for (int n = 0; n != num; ++n)
				if (dist3[m][n] == infinity)
					cout << "N\t";
				else
					cout << dist3[m][n] << "\t";
			cout << endl;
		}

		//cout << "请输入你想查看的路径以0 0结束:" << endl;
		//int _src, dest;
		//while (cin >> _src >> dest && (_src != 0 || dest != 0))
		//	PrintFloyd(dist3, _src, dest);
	}
	return 0;
}

/*data
positive weight
positive circuit
7
0 1 2
0 3 1
1 3 2
1 4 10
2 0 4
2 5 5 
3 2 2
3 4 2
3 5 8
3 6 4
4 6 6
6 5 1
0 0 0

negative circuit
4
0 1 1
0 2 1
1 3 3
1 2 -2
2 1 1
2 3 2
0 0 0

negative-weight
positive circuit
5
0 1 3
0 2 8
0 4 -4
1 4 7
1 3 1
2 1 4
3 0 2
3 2 -5
4 3 6
0 0 0

positive weight
no circuit
10
0 1 3
0 2 2
1 4 3
1 3 2
2 3 2
2 5 1
4 6 3
3 6 3
3 7 2
5 7 2
5 9 4
6 8 1
7 8 1
9 8 1
0 0 0
*/