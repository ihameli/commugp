#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include <list>
#include "vertex.h"

class Community;

class Solution {

  public:
	bool virtual proposeNeighbour() = 0;
	void virtual acceptNeighbour() = 0;
	void virtual rejectNeighbour() = 0;

	double virtual evaluateNeighbour() = 0;
	double virtual evaluate() = 0;

	void findCommunities(double t_final = 1, bool logCommunities = false);
	void findNaturalCommunity(Vertex* v, bool logCommunities = false);
	std::list<Community*> virtual getCommunities() = 0;

};

#endif

