#include "Node.h"


std::string Node::name_ = "Node";
int Node::numNode_ = 0;
std::vector<Node*> Node::nodes_ = std::vector<Node*>();

Node::Node() :
	id_(0)
{
}

Node::~Node()
{
}

void Node::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_n"); if (it != params.end()) enabled_ = params.at("enabled_n").b;
}

Component * Node::clone()
{
	Node* clonedComponent = new Node();

	clonedComponent->enabled_ = this->enabled_;

	numNode_++;
	clonedComponent->id_ = numNode_;
	nodes_.push_back(clonedComponent);

	return clonedComponent;
}

void Node::init()
{
	setEnabled(enabled_);
}