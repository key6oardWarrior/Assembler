#pragma once
#include "..\pch.h"
#include "..\Const\Errors.hpp"

class Assemble {
private:
	std::map<std::string*, int> code;

	/*
	* Determin if the current line of code is legal, or not.
	* 
	* @param codeLine - The line of code being checked
	* @returns - true if the line is legal else false
	*/
	bool isLineLegal(std::string&) const;

	/*
	* If a line of code is not legal (see isLineLegal function) then throw an
	error
	*
	* @param error - Error used to generate error message
	*/
	void throwError(const Errors&) const;

public:
	Assemble(std::fstream&);
	~Assemble(void) = default;

	// void assemble(void);
};
