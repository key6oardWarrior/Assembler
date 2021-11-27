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
	* Removes whitespaces from either left, right, or both side of the string
	* 
	* @param str - the string whitespaces will be removed from
	* @param pos - if 3 both left and right side will be removed, if 2 only right
		side will be removed, else only left side will be removed
	*/
	void whitespaceRemover(std::string&, const size_t&) const;

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
