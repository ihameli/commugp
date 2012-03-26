#include "connectedcomponent.h"
#include <iostream>

void ConnectedComponent::printConnectedComponent() {
	std::vector<Vertex*>::iterator vertices_it = vertices.begin();
	std::cout << "CONNECTED COMPONENT: ";
	while (vertices_it!=vertices.end()) {
		std::cout << (*vertices_it)->getNumber() << " ";
		vertices_it++;
	}
	std::cout << std::endl;
}


