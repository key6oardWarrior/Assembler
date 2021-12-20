#pragma once

struct Node {
	Node* left = NULL; // a branching instruction's next execuition will go here
	Node* right = NULL; // the new procedural instruction will go here
	KeywordMap::Keywords instruction;
	std::string specifier;
	bool isGo2 = 0; // if Node has a left branch, or is left branch

	Node(void) = default;
	~Node(void) = default;
};