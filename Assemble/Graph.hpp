#pragma once
#include "..\pch.h"

class Graph {
private:
	Node* root;

	Graph(void) = default;

public:
	Graph(Node* node) {
		root = node;
	}

	~Graph(void);

	Node* getRoot(void) {
		return root;
	}
};
