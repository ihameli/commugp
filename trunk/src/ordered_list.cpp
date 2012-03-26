#include "ordered_list.h"
#include <iostream>

OrderedList::OrderedList() {
	first = NULL;
	last = NULL;
	actual = NULL;
	s = 0;
}

Node* OrderedList::insert(bestNeighbourPtr bnPtr) {
	Node* newNode = new Node();
	newNode->bnPtr = bnPtr;
	Node* node = first;
	bool found = false;
	while ((node!=NULL)&(!found)) {
		if (bestNeighbourOrder(newNode->bnPtr, node->bnPtr))
			found = true;
		else {
			node = node->next;
		}
	}
	if (node!=NULL) {
		if (node->prev!=NULL)
			node->prev->next = newNode;
		else {
			first = newNode;
		}
		newNode->prev = node->prev;

		node->prev = newNode;
		newNode->next = node;
	}
	else {
		newNode->prev = last;
		if (newNode->prev!=NULL)
			newNode->prev->next = newNode;
		else
			first = newNode;
		newNode->next = NULL;
		last = newNode;
	}
	s++;
	return newNode;
}

void OrderedList::remove(Node* node) {
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	else {
		first = node->next;
	}
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
	else
		last = node->prev;
	s--;
}

Node* OrderedList::update(Node* node) {
	remove(node);
	return insert(node->bnPtr);
}

Node* OrderedList::go_front() {
	actual  = first;
	return actual;
}

Node* OrderedList::advance() {
	actual = actual->next;
	return actual;
}

int OrderedList::size() {
	return s;
}

