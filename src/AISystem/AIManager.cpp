#include "AImanager.h"


AIManager* AIManager::instance_ = nullptr;

AIManager * AIManager::Instance()
{
	if (instance_ == nullptr) instance_ = new AIManager();
	return instance_;
}

AIManager::AIManager()
{
}

AIManager::~AIManager()
{
}

void AIManager::init(const std::string& graphFile)
{
	std::cout << "[AISystem]: Initializing...\n";
	std::cout << "[AISystem]: Creating graph...\n";
	createGraph(graphFile);
	std::cout << "[AISystem]: Solving graph paths...\n";
	solveGraph();
}

void AIManager::update(float deltaTime)
{
}

void AIManager::close()
{
	std::cout << "[AISystem]: System closed.\n";
}

std::list<int> AIManager::getPath(int sourceVertex, int destinationVertex)
{
	if (graphSolver_.hasPath(destinationVertex))
	{
		std::cout << "[AISystem]: Found path!\n";
		return graphSolver_.getPath(sourceVertex, destinationVertex);
	}

	std::cout << "[AISystem]: Could not find path!\n";
	return std::list<int>();
}

void AIManager::createGraph(const std::string & filename)
{
	std::ifstream file;
	file.open(filename);
	int graphNumOfVertex = 0;
	file >> graphNumOfVertex;
	
	graph_ = Graph(graphNumOfVertex);

	if (file.eof()) std::cout << "[AISystem]: Error loading file " << filename << "\n";
	else
	{
		int v = 0;
		int w = 0;

		while (!file.eof())
		{
			file >> v >> w;
			graph_.addEdge(v, w);
			std::cout << "[AISystem]: Linked " << v << " and " << w << " in the graph.\n";
		}
	}

	graph_.print();
}

void AIManager::solveGraph()
{
	graphSolver_ = GraphSolver(graph_, 0);
}
