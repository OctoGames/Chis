#include "GraphGenerator.h"


GraphGenerator* GraphGenerator::instance_ = nullptr;


GraphGenerator * GraphGenerator::Instance()
{
	if (instance_ == nullptr) instance_ = new GraphGenerator();
	return instance_;
}

GraphGenerator::GraphGenerator() :
	graph_(nullptr),
	graphSolver_(nullptr),
	size_(0),
	cheeseVertex_(-1)
{
}

GraphGenerator::~GraphGenerator()
{
}

void GraphGenerator::init()
{
	getVertexVector();
	readFile("graph.txt");
	graphSolver_ = new GraphSolver(*graph_, cheeseVertex_);
	
}

void GraphGenerator::getVertexVector() 
{
	nodes_ = EntityComponentManager::Instance()->findGameObjectsWithTag(nodesTag_);
	vertexPositions_.resize(nodes_.size());
	
	std::list<GameObject*>::iterator it;
	for( it = nodes_.begin(); it != nodes_.end(); ++it)
	{
		int nameSize = (*it)->getName().size() - 1;
		char s = (*it)->getName()[nameSize];
		int nNode = s - '0';
		//std::cout << "Nodo n: "<< (*it)->getName();
		vertexPositions_[nNode] = (*it)->transform()->getPosition();
	}
}

int GraphGenerator::nextNode(int & current)
{
	int next = current;
	for (int n : graph_->adjacents(current)) 
	{
		if (graphSolver_->distanceTo(n) < graphSolver_->distanceTo(next))
			next = n;
	}

	std::cout << "Next node: " << next;
	current = next;

	return next;
}

void GraphGenerator::nextNodePosition(int & currentNode)
{
	if (currentNode == cheeseVertex_) 
	{
		std::cout << "Reached destination!" << std::endl;
	}
	else
	{
		Ogre::Vector3 v = vertexPositions_[nextNode(currentNode)];
		std::cout << " and its position: " << v << std::endl;
	}
}


void GraphGenerator::readFile(const std::string& filename)
{
	//Abre el fichero y lo lee y crea las aristas del grafo
	std::ifstream file;
	file.open(PATH_ + filename);
	file >> size_ >> cheeseVertex_;
	graph_ = new Graph(size_);
	
	if (file.eof())
	{
		Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Error al cargar el archivo del grafo ++++++++++++++++++++++ \n El archivo " +
			filename + " no existe en la carpeta Assets/AIFiles/ \n");
	}
	else 
	{
		int n1, n2;
		while (!file.eof()) 
		{
			file >> n1 >> n2;
			graph_->addEdge(n1, n2);
			Ogre::LogManager::getSingleton().logMessage("********************** Unidas las aristas " + std::to_string(n1) + ", " + std::to_string(n2) +
				" ************");
		}
	}

	graph_->print();
}


