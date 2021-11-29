#pragma once
#include "../pch.h"
#include "Node.hpp"

class Graph {
private:
	Node* root;
	int height = 0;

	Graph(void) = default;

public:
	Graph(Node* node) {
		root = node;
		height = 1;
	}

	Node* getRoot(void) { return root; }

	int getHeight(void) { return height; }
};
