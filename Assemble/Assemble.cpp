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

void Assemble::whitespaceRemover(std::string& str, const size_t& pos=1) const {
	switch(pos) { // remove trailing and begining whitespace(s)
		case 3: {
			auto itr = str.rbegin();
			size_t rindex = str.size();

			while(*itr == ' ') {
				rindex--;
				itr++;
			}

			str = str.substr(0, rindex);

			auto b_itr = str.begin();
			size_t index = 0;

			while(*b_itr == ' ') {
				index++;
				b_itr++;
			}

			str = str.substr(index);
			break;
		}

		case 2: {// remove trailing whitespaces
			auto itr = str.rbegin();
			size_t rindex = str.size();

			while(*itr == ' ') {
				rindex--;
				itr++;
			}

			str = str.substr(0, rindex);
			break;
		}
		default: // remove begining whitespaces
			auto b_itr = str.begin();
			size_t index = 0;
			while(*b_itr == ' ') {
				index++;
				b_itr++;
			}

			str = str.substr(index);
	}
}

bool Assemble::isLineLegal(std::string& codeLine) {
	whitespaceRemover(codeLine, 3);
	size_t commandIndex = codeLine.find(' ');
	std::string command = codeLine.substr(0, commandIndex);

	if(StrMap::keywordMap[command].size() == 0) {
		commandIndex = codeLine.find(' ', command.size()-1);
		const size_t stop = codeLine.find(' ', commandIndex)-1;
		command = codeLine.substr(commandIndex, stop);
		whitespaceRemover(command, 3);
	}

	// largest size of vector: 2
	const std::vector<char> accepts = StrMap::keywordMap[command];

	if(accepts.size() < 1) { // command not found
		return 0;
	}
	
	if(accepts[0] == 1) {
		return 1;
	}

	const size_t size = codeLine.size();
	const char back = std::tolower(codeLine.back());
	for(char ii : accepts) {
		if(back == ii) { // command found
			code.insert(std::pair<KeywordMap::keywords, std::string>
				(KeywordMap::keywordMap[command], codeLine.substr(
					commandIndex+1, size)));
			return 1;
		}
	}

	return 0;
}

void Assemble::assemble(void) {
	auto itr = code.begin();

	while(itr != code.end()) {
		
	}
}

void Assemble::throwError(const Errors& error, const std::string& codeLine,
	const size_t& lineNum) const {
	switch(error) {
		case Errors::CommandNotFound:
			throw "Command Not legal:\n" + codeLine + "\nLine: " +
				std::to_string(lineNum);
			break;
	}
}
