#include "utils.h"
#include <iostream>
#include "math.h"
#include <vector>

double mutual_information(std::map<int, std::list<int> > partition_1, std::map<int, std::list<int> > partition_2) {
	std::vector<std::vector<int> > M; //intersections matrix
	M.resize(partition_1.size());

	int N = 0; //Number of nodes in the network

	std::list<int> C1, C2;

	std::map<int, std::list<int> >::iterator it_i, it_j;
	int i, j;

	i = 0;
	for (it_i=partition_1.begin();it_i!=partition_1.end();it_i++) {
		M[i].resize(partition_2.size());
		j = 0;
		for (it_j=partition_2.begin();it_j!=partition_2.end();it_j++) {
			M[i][j] = intersection(partition_1[it_i->first], partition_2[it_j->first]).size();
			j++;
		}
		N += partition_1[it_i->first].size();
		i++;
	}

	double Num = 0;

	i = 0;
	for (it_i=partition_1.begin();it_i!=partition_1.end();it_i++) {
		j = 0;
		for (it_j=partition_2.begin();it_j!=partition_2.end();it_j++) {
			C1 = partition_1[it_i->first];
			C2 = partition_2[it_j->first];
			if (M[i][j] != 0) {
				Num += (double)M[i][j]*log(((double)M[i][j]*N)/(C1.size()*C2.size()));
			}
			j++;
		}
		i++;
	}

	double Den = 0;

	for (it_i=partition_1.begin();it_i!=partition_1.end();it_i++) {
		C1 = partition_1[it_i->first];
		Den += C1.size()*log((double)C1.size()/N);
	}
	for (it_j=partition_2.begin();it_j!=partition_2.end();it_j++) {
		C2 = partition_2[it_j->first];
		Den += C2.size()*log((double)C2.size()/N);
	}

	return -2.0*Num/Den;
}

