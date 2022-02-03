#pragma once

using namespace KeywordMap;
typedef Keywords Key;

class Runtime {
private:
	Graph* order;
	Node* node;
	MemoryArray* memory;
	std::map<std::string, DataType*> vars;
	size_t line = 1;
	bool N = 0;
	bool Z = 0;
	bool C = 0;
	bool V = 0;

	Runtime(void) = default;

	/*
	* Determin addressing mode and return the value at the given memory
	* location
	* 
	* @return a value at a memory location, or the indirect value
	*/
	int addressingMode(void);

	/*
	* Find the integer in the string
	* 
	* @param str - the string to be searched
	* @returns the short that is before the memory addresser
	*/
	short findInt(const std::string&) const;

	/*
	* Execute each line of code
	* 
	* @param node - line of code to be executed
	*/
	void execute(void);

public:
	Runtime(Graph* order, MemoryArray* memory,
		const std::map<std::string, DataType*>& vars) {

		this->order = order;
		this->memory = memory;
		this->vars = vars;
		node = order->getRoot();
	}

	~Runtime(void);

	/*
	* Run all the assembled code
	*/
	void run(void);
};
