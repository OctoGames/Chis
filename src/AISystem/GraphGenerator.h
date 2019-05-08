#ifndef __GRAPH_GENERATOR_H__
#define __GRAPH_GENERATOR_H__

#include <string>
#include <fstream>

#include "Graph.h"
#include "GraphSolver.h"

#include "RenderManager.h"
#include "EntityComponentManager.h"

class GraphGenerator
{
public:
	GraphGenerator();
	~GraphGenerator();

	static GraphGenerator* Instance();

	inline Graph* getGraph() { return graph_; }
	inline GraphSolver* getGraphSolver() { return graphSolver_; }

	void init();
	void nextNodePosition(int& currentNode);

private:
	static GraphGenerator* instance_;

	const char* PATH_ = "Assets/AIFiles/";
	const std::string nodesTag_ = "IANode";

	Graph* graph_;
	GraphSolver* graphSolver_;

	int size_;
	int cheeseVertex_;

	std::list<GameObject*> nodes_;
	std::vector<Ogre::Vector3> vertexPositions_;

	void readFile(const std::string& filename);
	int nextNode(int& current);
	void getVertexVector();
};

#endif	// !__GRAPH_GENERATOR_H__
