#pragma once

class Assemble {
private:
	std::fstream file;
	Node* prev;
	// the order code needs to execute
	Graph* order = new Graph();
	// key = what node needs to do, value = Node
	std::map<std::string, Node*> brMap;
	// key = var name, value = a continuous memory space
	std::map<std::string, DataType*> vars;
	// Memory used to store vars
	MemoryArray* memory = new MemoryArray();
	size_t index = 0;
	bool isArr = 0;
	std::string lastVar = "";

	Assemble(void) = default;
	
	/*
	* Connect branching instructions together if there is a key collision
	* 
	* @param str - the string to test
	* @param node - the node to go into brMap if there is no collision
	*/
	void isCollision(const std::string&, Node*&);

	/*
	* Check the brMap to ensure that all goto Nodes goto valid Nodes
	*/
	void checkGraph(void) const;

	/*
	* Determin if the current line of code is legal, or not.
	* 
	* @param codeLine - The line of code being checked
	* @returns - true if the line is legal else false
	*/
	bool isLineLegal(std::string&);

	/*
	* If the user enters to many spaces in some places the spaces must be removed
	* 
	* @param str - The line of code that will be checked for bad spacing
	*/
	void removeBadSpacing(std::string&) const;

	/*
	* Determin if the string is in base 10, 2, or 16 then return the base 10
	* integer.
	* 
	* @param memorySize - the size of the memory block
	* @returns Base 10 integer
	*/
	int findInt(const std::string&) const;

	/*
	* Any time a varible is found declare and store it
	*
	* @param declaration - var that needs to be stored
	* @param index - where the first ' ' char is located in declaration
	*/
	void declareVars(const std::string&, const size_t&);

	/*
	* If a line of code is not legal (see isLineLegal function) then throw an
	error
	*
	* @param error - Error used to generate error message
	* @param codeLine - Line of code that caused the error
	* @param lineNum - Line number where error occurred
	*/
	void throwError(const AssembleErrors&, const std::string&, const size_t&) const;

public:
	Assemble(const std::string& fileName) { file = std::fstream(fileName); }

	~Assemble(void) {
		delete order;
		order = nullptr;

		if(file.is_open()) {
			file.close();
		}
	}

	/*
	* Assemble all code in assembly file
	*/
	void assembleCode(void);

	Graph* getOrder(void);

	std::map<std::string, DataType*> getVars(void);

	MemoryArray* getMemory(void);
};