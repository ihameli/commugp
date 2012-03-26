#include "community.h"
#include <algorithm>
#include <iostream>

//Return number of edges, counted by two (equals the internal community degree)
double Community::tEdges() {
	std::vector<Community*>::iterator it;
	it = std::find(community_cuts1.begin(), community_cuts1.end(), this);
	if (it != community_cuts1.end()) {
		return community_cuts2[it-community_cuts1.begin()];
	}
	else {
		return 0;
	}
}

void Community::addCommunityCut(Community* c, double n) {
	std::vector<Community*>::iterator it;
	it = std::find(community_cuts1.begin(), community_cuts1.end(), c);
	if (it != community_cuts1.end()) {
		community_cuts2[it-community_cuts1.begin()] += n;
	}
	else {
		community_cuts1.push_back(c);
		community_cuts2.push_back(n);
	}
}

void Community::removeCommunityCut(Community* c, double n) {
	std::vector<Community*>::iterator it;
	std::vector<double>::iterator it2;
	it = std::find(community_cuts1.begin(), community_cuts1.end(), c);

	if (it==community_cuts1.end())
		std::cout << "ERROR" << std::endl;

	int dist = it - community_cuts1.begin();
	it2 = community_cuts2.begin() + dist;

	if (n==-1) //Remove all
		n = *it2;
//	if (c == this)
//		std::cout << "SOS! son iguales, corte es " << *it2 << " borro " << n << std::endl;
//	else
//		std::cout << "SOS! son distintas " << std::endl;

	if (*it2 > n)
		*it2 -= n;
	else {
		community_cuts2.erase(it2);
		community_cuts1.erase(it);
	}
}


int Community::next_available_number = 1;

//PUBLIC

Community::Community() {
	sumStrengths = 0.0;
	removed = false;

	size = 0;

	bn = NULL;

	id = 0;
	name = 0;

	//Setting this community's number
	number = next_available_number;
	next_available_number++;

	aux = false;
}

Community::~Community() {
	if (bn!=NULL)
		delete(bn);
}

/*
void Community::recompute() {
	std::vector<Vertex*>::iterator it;
	sumStrengths = 0;
	for (it=vertices.begin();it!=vertices.end();it++) {
		sumStrengths += (*it)->tStrength();
	}	
}
*/

void Community::renew() {
	number = next_available_number;
	next_available_number++;
}


double Community::getCommunityCut(Community* c) {
	std::vector<Community*>::iterator it;
	std::vector<double>::iterator it2;
	it = std::find(community_cuts1.begin(), community_cuts1.end(), c);

	if (it==community_cuts1.end()) {
		//std::cout << "ERROR" << std::endl;
		return 0.0;
	}

	int dist = it - community_cuts1.begin();
	it2 = community_cuts2.begin() + dist;

	return *it2;
}

bool Community::hasNeighbour(Community* c) {
	std::vector<Community*>::iterator nb_it;
	nb_it = std::find(community_cuts1.begin(), community_cuts1.end(), c);
	if (nb_it != community_cuts1.end())
		return true;
	return false;
}

void Community::addVertex(Vertex* v) {
	size += 1;
	sumStrengths += v->getDegree();
	vertices.push_back(v);

	std::vector<Vertex*>* ngb = v->getNeighbours();
	std::vector<double> w = v->getWeights();
	std::vector<Vertex*>::iterator ngb_it;
	std::vector<double>::iterator w_it;
	w_it = w.begin();
	for (ngb_it=ngb->begin();ngb_it!=ngb->end();ngb_it++) {
		if ((*ngb_it)->getCommunity()!=NULL) {
			addCommunityCut((*ngb_it)->getCommunity(), *w_it);
			(*ngb_it)->getCommunity()->addCommunityCut(this, *w_it);
		}
		w_it++;
	}
}

void Community::delVertex(Vertex* v) {
	size -= 1;
	sumStrengths -= v->getDegree();
	vertices.remove(v);

	std::vector<Vertex*>* ngb = v->getNeighbours();
	std::vector<double> w = v->getWeights();
	std::vector<Vertex*>::iterator ngb_it;
	std::vector<double>::iterator w_it;
	w_it = w.begin();
	for (ngb_it=ngb->begin();ngb_it!=ngb->end();ngb_it++) {
//		std::cout << "Vecino " << (*ngb_it)->getNumber() << std::endl;
		if ((*ngb_it)->getCommunity()!=NULL) {
//			std::cout << "Peso " << *w_it << std::endl;
			removeCommunityCut((*ngb_it)->getCommunity(), *w_it);
			(*ngb_it)->getCommunity()->removeCommunityCut(this, *w_it);
		}
		w_it++;
	}
}

std::list<int> Community::listVertices() {
	std::list<int> res;
	std::list<Vertex*>::iterator it;
	for (it=vertices.begin();it!=vertices.end();it++) {
		res.push_back((*it)->getNumber());
	}
	return res;
}

void Community::addCommunity(Community* C, double prevcut) { 
	sumStrengths += C->tStrengths();
	size += C->getSize();

	//THESE TWO LINES ARE NOT NECESSARY
	//vertices.reserve(vertices.size() + C->vertices.size());
	vertices.insert(vertices.end(), C->vertices.begin(), C->vertices.end());
	//Neighbours and their communities get their map of community cuts updated
	std::vector<Community*>::iterator it_nbcomm;
	std::vector<double>::iterator it2;

	for (it_nbcomm=C->community_cuts1.begin();it_nbcomm!=C->community_cuts1.end();it_nbcomm++) {
		int dist = it_nbcomm - C->community_cuts1.begin();
		it2 = C->community_cuts2.begin() + dist;

		if ((*it_nbcomm)!=C) {
			(*it_nbcomm)->addCommunityCut(this, (*it2));
			this->addCommunityCut((*it_nbcomm), (*it2));
		}
		else
			this->addCommunityCut(this, (*it2));
	}

//	if (t<8)
//		std::cout << "Size del vector de comunidades vecinas: " << community_cuts1.size() << ", " << community_cuts2.size() << std::endl;
/*	std::cout << "SOY" << std::endl;
	printCommunity();
	for (it_nbcomm=getCommunityCuts().begin();it_nbcomm!=getCommunityCuts().end();it_nbcomm++) {
		(*it_nbcomm).first->printCommunity();
		std::cout << (*it_nbcomm).second << std::endl;
	}
	std::cout << "*********************" << std::endl;
*/
}

void Community::clear() {
	sumStrengths = 0;
	size = 0;
	bn = NULL;
	removed = false;
	community_cuts1.clear();
	community_cuts2.clear();
	community_cuts3.clear();
	community_cuts.clear();
	vertices.clear();
}

void Community::removeCommunity() { 
	sumStrengths = 0;
	size = 0;

	//Neighbours and their communities get their map of community cuts updated
	std::vector<Community*>::iterator it_nbcomm;
	std::vector<double>::iterator it2;
	for (it_nbcomm=community_cuts1.begin();it_nbcomm!=community_cuts1.end();it_nbcomm++) {
		int dist = it_nbcomm - community_cuts1.begin();
		it2 = community_cuts2.begin() + dist;
		if ((*it_nbcomm)!=this)
			(*it_nbcomm)->removeCommunityCut(this, (*it2));
//		if ((*it_nbcomm)!=this)
//			removeCommunityCut((*it_nbcomm), (*it2));
	}
	community_cuts1.clear();
	community_cuts2.clear();
	vertices.clear();
}

void Community::printCommunity() {
	std::list<Vertex*>::iterator vertex_it;
	std::cout << "C: ";
	for (vertex_it=vertices.begin();vertex_it!=vertices.end();vertex_it++) {
		std::cout << (*vertex_it)->getNumber() << " ";
	}
	std::cout << std::endl;
}

void Community::computeBestNeighbour() {
	double tmax_num = 0;
	double tmax_den = 1;

	Community* CNewBestNb;
	std::vector<Community*>::iterator comm_it2;
	std::vector<double>::iterator comm_it3;
	double t_num, t_den;

	comm_it2 = community_cuts1.begin();
	comm_it3 = community_cuts2.begin();
	double deg = (double)tStrengths();
	while (comm_it2!=community_cuts1.end()) {
		if ((*comm_it2)->getNumber() < getNumber()) {
			t_num = (double)(*comm_it3);
			t_den = deg * (double)(*comm_it2)->tStrengths();
			if ((t_num*tmax_den > t_den*tmax_num)) {
				tmax_num = t_num;
				tmax_den = t_den;
				CNewBestNb = *comm_it2;
			}
		}
		comm_it2++;
		comm_it3++;
	}
	if (tmax_num!=0) {
		if (bn==NULL) {
			bn = new bestNeighbour();
		}
		bn->C1 = this;
		bn->C2 = CNewBestNb;
		bn->num = tmax_num;
		bn->den = tmax_den;
		return;
	}
	if (bn!=NULL) {
		delete(bn);
		bn=NULL;
	}

}

