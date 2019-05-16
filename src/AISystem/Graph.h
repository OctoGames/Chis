#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <vector>
#include <stdexcept>

using Adjs = std::vector<int>;

class Graph
{
public:
	Graph(int v = 0) : V_(v), E_(0), adjacents_(V_) {}

	inline int V() const { return V_; }
	inline int E() const { return E_; }

	void addEdge(int v, int w) 
	{
		if (v >= V_ || w >= V_)	throw std::invalid_argument("Vertex does not exist!");
		
		++E_;
		adjacents_[v].push_back(w);
		adjacents_[w].push_back(v);
	}

	const Adjs& adjacents(int v) const 
	{
		if (v >= V_) throw std::invalid_argument("Vertex does not exist!");
		return adjacents_[v];
	}

	void print(std::ostream & o = std::cout) const 
	{
		o << V_ << " vertices, " << E_ << " edges\n";

		for (auto v = 0; v < V_; ++v) 
		{
			o << v << ": ";

			for (auto w : adjacents_[v]) 
			{
				o << w << " ";
			}

			o << "\n";
		}
	}

private:
	int V_;
	int E_;

	std::vector<Adjs> adjacents_;
};

inline std::ostream & operator<<(std::ostream & o, Graph const& g) 
{
	g.print(o);
	return o;
}


#endif // !__GRAPH_H__
