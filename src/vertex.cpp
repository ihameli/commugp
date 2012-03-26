#include "vertex.h"
#include <algorithm>
#include <iostream>

Vertex::Vertex(int m_number) {
	number = m_number;
	community = NULL;
	strength = 0.0;
	cluster = NULL;
}

Vertex::Vertex(Vertex* v) {
	number = v->number;
	community = 0;
	strength = 0.0;
	cluster = 0;
}

void Vertex::addNeighbour(Vertex* v, double weight) {
	neighbours.push_back(v);
	w.push_back(weight);
	strength+=weight;
}

bool Vertex::hasNeighbour(Vertex* v) {
	std::vector<Vertex*>::iterator nb_it;
	nb_it = find(neighbours.begin(), neighbours.end(), v);
	if (nb_it != neighbours.end())
		return true;
	return false;
}

int Vertex::getDegree() {
	return neighbours.size();
}

double Vertex::getWeight(int i) {
	return w[i];
}

double Vertex::getStrength() {
	return strength;
}

void Vertex::applyPFunction(std::vector<double> pFunction, int lim) {
	p = 0;

	double strength = 0.0;

	for (int i=0;i<neighbours.size();i++) {
		strength += w[i];
	}

	while (pFunction[p] < strength)
		p += 1;

	if (p<lim)
		p = lim;
}	

void Vertex::setCluster(std::list<Vertex*>* m_cluster) {
	cluster = m_cluster;
}

std::list<int> Vertex::listNeighbours() {
	std::list<int> l;
	std::vector<Vertex*>::iterator nb_it;
	for (nb_it = neighbours.begin(); nb_it != neighbours.end(); nb_it++) {
		l.push_back((*nb_it)->getNumber());
	}
	return l;
}

