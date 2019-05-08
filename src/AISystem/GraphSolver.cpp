#include "GraphSolver.h"

#include <queue>
#include <stack>
#include <limits>

GraphSolver::GraphSolver(Graph const& G, int source) : 
	marked_(G.V(), false), 
	distTo_(G.V(), -1),
	edgeTo_(G.V(), -1)
{
	bfs(G, source);
}

void GraphSolver::bfs(Graph const& G, int source) 
{
	std::queue<int> queue;
	queue.push(source);
	marked_[source] = true;
	distTo_[source] = 0;

	while (!queue.empty())
	{
		source = queue.front(); queue.pop();
		for (int w : G.adjacents(source))
		{
			if (!marked_[w])
			{
				distTo_[w] = distTo_[source] + 1;
				edgeTo_[w] = source;
				queue.push(w);
				marked_[w] = true;
			}
		}
	}
}

std::vector<int> GraphSolver::getPath(int source, int destination)
{
	if (!hasPath(destination)) return std::vector<int>();

	std::vector<int> path;
	std::stack<int> stack;
	stack.push(destination);

	while (stack.top() != source) stack.push(edgeTo_[stack.top()]);
	while (!stack.empty()) { path.push_back(stack.top()); stack.pop(); }

	return path;
}