#ifndef _GRAPH_SOCIAL_H_
#define _GRAPH_SOCIAL_H_

#include "vertex.h"
#include <map>
#include <list>
#include <vector>
#include <string>
#include "graph.h"

class Vertex;
class SolutionSubmodular;

class Graph_Social : public Graph {

  private:
	std::vector<Community*> v;

	std::list<Community*> communities;

	bool full_communities;

  public:

	Graph_Social(bool m_simple=true, bool m_weighted=false, bool full_communities=true);
	~Graph_Social();

	std::vector<Vertex*> addVertices(std::vector<Vertex*>& vertices);
	std::list<Vertex*> addVertices(std::list<Vertex*>& vertices);

	//Fast access to vertices
	std::map<int, Community*> existent_vertices;

	void clearCommunities();

	std::list<Community*>& getCommunities() { return communities; };

	virtual Graph* extractComponent(ConnectedComponent* comp);

};

#endif

