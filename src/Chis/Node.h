#ifndef __NODE_H__
#define __NODE_H__

#include <Engine.h>

class Node : public Component
{
public:
	Node();
	virtual ~Node();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	static std::vector<Node*> nodes_;

private:
	static std::string name_;
	static int numNode_;

	int id_;
};

class NodeFactory : public BaseFactory
{
public:
	NodeFactory() {}
	virtual ~NodeFactory() {}

	virtual Component* create() { return new Node(); }
};

#endif // !__NODE__
