#pragma once
#include "..\pch.h"
#include "..\Const\Errors.hpp"
#include "..\Const\Kewords.hpp"

using namespace KeywordMap;

class Assemble {
private:
	// key = command, value = instruction
	std::map<KeywordMap::keywords, std::string> code;

	/*
	* Determin if the current line of code is legal, or not.
	* 
	* @param codeLine - The line of code being checked
	* @returns - true if the line is legal else false
	*/
	bool isLineLegal(std::string&);

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

	// void assemble(void);
};
