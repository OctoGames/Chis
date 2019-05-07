#ifndef __BFS_H__
#define __BFS_H__

#include <queue>
#include <vector>

#include "Grafo.h"

class BFS
{
public:
	BFS(Grafo const& G, int s);
	
	inline bool hayCamino(int v) const {
		return marked[v];
	}

	inline int distancia(int v) const {
		return distTo[v];
	}
private:
	void bfs(Grafo const& G, int s);
	std::vector<bool> marked; // marked[v] = ¿hay camino de s a v?
	std::vector<int> distTo;
};

#endif	// !__BFS_H__