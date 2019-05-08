#ifndef __BFS_H__
#define __BFS_H__

#include "Graph.h"

class GraphSolver
{
public:
	GraphSolver(Graph const& G, int source);
	
	inline bool distanceTo(int destination) const { return distTo_[destination]; }
	inline bool hasPath(int destination) const { return marked_[destination]; }
	std::vector<int> getPath(int source, int destination);

private:
	void bfs(Graph const& G, int source);

	std::vector<bool> marked_;
	std::vector<int> edgeTo_;
	std::vector<int> distTo_;
};

#endif	// !__BFS_H__