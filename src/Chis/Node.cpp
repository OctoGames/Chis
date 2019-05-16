#include "Node.h"


std::vector<Ogre::SceneNode*> Node::nodes_ = std::vector<Ogre::SceneNode*>();
std::string Node::name_ = "Node";

Node::Node() :
	id_(-1)
{
}

Node::~Node()
{
}

void Node::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_n"); if (it != params.end()) enabled_ = params.at("enabled_n").b;
	it = params.find("id"); if (it != params.end()) id_ = static_cast<int>(params.at("id").f);
}

Component * Node::clone()
{
	Node* clonedComponent = new Node();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void Node::init()
{
	nodes_[id_] = gameObject()->transform();
	setEnabled(enabled_);
}