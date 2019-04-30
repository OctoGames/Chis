#include "GraphGenerator.h"

GraphGenerator* GraphGenerator::instance_ = nullptr;


void GraphGenerator::init()
{
	getVertexVector();
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

void GraphGenerator::getVertexVector() {
	nodes = EntityComponentManager::Instance()->findGameObjectsWithTag(nodesTag);
	vertexPositions.resize(nodes.size());
	std::list<GameObject*>::iterator it;
	for( it = nodes.begin(); it != nodes.end(); ++it){
		int nameSize = (*it)->getName().size() - 1;
		char s = (*it)->getName()[nameSize];
		int nNode = s - '0';
		//std::cout << "Nodo n: "<< (*it)->getName();
		vertexPositions[nNode] = (*it)->transform()->getPosition();
	}
}

int GraphGenerator::nextNode(int & current)
{
	int next = current;
	for (int n : graph->ady(current)) {
		if (bfs->distancia(n) < bfs->distancia(next))
			next = n;
	}
	std::cout << "Siguiente nodo: " << next;
	current = next;
	return next;
}

void GraphGenerator::nextNodePosition(int & currentNode)
{
	if (currentNode == cheeseVertex) {
		std::cout << "Ha llegado a su destino" << std::endl;
	}
	else
	{
		Ogre::Vector3 v = vertexPositions[nextNode(currentNode)];
		std::cout << " y su posicion: " << v << std::endl;
	}
}


void GraphGenerator::readFile(const std::string& filename)
{//Abre el fichero y lo lee y crea las aristas del grafo
	std::ifstream file;
	file.open(PATH_ + filename);
	file >> size >> cheeseVertex;
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


