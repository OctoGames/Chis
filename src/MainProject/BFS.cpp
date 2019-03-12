#include "BFS.h"


BFS::BFS(Grafo const& G, int s) : marked(G.V(), false), distTo(G.V()) {
	bfs(G, s);
}

void BFS::bfs(Grafo const& G, int s) {
	std::queue<int> q;
	distTo[s] = 0;
	marked[s] = true;
	q.push(s);
	while (!q.empty()) {
		int v = q.front(); q.pop();
		for (int w : G.ady(v)) {
			if (!marked[w]) {
				distTo[w] = distTo[v] + 1;
				marked[w] = true;
				q.push(w);

			}
		}
	}
};
