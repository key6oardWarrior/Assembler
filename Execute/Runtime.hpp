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
	* @returns the int that is before the memory addresser
	*/
	int findInt(const std::string&) const;

	/*
	* Determin the addressing more and add the correct value to the regester
	* 
	* @param regNum - The reference to the regester that will be added
	* @param am - The addressing mode string
	*/
	void addAddressingMode(volatile int& regNum, const std::string& am) const {
		if(am.back() == 'i') {
			regNum += findInt(am);
		} else {
			regNum += 0; // memory[findInt(am)];
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
			regNum -= 0; // memory[findInt(am)];
		}
	}

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

	~Runtime(void) = default;

	/*
	* Run all the assembled code
	*/
	void run(void);
};