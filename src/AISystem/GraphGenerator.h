#ifndef GRAPH_GENERATOR_H_
#define GRAPH_GENERATOR_H_

#include "Grafo.h"
#include "BFS.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <Ogre.h>
#include <SceneLoader.h>
class GraphGenerator
{
private:
	static GraphGenerator* instance_;
	const char* PATH_ = "Assets/AIFiles/";
	Grafo* graph;
	int size, cheeseVertex;
	std::vector<Ogre::Vector3> vertexPositions;
	inline void getVertexVector() {
		vertexPositions = SceneLoader::Instance()->getVertex();
	};

	void readFile(const std::string& filename);
public:
	Grafo* getGraph() {
		return graph;
	}
	BFS* bfs;
	static GraphGenerator* Instance();
	GraphGenerator();
	~GraphGenerator();
};

#endif 
