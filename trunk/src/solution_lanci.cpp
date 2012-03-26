#include "solution_lanci.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

int step=0;
int tant;

Community *c_new, *c_old;

SolutionLanci::SolutionLanci(Graph_Social* m_network, bool m_eliminations) {

	eliminations = m_eliminations;

	network = m_network;
	total2Weights = network->tWeights();

	network->clearCommunities();
	std::list<Vertex*>::iterator it;

	for (it=network->getVertices().begin();it!=network->getVertices().end();it++) {
		Community* va = new Community();
		network->getCommunities().push_back(va);
		(*it)->setCommunity(va);
		va->addVertex((*it));
		network->existent_vertices[(*it)->getNumber()] = va;
	}

	amountCommunities = network->getCommunities().size();

	T = -1;

	tant = -1;
}

double SolutionLanci::getFitness() {
	return (mV-mE)/pow(mV, r);
}

double SolutionLanci::getFitnessForDelta(double delta_mV, double delta_mE) {
	return (mV+delta_mV-mE-delta_mE)/pow(mV+delta_mV, r);
}

bool SolutionLanci::proposeNeighbour() {
//	std::cout << "Tomando mejores t..." << std::endl;

	join = NULL;
	join_cut = 0;
	removal.clear();

	std::vector<Community*> neighbourCommunities = selected_community->getNeighbourCommunities();
//	std::cout << "Tengo vecinos " << neighbourCommunities.size() << std::endl;
	std::vector<double> neighbourCommunityCuts = selected_community->getNeighbourCommunityCuts();

	std::vector<Community*>::iterator itNeighbourCommunities = neighbourCommunities.begin();
	std::vector<double>::iterator itNeighbourCommunityCuts = neighbourCommunityCuts.begin();

	Community* nb;
	double cut;

	double selected_cut;

//	double fitness;
//	double bestFitness = -1;

	double eachT;
	double minT = -1;

//	selected_community->printCommunity();

	for (itNeighbourCommunities = neighbourCommunities.begin(); itNeighbourCommunities != neighbourCommunities.end(); itNeighbourCommunities++) {
		nb = *itNeighbourCommunities;
		cut = *itNeighbourCommunityCuts;
		if (nb != selected_community) {
			//std::cout << "cut con " << nb->getVertices().front()->getNumber() << ": " << cut << ", nodos " << nb->getSize() << std::endl;
			//std::cout << "t: " << total2Weights << std::endl;
			delta_mV = ((double)(nb->tStrengths())) / total2Weights;
			delta_mE = ((double)(nb->tStrengths() - (2 * cut))) / total2Weights;
			//std::cout << "tengo vecino " << delta_mV << " " << delta_mE << std::endl;
	//		eachT = -1;
	//		if (delta_mV - delta_mE != 0)
			eachT = (delta_mV*(mV + delta_mV / 2))/(delta_mV - delta_mE);
			//std::cout << "Vecino " << nb->getVertices().front()->getNumber() << " t: " << eachT << " delta_mV: " << delta_mV << " delta_mE: " << delta_mE << " grado: " << nb->tStrengths() << std::endl;
			if ((eachT < minT)||(minT == -1)) {
				//std::cout << "VECINO " << nb->getVertices().front()->getNumber() << " t: " << eachT << " minT: " << minT << std::endl;
				minT = eachT;
				join = nb;
				selected_cut = cut;
			}
		}
		itNeighbourCommunityCuts++;
	}

	join_cut = selected_cut;

	step++;

	//join->printCommunity();

	newT = max_from_2(T, minT);

	return ((neighbourCommunities.size()-1!=0)||(neighbourCommunities.front()!=selected_community));
}

void SolutionLanci::acceptNeighbour() {
//	std::cout << "Uniendo comunidades" << std::endl;
	double cut;
	double eachT;

	if (join != NULL) {
		cut = join_cut;
		c_new = selected_community;
		c_old = join;

		amountCommunities--;

		c_old->removed = true;
		delta_mV = (double)(c_old->tStrengths()) / total2Weights;
		delta_mE = ((double)(c_old->tStrengths() - 2 * cut))  / total2Weights;
		last_added_node = c_old->getVertices().front()->getNumber();

		last_added_node_st = (delta_mV*(mV + delta_mV / 2))/(delta_mV - delta_mE);
		last_added_node_koki = (double)(c_old->tStrengths() - cut) / c_old->tStrengths();
		last_added_node_ko = (int)(c_old->tStrengths() - cut);
		last_added_node_ki = (int)cut;

		mV += delta_mV;
		mE += delta_mE;

		last_added_node_mV = mV;
		last_added_node_mE = mE;

		c_new->addCommunity(c_old, cut);

		std::list<Vertex*>::iterator v_it;
		for (v_it = c_old->getVertices().begin(); v_it != c_old->getVertices().end() ; v_it++) {
			(*v_it)->setCommunity(c_new);
			network->existent_vertices[(*v_it)->getNumber()] = selected_community;
		}

		c_new->renew();

		c_old->removeCommunity();
		//c_new->printCommunity();

		delete(join);
		std::list<Community*>::iterator it_del;
		it_del = std::find(network->getCommunities().begin(), network->getCommunities().end(), join);
		if (it_del != network->getCommunities().end())
			network->getCommunities().erase(it_del);

		T = newT;
	}

	//TODO
	//Node removal
	bool removing = true;
	last_deleted_nodes.clear();
	last_deleted_nodes_mV.clear();
	last_deleted_nodes_mE.clear();
	double delta_mV_selected, delta_mE_selected;
	int cut_selected;
	if (eliminations) {
	while (removing) {
		removing = false;
		double maxT = -1;
		std::list<Vertex*> selected_comm_vertices = selected_community->getVertices();
		std::list<Vertex*>::iterator it;
		std::vector<Vertex*>::iterator it2;
		Vertex* v, *v_selected;
		for (it = selected_comm_vertices.begin(); it != selected_comm_vertices.end(); it++) {
			if (*it==initialVertex)
				continue;

			//Compute delta_mV
			v = (*it);
			delta_mV = -((double)(v->getStrength())) / total2Weights;
			double cut = 0;
			for (it2 = v->getNeighbours()->begin(); it2 != v->getNeighbours()->end(); it2++) {
				if ((*it2)->getCommunity() == selected_community) {
					cut++;
				}
			}
			//Compute delta_mE
			delta_mE = -((double)(v->getStrength() - (2 * cut))) / total2Weights;

			eachT = (delta_mV*(mV + delta_mV / 2))/(delta_mV - delta_mE);
			//std::cout << "nodo " << (*it) << ", delta_mV: " << delta_mV << ", delta_mE: " << delta_mE << std::endl;
			//std::cout << "nodo " << (*it) << ", T: " << eachT << ", Tactual: " << T << std::endl;
			if (eachT > maxT) {
				maxT = eachT;
				v_selected = v;
				delta_mE_selected = delta_mE;
				delta_mV_selected = delta_mV;
				cut_selected = cut;
			}
		}

		if (maxT > T + 0.0000000001) {
			selected_community->delVertex(v_selected);

			amountCommunities++;
			if (delta_mV - delta_mE != 0)
				last_deleted_nodes_st.push_back((delta_mV*(mV + delta_mV / 2))/(delta_mV - delta_mE));
			else
				last_deleted_nodes_st.push_back(-1);
			last_deleted_nodes_koki.push_back(delta_mE/delta_mV);

			mV += delta_mV_selected;
			mE += delta_mE_selected;
			//CREAR NUEVA COMUNIDAD PARA EL VERTICE
			//std::cout << "Nueva comunidad" << std::endl;
			Community* c = new Community();
			v_selected->setCommunity(c);
			c->addVertex(v_selected);
			network->existent_vertices[v_selected->getNumber()] = c;

			//Armado de la comunidad y sus conexiones
			network->getCommunities().push_back(c);

			last_deleted_nodes.push_back(v_selected->getNumber());
			last_deleted_nodes_mV.push_back(mV);
			last_deleted_nodes_mE.push_back(mE);
			last_deleted_nodes_ko.push_back(c->tStrengths() - cut_selected);
			last_deleted_nodes_ki.push_back(cut_selected);

			removing = true;
		}

	} //while (removing)
	} //if (eliminations)


}

void SolutionLanci::rejectNeighbour() {
//	std::cout << "rejectNeighbour" << std::endl;
}

double SolutionLanci::evaluateNeighbour() {
	return newT;
}

double SolutionLanci::evaluate() {
//	return Q - deltaQJump + (mNum/mDen) * (1 - T);
	return 0;
}

//COMMUNITIES

void SolutionLanci::printCommunities() {
	std::list<Community*>::iterator comm_it;
	int comm_number=1;
	for (comm_it = network->getCommunities().begin(); comm_it != network->getCommunities().end() ; comm_it++) {
		if ((*comm_it)->removed == false) {
			(*comm_it)->printCommunity();
/*			std::cout << comm_number << ": ";
			std::vector<Vertex*>::iterator v_it;
			for (v_it = (*comm_it)->getVertices().begin(); v_it != (*comm_it)->getVertices().end() ; v_it++) {
				std::cout << (*v_it)->getNumber() << " ";
			}
			std::cout << std::endl;
			comm_number++;
*/		}
	}
}

void SolutionLanci::logAllCommunities() {
}

void SolutionLanci::assignCommunitiesFromFile(std::string filename) {
/*	std::ifstream f;
	int n1,n2;
	std::map<int, Community*> mapita;

	f.open(filename.c_str());
	f >> n1 >> n2;
	while (!f.eof()) {
		//std::cout << n1 << " " << n2 << std::endl;
		if (mapita.find(n2)==mapita.end()) {
			Community* c = new Community();
			mapita[n2] = c;
			communities.push_back(c);
		}
		vertices_map[n1]->setCommunity(mapita[n2]);
		mapita[n2]->addVertex(vertices_map[n1]);
		f >> n1 >> n2;
	}
	f.close();
*/}

void SolutionLanci::do_clean_process() {

	std::list<int> nodes;
	std::list<int> def_nodes;
	std::list<int>::iterator nodes_it;
	std::map<int, int> already;
	std::map<int, tProcessData> data;

	int v;

	std::list<std::pair<int, tProcessData> >::iterator process_it;
	for (process_it=process.begin();process_it!=process.end();process_it++) {
		nodes.push_back((*process_it).first);
		data[(*process_it).first] = (*process_it).second;
	}

	nodes_it = nodes.end();

	do {
		nodes_it--;
		if (already.find(*nodes_it)==already.end()) {
			def_nodes.push_front(*nodes_it);
			already[*nodes_it] = 1;
		}
	}
	while (nodes_it != nodes.begin());

	for (nodes_it=def_nodes.begin();nodes_it!=def_nodes.end();nodes_it++) {
		v = (*nodes_it);
		clean_process.push_back(std::make_pair<int, tProcessData>(v, data[v]));
	}

}


void SolutionLanci::clearCommunities() {
	std::list<Community*>::iterator comm_it;
	for (comm_it=network->getCommunities().begin();comm_it!=network->getCommunities().end();comm_it++) {
		(*comm_it)->clear();
	}
	network->getCommunities().clear();
}

//Find communities up to r
void SolutionLanci::findNaturalCommunity(Vertex* v, double m_r, bool logCout, bool logCommunities) {
	
	r = m_r;
	last_added_node_mV = mV;

	//std::cout << "Paso 1" << std::endl;

	bool unaSolaComm=false;
	double st;

	selected_community = network->existent_vertices[v->getNumber()];

	initialVertex = v;

	mV = (double) v->getStrength() / total2Weights;
	mE = (double) v->getStrength() / total2Weights;

	//std::cout << "Paso 2" << std::endl;
//	SolutionSuborthogonal* solution = new SolutionSuborthogonal(communities, this);
	unaSolaComm = !proposeNeighbour();

//	double t = evaluateNeighbour();

	//std::cout << "t: " << t << std::endl;
//	while ((t >= 1)) {
	int i=0;
	double t_prev = -1;
	int amount_comm_prev = -1;
	tProcessData processData;

	std::list<unsigned int>::iterator deleted_it;
	std::list<double>::iterator deleted_mV_it;
	std::list<double>::iterator deleted_mE_it;
	std::list<double>::iterator deleted_st_it;
	std::list<double>::iterator deleted_koki_it;
	std::list<int>::iterator deleted_ko_it;
	std::list<int>::iterator deleted_ki_it;

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	processData.t = T;
	processData.sign = 1;
	processData.mV = mV;
	processData.mE = mE;
	processData.st = 0;
	processData.koki = 1;
	processData.ko = v->getDegree();
	processData.ki = 0;
	process.push_back(std::make_pair<int, tProcessData>(v->getNumber(), processData));
	if (logCommunities) {
		logGrowthProcess << T << "\t" << v->getNumber() << "\t1\t" << mV << "\t" << mE << "\t" << 0 << " " << 1 << " " << v->getDegree() << " " << 0 << "\n";
	}
	if (logCout) {
		std::cout << "-------------------------------------------------------------------------------------------------------\n";
		std::cout << "|      t     " << "      Node      " << "       mV   " << "       mE          st      ko/(ko+ki)        ko           ki |" << "\n";
		std::cout << "-------------------------------------------------------------------------------------------------------\n";
		std::cout << "| " << std::setw(12) << std::setprecision(3) << T << std::setw(12) << std::setprecision(3) << v->getNumber() << "(+)" << std::setw(12) << std::setprecision(3) << mV << std::setw(12) << std::setprecision(3) << mE << std::setw(12) << std::setprecision(3) << 0.000 << std::setw(12) << std::setprecision(3) << 1.000 << std::setw(12) << v->getDegree() << std::setw(12) << 0 << " |" << "\n";
	}

	double last_logged_i = i;

	while ((T <= r)&&(!unaSolaComm)) {

		i++;
		acceptNeighbour();

		processData.t = T;
		processData.sign = 1;
		processData.mV = last_added_node_mV;
		processData.mE = last_added_node_mE;
		processData.st = last_added_node_st;
		processData.koki = last_added_node_koki;
		processData.ko = last_added_node_ko;
		processData.ki = last_added_node_ki;
		process.push_back(std::make_pair<int, tProcessData>((int)last_added_node, processData));
		if (logCommunities) {
			logGrowthProcess << T << "\t" << (int)last_added_node << "\t1\t" << last_added_node_mV << "\t" << last_added_node_mE << "\t" << last_added_node_st << "\t" << last_added_node_koki << "\t" << last_added_node_ko << "\t" << last_added_node_ki << "\n";
		}
		deleted_mV_it = last_deleted_nodes_mV.begin();
		deleted_mE_it = last_deleted_nodes_mE.begin();
		deleted_st_it = last_deleted_nodes_st.begin();
		deleted_koki_it = last_deleted_nodes_koki.begin();
		deleted_ko_it = last_deleted_nodes_ko.begin();
		deleted_ki_it = last_deleted_nodes_ki.begin();
		for (deleted_it=last_deleted_nodes.begin();deleted_it!=last_deleted_nodes.end();deleted_it++) {
			processData.t = T;
			processData.sign = -1;
			processData.mV = (*deleted_mV_it);
			processData.mE = (*deleted_mE_it);
			processData.st = (*deleted_st_it);
			processData.koki = (*deleted_koki_it);
			processData.ko = (*deleted_ko_it);
			processData.ki = (*deleted_ki_it);
			process.push_back(std::make_pair<int, tProcessData>((int)(*deleted_it), processData));
			if (logCommunities) {
				logGrowthProcess << T << "\t" << (int)(*deleted_it) << "\t-1\t" << (*deleted_mV_it) << "\t" << (*deleted_mE_it) << "\t" << (*deleted_st_it) << "\t" << (*deleted_koki_it) << "\t" << (*deleted_ko_it) << "\t" << (*deleted_ki_it) << "\n";
			}
			deleted_mV_it++;
			deleted_mE_it++;
			deleted_st_it++;
			deleted_koki_it++;
			deleted_ko_it++;
			deleted_ki_it++;
		}

		amount_comm_prev = (int)getAmountCommunities();
		if ((i>=last_logged_i+1)&&(logCout)) {
			std::cout << "| " << std::setw(12) << std::setprecision(3) << T << std::setw(12) << std::setprecision(3) << last_added_node << "(+)" << std::setw(12) << std::setprecision(3) << last_added_node_mV << std::setw(12) << std::setprecision(3) << last_added_node_mE << std::setw(12) << std::setprecision(3) << last_added_node_st << std::setw(12) << std::setprecision(3) << last_added_node_koki << std::setw(12) << last_added_node_ko << std::setw(12) << last_added_node_ki << " |" << "\n";
			deleted_mV_it = last_deleted_nodes_mV.begin();
			deleted_mE_it = last_deleted_nodes_mE.begin();
			deleted_st_it = last_deleted_nodes_st.begin();
			deleted_koki_it = last_deleted_nodes_koki.begin();
			deleted_ko_it = last_deleted_nodes_ko.begin();
			deleted_ki_it = last_deleted_nodes_ki.begin();
			for (deleted_it=last_deleted_nodes.begin();deleted_it!=last_deleted_nodes.end();deleted_it++) {
				std::cout << "| " << std::setw(12) << std::setprecision(3) << T << std::setw(12) << std::setprecision(3) << (*deleted_it) << "(-)" << std::setw(12) << std::setprecision(3) << (*deleted_mV_it) << std::setw(12) << std::setprecision(3) << (*deleted_mE_it) << std::setw(12) << std::setprecision(3) << (*deleted_st_it) << std::setw(12) << std::setprecision(3) << (*deleted_koki_it) << std::setw(12) << (*deleted_ko_it) << std::setw(12) << (*deleted_ki_it) << " |" << "\n";
				deleted_mV_it++;
				deleted_mE_it++;
				deleted_st_it++;
				deleted_koki_it++;
				deleted_ko_it++;
				deleted_ki_it++;
			}
			last_logged_i = i;
		}

		t_prev = T;

		if (T<=r) {
			unaSolaComm = !proposeNeighbour();
			T = evaluateNeighbour();
		}
	}
	if (logCout)
		std::cout << "-------------------------------------------------------------------------------------------------------\n";

	finalizeCommunities();

	do_clean_process();

	if (logCommunities)
		logAllCommunities();
		
/*	Solution* solution = new SolutionKCores(vert, communities, partial_graph);
	std::cout << "Q final: " << solution->evaluate() << std::endl;
*/
}

void SolutionLanci::finalizeCommunities() {
	std::list<Community*>::iterator comm_it;
	int comm_number=1;
	for (comm_it = network->getCommunities().begin(); comm_it != network->getCommunities().end() ; comm_it++) {
		if ((*comm_it)->removed == false) {
			std::list<Vertex*>::iterator v_it;
			for (v_it = (*comm_it)->getVertices().begin(); v_it != (*comm_it)->getVertices().end() ; v_it++) {
				(*v_it)->setCommunity(*comm_it);
			}
			comm_number++;
		}
	}
}

std::list<Community*> SolutionLanci::getCommunities() { 
	std::list<Community*>::iterator comm_it;
	std::list<Community*> result;
	for (comm_it=network->getCommunities().begin();comm_it!=network->getCommunities().end();comm_it++) {
		if (*comm_it != NULL)
			result.push_back(*comm_it);
	}
	return result;
}

