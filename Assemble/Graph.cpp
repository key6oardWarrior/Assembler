#include "..\pch.h"

Graph::~Graph(void) {
	Node* current = root;
	std::queue<Node*> queue;
	std::set<Node*> visited;

	queue.push(current);
	visited.insert(current);

	// Breadth-first search to store every node in a set
	while(queue.size() > 0) {
		Node* parent = queue.front();
		queue.pop();

		while(parent != NULL) {
			const auto node = visited.find(parent);
			const auto end = visited.end();

			if((node == end) || (*node != parent)) {
				visited.insert(parent);

				if(parent->left != NULL) {
					const auto nextNode = visited.find(parent->left);
					if((nextNode == end) || (*(nextNode) != parent->left)) {
						queue.push(parent->left);
					}
				}

				if(parent->right != NULL) {
					const auto nextNode = visited.find(parent->right);
					if((nextNode == end) || (*(nextNode) != parent->right)) {
						queue.push(parent->right);
					}
				}
				parent = NULL;
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