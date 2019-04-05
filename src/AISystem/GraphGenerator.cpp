#include "GraphGenerator.h"

GraphGenerator* GraphGenerator::instance_ = nullptr;


void GraphGenerator::init()
{
	//getVertexVector();
	readFile("graph.txt");
	bfs = new BFS(*graph, cheeseVertex);
	
}
GraphGenerator * GraphGenerator::Instance()
{
	if (instance_ == nullptr) {
		instance_ = new GraphGenerator();
	}
	return instance_;
}

void GraphGenerator::readFile(const std::string& filename)
{//Abre el fichero y lo lee y crea las aristas del grafo
	std::ifstream file;
	file.open(PATH_ + filename);
	file >> size >>cheeseVertex;
	graph = new Grafo(size);
	if (file.eof())
		Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Error al cargar el archivo del grafo ++++++++++++++++++++++ \n El archivo " + 
													filename + " no existe en la carpeta Assets/AIFiles/ \n");
	else {
		int n1, n2;


		while (!file.eof()) {
			file >> n1 >> n2;
			graph->ponArista(n1, n2);
			Ogre::LogManager::getSingleton().logMessage("********************** Unidas las aristas " + std::to_string(n1) + ", " + std::to_string(n2)
														+ " ************");
		}
	}
	graph->print();
}
