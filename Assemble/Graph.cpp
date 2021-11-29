#include "Assemble/Graph.hpp"

Graph::~Graph(void) {
	Node* current = root;
	std::queue<Node*> queue;
	queue.push(current);
	std::set<Node*> visited;
	visited.insert(current);

	// Breadth-first search to store every node in a set
	while (queue.size() > 0) {
		Node* left = queue.front();
		Node* right = left;
		queue.pop();

		while (left != NULL) {
			if (*(visited.find(left)) != left) {
				visited.insert(left);
				if (left->left != NULL) {
					queue.push(left->left);
				}

				if (left->right != NULL) {
					queue.push(left->right);
				}
			}

			left = (left->left != NULL) ? left->left : left->right;
		}
	}

	auto itr = visited.begin();
	while (itr != visited.end()) {
		auto prev = itr++;
		delete* prev;
	}
}