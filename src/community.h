#ifndef _COMMUNITY_H_
#define _COMMUNITY_H_

#include <list>
#include <vector>
#include "types.h"
#include "ordered_list.h"
#include <set>
#include "vertex.h"

class Vertex;

class Community {
  private:
	static int next_available_number;
	int number;

	int id; //To follow a community through time
	int name; //Original name, in case the communities were loaded from a file
	int order;

	std::list<Vertex*> vertices;
	double sumStrengths;
	double sumEdges;
	unsigned int size;

	void addCommunityCut(Community* c, double n = 1);
	void removeCommunityCut(Community* c, double n = 1);

	//For graphics
	double rhoCoordinate;
	double phiCoordinate;
	double xCoordinate;
	double yCoordinate;
	double ratioAbstract;
	double ratioReal;

  public:
	Community();
	~Community();

	void clear();
	std::vector<Community*> community_cuts1; //Neighbour community
	std::vector<double> community_cuts2; //Cut with the neighbour community
	std::vector<unsigned int> community_cuts3; //MY POSITION IN THE NEIGHBOUR COMMUNITY'S VECTOR OF NEIGHBOUR COMMUNITIES
	std::set<bestNeighbour*, Compare> community_cuts;

	inline int getNumber() { return number; }
	bool removed;

	inline int getId() { return id; }
	inline void setId(int i) { id = i; }
	inline int getName() { return name; }
	inline void setName(int n) { name = n; }
	inline int getOrder() { return order; }
	inline void setOrder(int o) { order = o; }

	inline double getSize() const { return size; }

	void addVertex(Vertex* v);
	void delVertex(Vertex* v);

	void addCommunity(Community* C, double prevcut);
	void renew();
	void removeCommunity();
	//void removeVertex(Vertex* v);

	bool hasNeighbour(Community* c);

	inline std::list<Vertex*>& getVertices() { return vertices; }
	std::list<int> listVertices();
	inline double tStrengths() { return sumStrengths; }

	//Return number of edges, counted by two (equals the internal community degree)
	double tEdges();

	//Recomputes parameters, probably because vertices have changed
	void recompute();

/*
	inline int cut(Community* c) {
		std::map<Community*, int>::iterator it = community_cuts.find(c);
		if (community_cuts.find(c)!=community_cuts.end())
			return it->second;
		return 0; };
*/

	inline std::set<bestNeighbour*, Compare>& getCommunityCuts() {return community_cuts; };

	double getCommunityCut(Community* c);

	void printCommunity();

	void computeBestNeighbour();

	//For graphics
	inline double getRhoCoordinate() { return rhoCoordinate;}
	inline double getPhiCoordinate() { return phiCoordinate;}
	inline double getX() { return xCoordinate;}
	inline double getY() { return yCoordinate;}
	inline double getRatioAbstract() { return ratioAbstract;}
	inline double getRatioReal() { return ratioReal;}
	inline void setRhoCoordinate(double rho) { rhoCoordinate = rho;}
	inline void setPhiCoordinate(double phi) { phiCoordinate = phi;}
	inline double setX(double x) { xCoordinate = x;}
	inline double setY(double y) { yCoordinate = y;}
	inline void setRatioAbstract(double r) { ratioAbstract = r;}
	inline void setRatioReal(double r) { ratioReal = r;}

	inline std::vector<Community*>& getNeighbourCommunities() { return community_cuts1; }
	inline std::vector<double>& getNeighbourCommunityCuts() { return community_cuts2; }
	//For use by solution_suborthogonal
	int selected;

	bestNeighbour* bn;

	//For use by pair_communities
	bool aux;

	int mark;
};

#endif

