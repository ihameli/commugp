#include <iostream>
#include "graph_builder_txt.h"
#include "stdlib.h"
#include <sys/stat.h>
#include "string.h"
#include "utils.h"
#include "mutual_information.h"
#include "solution_lanci.h"


void printHeader() {
	std::cout << "CommUGP 1.0\n";
	std::cout << "-----------\n\n";
	std::cout << "Local community detection algorithm based on a uniform fitness growth process.\nSoftware license: Academic Free License (AFL)";
	std::cout << "\nAuthors: J. R. Busch, M. G. Beiró, J. I. Alvarez-Hamelin";
	std::cout << "\nFacultad de Ingeniería - Universidad de Buenos Aires\n\n";
}


void cut_communities_lanci(Graph_Social* network, std::list<std::pair<int, tProcessData> > clean_process) {

	network->getCommunities().clear();
	network->existent_vertices.clear();

	Vertex* v;

	int n_prev, n;
	Community* actualCommunity;
//	Community* actualCommunity = new Community();

//	network->getCommunities().push_back(actualCommunity);

	std::map<int, Vertex*> all_v = network->getMapIntVertices();
	std::map<int, Vertex*>::iterator it_all_v;

	int ko, ki;

	double R_prev = -100;
	double R, R_prox;
	double last_min = 100;

	tProcessData processData;

	int i = 0; //Contador

	actualCommunity = new Community();
	network->getCommunities().push_back(actualCommunity);

	//Cross-checking
	for (it_all_v=all_v.begin();it_all_v!=all_v.end();it_all_v++) {
		(*it_all_v).second->setCommunity(NULL);
	}

	std::list<std::pair<int, tProcessData> >::iterator clean_process_it;
	clean_process_it = clean_process.begin();
	n_prev = (*clean_process_it).first;
	processData = (*clean_process_it).second;

	while (clean_process_it!=clean_process.end()) {

		i++;
		it_all_v = all_v.find(n_prev);

		if (it_all_v == all_v.end())
			throw ExceptionVertexNotExistent(n_prev);
		
		ko = processData.ko;
		ki = processData.ki;

		R = ((double)(ki-ko)/(ki+ko));
		
		clean_process_it++;
		n = (*clean_process_it).first;
		processData = (*clean_process_it).second;

		ko = processData.ko;
		ki = processData.ki;

		R_prox = ((double)(ki-ko)/(ki+ko));

		//std::cout << "R: " << R << ", last_min: " << last_min << ", R_prev: " << R_prev << ", R_prox: " << R_prox << std::endl;
		if ((R<=last_min)&&(R<R_prev)) {
			last_min = R;
			actualCommunity = new Community();
			network->getCommunities().push_back(actualCommunity);
			//std::cout << "Community starting from vertex " << n_prev << std::endl;
		}

		if ((R_prox>R)&&(R<R_prev)&&(R<R_prox)) {
			last_min = R;
		}

		R_prev = R;

		(*it_all_v).second->setCommunity(actualCommunity);
		actualCommunity->addVertex((*it_all_v).second);
		network->existent_vertices[n_prev] = actualCommunity;
		
		n_prev = n;

	}

	//Cross-checking
	for (it_all_v=all_v.begin();it_all_v!=all_v.end();it_all_v++) {
		if ((*it_all_v).second->getCommunity() == NULL) {
			std::vector<Vertex*>::iterator ngb_it;
			for (ngb_it=(it_all_v->second)->getNeighbours()->begin();ngb_it!=(it_all_v->second)->getNeighbours()->end();ngb_it++) {
				if ((*ngb_it)->getCommunity()!=NULL) {
					(*it_all_v).second->setCommunity((*ngb_it)->getCommunity());
					(*ngb_it)->getCommunity()->addVertex((*it_all_v).second);
					network->existent_vertices[(it_all_v->second)->getNumber()] = (*ngb_it)->getCommunity();
					break;
				}
			}
			if ((*it_all_v).second->getCommunity() == NULL) {
				actualCommunity = new Community();
				network->getCommunities().push_back(actualCommunity);
				(*it_all_v).second->setCommunity(actualCommunity);
				actualCommunity->addVertex((*it_all_v).second);
				network->existent_vertices[(it_all_v->second)->getNumber()] = actualCommunity;
			}
		}
	}

}
	

int buscar_maximo(std::map<Community*, int> map_neighbour_communities, Community* &comm_max_cut) {
	std::map<Community*, int>::iterator map_it;
	int max_cut = -1;
	for (map_it=map_neighbour_communities.begin();map_it!=map_neighbour_communities.end();map_it++) {
		if (map_it->second > max_cut) {
			max_cut = map_it->second;
			comm_max_cut = map_it->first;
		}
	}
	return max_cut;
}

int buscar_apareo_comm(Graph_Social* graph, Community* &comm1, Community* &comm2, bool permitir_k_i_cero) {

	bool enc = false;
	int max_cut = -1;
	int sum_ki_max = -1;

	std::list<Community*> comm_list = graph->getCommunities();
	std::list<Community*>::iterator comm_it;
	Community* c, *ngb_c;

	int sum_ki, sum_ko;
	int cut;

	for (comm_it=comm_list.begin();comm_it!=comm_list.end();comm_it++) {
		c = (*comm_it);
		
		comm_it++;

		if (comm_it==comm_list.end())
			break;

		ngb_c = *(comm_it);
		comm_it--;

		sum_ki = c->tEdges();
		sum_ko = c->tStrengths() - c->tEdges();

		cut = c->getCommunityCut(ngb_c);

		if ((cut > sum_ki)&&(cut - sum_ki > max_cut - sum_ki_max)&&((sum_ki != 0)||(permitir_k_i_cero))) {
			max_cut = cut;
			sum_ki_max = sum_ki;
			//comm1 = v->getCommunity();
			comm1 = c;
			comm2 = ngb_c;
			enc = true;
		}

		sum_ki = ngb_c->tEdges();
		sum_ko = ngb_c->tStrengths() - ngb_c->tEdges();

		if ((cut > sum_ki)&&(cut - sum_ki > max_cut - sum_ki_max)&&((sum_ki != 0)||(permitir_k_i_cero))) {
			max_cut = cut;
			sum_ki_max = sum_ki;
			//comm1 = v->getCommunity();
			comm1 = ngb_c;
			comm2 = c;
			enc = true;
		}

	}
	return enc;
}

int buscar_apareo(Graph* graph, Vertex* &vertex, Community* &comm2, bool permitir_k_i_cero) {

	bool enc = false;
	int max_max_cut = -1;
	int k_i_max = -1;
	Vertex* v_max = NULL;

	std::list<Vertex*> vertices = graph->getVertices();
	std::list<Vertex*>::iterator vertex_it;
	Vertex* v;
	Community* c, *ngb_c;

	for (vertex_it=vertices.begin();vertex_it!=vertices.end();vertex_it++) {
		v = (*vertex_it);
		c = (*vertex_it)->getCommunity();
		std::list<int> intersect = intersection(v->listNeighbours(), c->listVertices());
		int k_i = intersect.size();
		int k_o = v->getDegree() - k_i;

		std::map<Community*, int> map_neighbour_communities;
		std::vector<Vertex*>* ngb = v->getNeighbours();
		std::vector<Vertex*>::iterator ngb_it;
		for (ngb_it = ngb->begin();ngb_it != ngb->end();ngb_it++) {
			ngb_c = (*ngb_it)->getCommunity();
			if (ngb_c != c) {
				if (map_neighbour_communities.find(ngb_c) != map_neighbour_communities.end()) {
					map_neighbour_communities[ngb_c]++;
				}
				else {
					map_neighbour_communities[ngb_c] = 1;
				}
			}
		}

		Community* comm_max_cut;
		int max_cut = buscar_maximo(map_neighbour_communities, comm_max_cut);
		//std::cout << v->getNumber() << " " << k_i << " " << k_o << " " << map_neighbour_communities.size() << " " << max_cut << std::endl;
		if ((max_cut > k_i)&&(max_cut - k_i > max_max_cut - k_i_max)&&((k_i != 0)||(permitir_k_i_cero))) {
			max_max_cut = max_cut;
			k_i_max = k_i;
			v_max = v;
			//comm1 = v->getCommunity();
			comm2 = comm_max_cut;
			enc = true;
		}








	}
	vertex = v_max;
//	if (v_max != NULL)
//		std::cout << "Peor: vert " << v_max->getNumber() << " " << k_i_max << " " << max_max_cut << std::endl;
	return enc;
}

std::list<Vertex*>::iterator vertex_it_una_pasada;


int buscar_apareo_una_pasada(Graph* graph, Vertex* &vertex, Community* &comm2, bool permitir_k_i_cero) {

	bool enc = false;
	int max_max_cut = -1;
	int k_i_max = -1;
	Vertex* v_max = NULL;

	Vertex* v;
	Community* c, *ngb_c;

	if (vertex_it_una_pasada!=graph->getVertices().end()) {
		v = (*vertex_it_una_pasada);
		c = (*vertex_it_una_pasada)->getCommunity();
		std::list<int> intersect = intersection(v->listNeighbours(), c->listVertices());
		int k_i = intersect.size();
		int k_o = v->getDegree() - k_i;

		std::map<Community*, int> map_neighbour_communities;
		std::vector<Vertex*>* ngb = v->getNeighbours();
		std::vector<Vertex*>::iterator ngb_it;
		for (ngb_it = ngb->begin();ngb_it != ngb->end();ngb_it++) {
			ngb_c = (*ngb_it)->getCommunity();
			if (ngb_c != c) {
				if (map_neighbour_communities.find(ngb_c) != map_neighbour_communities.end()) {
					map_neighbour_communities[ngb_c]++;
				}
				else {
					map_neighbour_communities[ngb_c] = 1;
				}
			}
		}

		Community* comm_max_cut;
		int max_cut = buscar_maximo(map_neighbour_communities, comm_max_cut);
		//std::cout << v->getNumber() << " " << k_i << " " << k_o << " " << map_neighbour_communities.size() << " " << max_cut << std::endl;
		if (max_cut > k_i) {
			comm2 = comm_max_cut;
			enc = true;
		}

		vertex_it_una_pasada++;
	}

	vertex = v;
//	if (v_max != NULL)
//		std::cout << "Peor: vert " << v_max->getNumber() << " " << k_i_max << " " << max_max_cut << std::endl;
	return enc;
}


void log_communities(Graph_Social* graph, std::string file, bool incremental_comm_number = false) {
	std::fstream salida_comunidades;
	salida_comunidades.open(file.c_str(), std::ios::out);
	std::list<Community*>::iterator comm_it;
	std::map<int, int> mapa_size_communities;
	int i = 0;
	for (comm_it = graph->getCommunities().begin(); comm_it != graph->getCommunities().end() ; comm_it++) {
		if ((*comm_it)->removed == false) {
			std::list<Vertex*>::iterator v_it;
			for (v_it = (*comm_it)->getVertices().begin(); v_it != (*comm_it)->getVertices().end() ; v_it++) {
//				std::cout << (*v_it)->getNumber() << " " << (*comm_it)->getNumber() << std::endl;
				if (!incremental_comm_number)
					salida_comunidades << (*v_it)->getNumber() << " " << (*comm_it)->getNumber() << "\n";
				else
					salida_comunidades << (*v_it)->getNumber() << " " << i << "\n";
			}
			i++;
		}
	}

}

int main(int argc, char* argv[]) {

	try {
		Graph_builder_txt graph_builder;
		Graph_Social* graph = new Graph_Social();
		int fileExists;

		bool allComponents = false;

		bool eliminations = true;

		if (argc<2) {
			printHeader();
			std::cout << "Syntax: ./commugp [network_file]" << std::endl << std::endl;
			exit(1);
		}

		printHeader();

		struct stat stFileInfo;
		int intStat;

		fileExists = stat(argv[1], &stFileInfo);
		if(fileExists != 0) {
			std::cout << "Error: File \"" << argv[1] << "\" not found." << std::endl << std::endl;
			exit(1);
		}

		double r = 999999; //FIXME

		unsigned int starting_node = atof(argv[3]); //FIXME

		graph_builder.readFile(argv[1], graph);
		//graph->printGraph();

		graph->findConnectedComponents();

		ConnectedComponent* connectedComponent;
		std::vector<ConnectedComponent*>::iterator connectedComponent_it;

		std::list<std::pair<int, tProcessData> > complete_clean_process;

		std::cout << "Running fitness growth process...";
		fflush(stdout);

		for (connectedComponent_it=graph->getConnectedComponents().begin();connectedComponent_it!=graph->getConnectedComponents().end();connectedComponent_it++) {

			Graph_Social* graph_b = (Graph_Social*)graph->extractComponent(*connectedComponent_it);

			SolutionLanci* solution = new SolutionLanci(graph_b);
			solution->findNaturalCommunity(graph_b->getVertex(graph_b->getVertices().front()->getNumber()), r, false, true);
			//graph_b->printCommunities();

			std::cout.setf(std::ios::floatfield);
			//std::cout <<solution->getAmountCommunities() << " communities." << std::endl;

			std::list<std::pair<int, tProcessData> > clean_process = solution->getCleanProcess();
			complete_clean_process.insert(complete_clean_process.end(), clean_process.begin(), clean_process.end());

			//std::list<std::pair<int, tProcessData> >::iterator clean_process_it;
			//for (clean_process_it=clean_process.begin();clean_process_it!=clean_process.end();clean_process_it++) {
			//	double R = ((double)((*clean_process_it).second.ki-(*clean_process_it).second.ko)/((*clean_process_it).second.ki+(*clean_process_it).second.ko));
			//	std::cout << (*clean_process_it).first << " " << R << std::endl;
			//}
		}

		std::cout << " DONE" << std::endl;
		std::cout << "(Full list of t, Node, mV, mE, st, ko/(ko+ki), ko, ki evolution in log/community.log)" << std::endl;

		bool first_stage=true, second_stage=true, third_stage=true;
		int j = 0;
		bool enc;

		std::cout << "Extracting community partition..." << std::endl;
		if (first_stage) {

			//STAGE 1
			std::cout << "Stage 1...";
			fflush(stdout);
			cut_communities_lanci(graph, complete_clean_process);
			log_communities(graph, "log/community_stage1.log");
			std::cout << " DONE" << std::endl;
		}

		if (second_stage) {

			//STAGE 2
			std::cout << "Stage 2...";
			fflush(stdout);
			do {
				Community *comm1, *comm2;
				j++;

				enc = buscar_apareo_comm(graph, comm1, comm2, true);
				if (enc) {
					int cut = comm1->getCommunityCut(comm2);
					comm1->addCommunity(comm2, cut);

					std::list<Vertex*>::iterator v_it;
					for (v_it = comm2->getVertices().begin(); v_it != comm2->getVertices().end() ; v_it++) {
						(*v_it)->setCommunity(comm1);
					}

					comm2->removeCommunity();

					graph->getCommunities().remove(comm2);
					comm2->removed = true;

				}
			} while (enc);
			std::cout << " DONE" << std::endl;
			log_communities(graph, "log/community_stage2.log");
		}


		if (third_stage) {
			std::cout << "Stage 3...";
			fflush(stdout);
			j=0;
			int pasadas=0;
			int j_inic;

			//STAGE 3

			do {
				j_inic = j;
				pasadas++;

				vertex_it_una_pasada=graph->getVertices().begin();
				do {
					Vertex* vertex;
					Community *comm1, *comm2;
					enc = buscar_apareo_una_pasada(graph, vertex, comm2, true);

					if (enc) {
						j++;
						comm1 = vertex->getCommunity();
						comm2->addVertex(vertex);
						comm1->delVertex(vertex);

						if (comm1->getSize() == 0) {
							comm1->removeCommunity();
							graph->getCommunities().remove(comm1);
							comm1->removed = true;
						}
						vertex->setCommunity(comm2);

					}
				} while ((vertex_it_una_pasada!=graph->getVertices().end())&&(pasadas<=5));
			} while (j_inic != j);

		}
		std::cout << " DONE" << std::endl;
		log_communities(graph, "log/community_stage3.log");

		log_communities(graph, "communities.log", true);

		std::cout << graph->getCommunities().size() << " communities found. The community partition is in communities.log" << std::endl;

	}
	catch (exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;

}

