#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <list>
#include <vector>
#include <map>
#include "community.h"
#include "types.h"
#include <sstream>

class Component;
class Community;

class Vertex {
  private:
	int number;

	Community* community;
	std::list<Vertex*>* cluster;
	std::vector<Vertex*> neighbours;
	std::list<int> edgeDenses; //For k-denses
	std::vector<double> w;
	double strength;
	int p;
	void applyPFunction(std::vector<double> pFunction, int i);

  public:
	Vertex(int m_number);
	Vertex(Vertex* v);
	void addNeighbour(Vertex* v, double weight = 1.0);
	bool hasNeighbour(Vertex* v);
	inline std::vector<Vertex*>* getNeighbours() { return &neighbours; }
	inline std::list<int>* getEdgeDenses() { return &edgeDenses; }
	inline void cleanEdgeDenses() { edgeDenses.clear(); }
	inline std::vector<double> &getWeights() { return w; }
	inline int getNumber() { return number; }
	int getDegree();
	double getWeight(int i);
	double getStrength();
	int getP(std::vector<double> pFunction, int lim = -1) {
		applyPFunction(pFunction, lim);
		return p;
	}
	void setCluster(std::list<Vertex*>* m_cluster);
	inline std::list<Vertex*>* getCluster() {return cluster;}

	inline void setCommunity(Community* m_community) { community = m_community; }
	inline Community* getCommunity() {return community;}

	std::list<int> listNeighbours();

	bool mark;

};

#endif

