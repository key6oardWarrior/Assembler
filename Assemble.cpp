#include "Assemble.hpp"

Assemble::Assemble(std::fstream& file) {
	std::string fileCode;
	int line = 0;

	if(file.is_open()) {
		while(file.eof() == 0) {
			std::getline(file, fileCode);

			// ignore whitespace
			if((fileCode == "") || (fileCode == "\n") || (fileCode == "\t")
				|| (fileCode == " ")) {
				continue;
			}

			if(isLineLegal(fileCode)) {
				code.insert(std::pair<std::string*, int>(&fileCode, line++));
			} else {
				throwError(Errors::CommandNotFound);
			}
		}
	}

	file.close();
}

bool Assemble::isLineLegal(std::string& codeLine) const {
	size_t rindex = codeLine.size();

	{ // remove trailing whitespace(s)
		auto itr = codeLine.rbegin();
		while(*itr == ' ') {
			rindex--;
			itr++;
		}

		codeLine = codeLine.substr(0, rindex);
	}

	std::string command = codeLine.substr(0, codeLine.find(' '));
	const std::vector<char> accepts = keywords[command]; // largest size 2

	if(accepts.size() < 1) { // command not found
		return 0;
	}

	const char back = std::tolower(codeLine.back());
	for(char ii : accepts) {
		if(back == ii) { // command found
			return 1;
		}
	}

	command[0] = std::tolower(command[0]);
	if((command[0] == 'b') || (command[0] == 's') || (command[0] == 'd')) {
		return 1;
	}

	return 0;
}

void Assemble::throwError(const Errors& error) const {
	switch(error) {
		case Errors::CommandNotFound:
			throw "Command Not legal";
			break;
	}
}