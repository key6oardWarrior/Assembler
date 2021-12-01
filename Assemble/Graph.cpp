#include "Graph.hpp"

Graph::~Graph(void) {
	Node* current = root;
	std::queue<Node*> queue;
	queue.push(current);
	std::set<Node*> visited;
	visited.insert(current);

	// Breadth-first search to store every node in a set
	while(queue.size() > 0) {
		Node* parent = queue.front();
		queue.pop();

		while(parent != NULL) {
			if(*(visited.find(parent)) != parent) {
				visited.insert(parent);

				if(parent->left != NULL) {
					queue.push(parent->left);
				}

				if(parent->right != NULL) {
					queue.push(parent->right);
				}
				break;
			} else {
				parent = (parent->left != NULL) ? parent->left : parent->right;
			}
		}
	}

	// delete every node stored in the set
	auto itr = visited.begin();
	while(itr != visited.end()) {
		auto prev = itr++;
		delete *prev;
	}
}
