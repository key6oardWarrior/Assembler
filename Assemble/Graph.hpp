#pragma once
#include "../pch.h"
#include "Node.hpp"

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
