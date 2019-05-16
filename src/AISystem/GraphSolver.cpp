#include "GraphSolver.h"

#include <queue>
#include <stack>
#include <limits>

GraphSolver::GraphSolver(Graph const& G, int source) : 
	marked_(G.V(), false), 
	edgeTo_(G.V(), -1)
{
	bfs(G, source);
}

void GraphSolver::bfs(Graph const& G, int source) 
{
	std::queue<int> queue;
	queue.push(source);
	marked_[source] = true;

	while (!queue.empty())
	{
		source = queue.front(); queue.pop();
		for (int w : G.adjacents(source))
		{
			if (!marked_[w])
			{
				edgeTo_[w] = source;
				queue.push(w);
				marked_[w] = true;
			}
		}
	}
}

std::list<int> GraphSolver::getPath(int source, int destination)
{
	std::list<int> path;
	
	if (hasPath(source))
	{
		std::stack<int> stack;
		stack.push(source);

		while (stack.top() != destination) stack.push(edgeTo_[stack.top()]);
		while (!stack.empty()) { path.push_front(stack.top()); stack.pop(); }
	}

	return path;
}