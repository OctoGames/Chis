#ifndef GRAPH_GENERATOR_H_
#define GRAPH_GENERATOR_H_

#include "Grafo.h"
#include "BFS.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <Ogre.h>
//#include <SceneLoader.h>
class GraphGenerator
{
private:
	static GraphGenerator* instance_;
	const char* PATH_ = "Assets/AIFiles/";
	Grafo* graph;
	BFS* bfs;
	int size, cheeseVertex;
	std::vector<Ogre::Vector3> vertexPositions;
	inline void getVertexVector() {
		//vertexPositions = SceneLoader::Instance()->getVertex();
	};

	void readFile(const std::string& filename);
public:
	inline Grafo* getGraph() {
		return graph;
	}
	inline BFS* getBFS() {
		return bfs;
	}
	static GraphGenerator* Instance();
	void init();
	GraphGenerator() {};
	~GraphGenerator() {};
};

#endif 
