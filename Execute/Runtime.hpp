#pragma once
#include "..\pch.h"

using namespace KeywordMap;
typedef Keywords Key;

class Runtime {
private:
	Graph* order;
	std::map<std::string, DataType*> vars;
	int memory[u_max] {0};
	size_t line = 1;
	bool N = 0;
	bool Z = 0;
	bool C = 0;
	bool V = 0;

	Runtime(void) = default;

	/*
	* @returns the int that is before the memory addresser
	*/
	int findInt(const std::string&) const;

	/*
	* Determin the addressing more and add the correct value to the regester
	* 
	* @param am - The addressing mode string
	* @param regNum - The reference to the regester that will be added
	*/
	void addAddressingMode(volatile int& regNum, const std::string& am) const {
		if(am.back() == 'i') {
			regNum += findInt(am);
		} else {
			regNum += memory[findInt(am)];
		}
	}

	/*
	* Determin the addressing more and add the correct value to the regester
	*
	* @param am - The addressing mode string
	* @param regNum - The reference to the regester that will be subtracted
	*/
	void subAddressingMode(volatile int& regNum, const std::string& am) const {
		if(am.back() == 'i') {
			regNum -= findInt(am);
		} else {
			regNum -= memory[findInt(am)];
		}
	}

	/*
	* Execute each line of code
	* 
	* @param node - line of code to be executed
	*/
	void execute(Node*&);

public:
	Runtime(Graph* order, const std::map<std::string, DataType*>& vars) {
		this->order = order;
		this->vars = vars;
	}

	~Runtime(void) = default;

	/*
	* Run all the assembled code
	*/
	void run(void);
};