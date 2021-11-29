#pragma once
#include "..\pch.h"
#include "..\Const\Errors.hpp"

using namespace KeywordMap;

class Assemble {
private:
	// key = line number, value = Node
	std::map<int, Node*> code;
	// key = what node needs to do, value = Node
	std::map<std::string, Node*> brMap;

	Assemble(void) = default;
	
	/*
	* Handles if there is a string key in the brMap
	* 
	* @param str - the string to test
	* @param node - the node to go into brMap if there is no collision
	*/
	void isCollision(const std::string&, Node*);

	/*
	* Determin if the current line of code is legal, or not.
	* 
	* @param codeLine - The line of code being checked
	* @param order - The order of execution
	* @returns - true if the line is legal else false
	*/
	bool isLineLegal(std::string&, const int&);

	void removeBadSpacing(std::string&) const;

	/*
	* Any time a varible is found declare and store it
	*
	* @param instruction - what the code needs to do
	*/
	void declareVars(const std::string&);

	/*
	* If a line of code is not legal (see isLineLegal function) then throw an
	error
	*
	* @param error - Error used to generate error message
	* @param codeLine - Line of code that caused the error
	* @param lineNum - Line number where error occurred
	*/
	void throwError(const Errors&, const std::string&, const int&) const;

public:
	Assemble(std::fstream&);
	~Assemble(void) = default;

	/*
	* Assemble all code
	*/
	void assemble(void);
};
