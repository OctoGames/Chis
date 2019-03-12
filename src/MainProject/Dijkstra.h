#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "GrafoDirigidoValorado.h"
#include "IndexPQ.h"
#include <vector>


class Dijkstra {
public:
	Dijkstra(GrafoDirigidoValorado <int> & G);

	inline int minDist() { return distTo[distTo.size() - 1]; }
	inline int hayCamino() { return distTo[distTo.size() - 1] != std::numeric_limits<int> ::max(); }
private:
	void relax(AristaDirigida<int> a);
	std::vector <int> distTo;
	IndexPQ<int> pQueue;

};
#endif