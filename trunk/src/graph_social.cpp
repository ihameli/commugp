#include "graph_social.h"
#include <iostream>

Graph_Social::Graph_Social(bool m_simple, bool m_weighted, bool m_full_communities) : Graph(m_simple, m_weighted) {
	full_communities = m_full_communities;
}

Graph_Social::~Graph_Social() {
	clearCommunities();
}

void Graph_Social::clearCommunities() {
	std::vector<Community*>::iterator comm_it;
	for (comm_it=v.begin();comm_it!=v.end();comm_it++) {
		delete(*comm_it);
	}
}

//Adds a list of vertices to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertices are copies from the parameter. The vertices are returned.
std::list<Vertex*> Graph_Social::addVertices(std::list<Vertex*>& vertices) {
	std::list<Vertex*> new_vertices;
	Vertex* v;

	std::list<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		v = addVertex(*it);
		new_vertices.push_back(v);
	}
	return new_vertices;
}


//Adds a list of vertices to the graph. Those vertex neighbours that don't belong to the graph, are removed from the list of neighbours. The vertices are copies from the parameter. The vertices are returned.
std::vector<Vertex*> Graph_Social::addVertices(std::vector<Vertex*>& vertices) {
	std::vector<Vertex*> new_vertices;
	Vertex* v;

	std::vector<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		v = addVertex(*it);
		new_vertices.push_back(v);
	}
	return new_vertices;
}


Graph* Graph_Social::extractComponent(ConnectedComponent* comp) {
	Graph_Social* n2 = new Graph_Social(simple, weighted);
//	n2->initialize(maxVertex);
	std::vector<Vertex*>::iterator it = comp->vertices.begin();
	while (it!=comp->vertices.end()) {
		std::vector<Vertex*>::iterator it2 = (*it)->getNeighbours()->begin();
		std::vector<double>::iterator it3 = (*it)->getWeights().begin();
		while (it2!=(*it)->getNeighbours()->end()) {
			//std::cout << "Agregando eje " << (*it)->getNumber() << " " << (*it2)->getNumber() << std::endl;
			n2->addEdge((*it)->getNumber(),(*it2)->getNumber(), (*it3));
			it2++;
			it3++;
		}
		it++;
	}
	return n2;
}

