#include "Dijkstra.h"

Dijkstra::Dijkstra(GrafoDirigidoValorado <int> & G) : distTo(G.V(), std::numeric_limits<int>::max()), pQueue(G.V())
{
	distTo[0] = 0.0;
	pQueue.push(0, 0.0);
	while (!pQueue.empty())
	{
		int v = pQueue.top().elem;
		pQueue.top();
		for (AristaDirigida<int> a : G.ady(v))
			relax(a);
	}
}

void Dijkstra::relax(AristaDirigida<int> a) {
	int origen = a.from(), destino = a.to();
	if (distTo[destino] > distTo[origen] + a.valor())
	{
		distTo[destino] = distTo[origen] + a.valor();
		pQueue.update(destino, distTo[destino]);
	}
}