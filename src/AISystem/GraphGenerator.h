#ifndef __GRAPH_GENERATOR_H__
#define __GRAPH_GENERATOR_H__

#include <string>
#include <fstream>

#include "BFS.h"
#include "Grafo.h"

#include "RenderManager.h"
#include "EntityComponentManager.h"

class GraphGenerator
{
private:
	static GraphGenerator* instance_;
	const char* PATH_ = "Assets/AIFiles/";
	const std::string nodesTag = "IANode";
	Grafo* graph;
	BFS* bfs;
	int size, cheeseVertex;
	std::vector<Ogre::Vector3> vertexPositions;
	std::list<GameObject*> nodes;
	void getVertexVector();
	void readFile(const std::string& filename);
	int nextNode(int& current);

public:
	inline Grafo* getGraph() {
		return graph;
	}
	inline BFS* getBFS() {
		return bfs;
	}

	void nextNodePosition(int& currentNode);
	static GraphGenerator* Instance();
	void init();

	GraphGenerator() {};
	~GraphGenerator() {};
};

#endif	// !__GRAPH_GENERATOR_H__
