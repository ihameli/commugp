#ifndef _ORDERED_LIST_H_
#define _ORDERED_LIST_H_

#include "log.h"
#include "types.h"

class Node {
  public:
	bestNeighbourPtr bnPtr;
	Node* prev;
	Node* next;
};

//A struct for accesing the best neighbour of a community in terms of submodularity
class OrderedList {
  private:
	Node *first, *last, *actual;
	int s;
  public:
	OrderedList();
	Node* insert(bestNeighbourPtr bnPtr);
	void remove(Node*);
	Node* update(Node*);
	Node* go_front();
	Node* advance();
	int size();

};

extern bool bestNeighbourOrder(const bestNeighbourPtr& bn1, const bestNeighbourPtr& bn2);

extern Log logCores;
extern Log logCommunities;
extern Log result_communities;

#endif

