#ifndef __AI_MANAGER_H__
#define __AI_MANAGER_H__

#include <string>
#include <fstream>

#include "Graph.h"
#include "GraphSolver.h"

class AIManager
{
public:
	static AIManager* Instance();

	void init(const std::string& graphFile);
	void update(float deltaTime);
	void close();

	inline int getNumNodes() const { return graph_.V(); }
	std::list<int> getPath(int sourceVertex, int destinationVertex);

private:
	AIManager();
	~AIManager();

	void createGraph(const std::string& filename);
	void solveGraph();

	static AIManager* instance_;

	Graph graph_;
	GraphSolver graphSolver_;
};

#endif	// !__AI_MANAGER_H__
