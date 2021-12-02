#pragma once
#include "..\pch.h"
#include "Graph.hpp"
#include "..\Const\Errors.hpp"

using namespace KeywordMap;

class Assemble {
private:
	// the order code needs to execute
	Graph* order = new Graph();
	// key = line number, value = Node
	std::vector<Node*> code;
	// key = what node needs to do, value = Node
	std::map<std::string, Node*> brMap;
	std::map<std::string, std::vector<int>> vars;

	Assemble(void) = default;
	
	/*
	* Connect branching instructions together if there is a key collision
	* 
	* @param str - the string to test
	* @param node - the node to go into brMap if there is no collision
	*/
	void isCollision(const std::string&, Node*);

	/*
	* Determin if the current line of code is legal, or not.
	* 
	* @param codeLine - The line of code being checked
	* @returns - true if the line is legal else false
	*/
	bool isLineLegal(std::string&);

	void removeBadSpacing(std::string&) const;

	/*
	* Any time a varible is found declare and store it
	*
	* @param instruction - what the code needs to do
	*/
	void declareVars(void);

	/*
	* If a line of code is not legal (see isLineLegal function) then throw an
	error
	*
	* @param error - Error used to generate error message
	* @param codeLine - Line of code that caused the error
	* @param lineNum - Line number where error occurred
	*/
	void throwError(const Errors&, const std::string&, const size_t&) const;

public:
	Assemble(std::fstream&);

	~Assemble(void) {
		delete order;
		order = nullptr;
	}

	Graph* getOrder(void);
};
