#include "Assemble.hpp"

using namespace StrMap;

Assemble::Assemble(std::fstream& file) {
	std::string fileCode;
	int line = 0;

	if(file.is_open()) {
		while(file.eof() == 0) {
			std::getline(file, fileCode);
			line++;

			// ignore whitespace
			if((fileCode == "") || (fileCode == "\n") || (fileCode == "\t")
				|| (fileCode == " ")) {
				continue;
			}

			if(isLineLegal(fileCode) == 0) {
				throwError(Errors::CommandNotFound, fileCode, line);
			}
		}
	}

	file.close();
}

bool Assemble::isLineLegal(std::string& codeLine) {
	const size_t size = codeLine.size();
	size_t rindex = size;

	{ // remove trailing whitespace(s)
		auto itr = codeLine.rbegin();
		while(*itr == ' ') {
			rindex--;
			itr++;
		}

		codeLine = codeLine.substr(0, rindex);
	}

	size_t commandIndex = codeLine.find(' ');
	std::string command = codeLine.substr(0, commandIndex);
	// largest size of vector: 2
	const std::vector<char> accepts = StrMap::keywordMap[command];

	if(accepts.size() < 1) { // command not found
		return 0;
	}

	const char back = std::tolower(codeLine.back());
	commandIndex++;
	for(char ii : accepts) {
		if(back == ii) { // command found
			code.insert(std::pair<KeywordMap::keywords, std::string>
				(KeywordMap::keywordMap[command], codeLine.substr(
					commandIndex, size)));
			return 1;
		}
	}

	command[0] = std::tolower(command[0]);
	if((command[0] == 'b') || (command[0] == 's') || (command[0] == 'd')) {
		code.insert(std::pair<KeywordMap::keywords, std::string>(
			KeywordMap::keywordMap[command], codeLine.substr(commandIndex,
				size)));
		return 1;
	}

	return 0;
}

void Assemble::throwError(const Errors& error, const std::string& codeLine,
	const int& lineNum) const {
	switch(error) {
		case Errors::CommandNotFound:
			throw "Command Not legal:\n" + codeLine + "\nLine: " +
				std::to_string(lineNum);
			break;
	}
}
