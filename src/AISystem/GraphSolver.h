#ifndef __GRAPH_SOLVER_H__
#define __GRAPH_SOLVER_H__

#include <list>

#include "Graph.h"

class GraphSolver
{
public:
	GraphSolver() {}
	GraphSolver(Graph const& G, int source);

	inline bool hasPath(int destination) const { return marked_[destination]; }
	std::list<int> getPath(int source, int destination);

private:
	void bfs(Graph const& G, int source);

	std::vector<bool> marked_;
	std::vector<int> edgeTo_;
};

#endif	// !__GRAPH_SOLVER_H__