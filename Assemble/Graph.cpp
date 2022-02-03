#include "..\pch.h"

Graph::~Graph(void) {
	Node* current = root;
	std::queue<Node*> queue;
	std::set<Node*> visited;

	queue.push(current);

	// Breadth-first search to store every node in a set
	while(queue.size() > 0) {
		Node* parent = queue.front();
		queue.pop();

		if(parent != NULL) {
			const auto end = visited.end();

			if(visited.find(parent) == end) {
				visited.insert(parent);

				if(parent->left != NULL) {
					const auto child = visited.find(parent->left);
					if(child == end) {
						queue.push(parent->left);
					}
				}

				if(parent->right != NULL) {
					const auto child = visited.find(parent->right);
					if(child == end) {
						queue.push(parent->right);
					}
				}
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