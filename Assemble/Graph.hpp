#pragma once

class Graph {
private:
	Node* root;

public:
	Graph(void) = default;
	~Graph(void);

	Node* getRoot(void) {
		return root;
	}

	void setRoot(Node* node) {
		root = node;
	}
};