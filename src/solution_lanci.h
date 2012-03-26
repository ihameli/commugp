#ifndef _SOLUTION_LANCI_H_
#define _SOLUTION_LANCI_H_

#include "solution.h"
#include "community.h"
#include <vector>
#include "graph.h"
#include "ordered_list.h"
#include <set>
#include "graph_social.h"

typedef struct {
	double t;
	int sign;
	double mV, mE;
	double st;
	double koki;
	double ko;
	double ki;
} tProcessData;

class SolutionLanci : public Solution {

  private:
	Graph_Social* network;
	std::list<Vertex*> removal;
	Community* join;
	double join_cut;
	Vertex* initialVertex;

	bool eliminations;

	double r;
	double mV, mE;
	double delta_mV, delta_mE;
	double T;
	double newT;
	unsigned int last_added_node;
	double last_added_node_mV;
	double last_added_node_mE;
	double last_added_node_st;
	double last_added_node_koki;
	int last_added_node_ko;
	int last_added_node_ki;
	std::list<unsigned int> last_deleted_nodes;
	std::list<double> last_deleted_nodes_mV;
	std::list<double> last_deleted_nodes_mE;
	std::list<double> last_deleted_nodes_st;
	std::list<double> last_deleted_nodes_koki;
	std::list<int> last_deleted_nodes_ko;
	std::list<int> last_deleted_nodes_ki;
	double total2Weights;

	int amountCommunities;

	Community* selected_community;
	double getFitnessForDelta(double delta_mV, double delta_mE);

	bool virtual proposeNeighbour();
	void virtual acceptNeighbour();
	void virtual rejectNeighbour();
	double virtual evaluateNeighbour();
	double virtual evaluate();

	std::list<std::pair<int, tProcessData> > process; //The list of vertices added and removed of the process. <vertex_id, 1|-1>
	std::list<std::pair<int, tProcessData> > clean_process; //The list of vertices after cleaning the process, leaving only the last insertions of the vertices

	void do_clean_process();

  public:

	double inline getT() { return T; }
	double inline getAmountCommunities() { return amountCommunities; }
	SolutionLanci(Graph_Social* m_network, bool eliminations = true);

	void findNaturalCommunity(Vertex* v, double r, bool logCout = true, bool logCommunities = false);
	double getFitness();
	std::list<Community*> getCommunities();
	void clearCommunities();
	void finalizeCommunities();
	void assignCommunitiesFromFile(std::string filename);
	void printCommunities();
	void logAllCommunities();

	std::list<std::pair<int, tProcessData> > inline getCleanProcess() { return clean_process; }

};

#endif

