#include "Assemble.hpp"

using namespace StrMap;

Assemble::Assemble(std::fstream& file) {
	std::string fileCode;
	size_t line = 0;

	if(file.is_open()) {
		while(file.eof() == 0) {
			std::getline(file, fileCode);
			line++;

			// ignore whitespace and comments
			if((fileCode == "") || (fileCode == "\n") || (fileCode == "\t")
				|| (fileCode == " ") || (fileCode[0] == ';')) {
				continue;
			}

			{ // ignore comments
				size_t comment = fileCode.find(';');
				if(comment != fileCode.npos) {
					fileCode = fileCode.substr(0, comment);
				}
			}

			removeBadSpacing(fileCode);

			if(isLineLegal(fileCode) == 0) {
				file.close();
				throwError(Errors::CommandNotFound, fileCode, line);
			}
		}
	} else {
		throwError(Errors::FileNotOpen, "", 1);
	}

	file.close();
	order->setRoot(code[0]);
}

void Assemble::removeBadSpacing(std::string& str) const {
	auto itr = str.begin();
	int cnt = 0;

	while(itr != str.end()) {
		if(*itr == ' ') {
			cnt++;
		} else {
			cnt = 0;
		}

		if(cnt > 1) {
			const auto c_itr = itr+1;
			if(*(c_itr) == ' ') {
				itr = c_itr;
			} else {
				str = str.replace(itr - cnt, ++itr, " ");
				itr = str.begin();
			}
		} else {
			itr++;
		}
	}

	if(*itr == ' ') {
		str = str.substr(1);
	}

	if(str.back() == ' ') {
		str = str.substr(0, str.size()-1);
	}

	size_t delim = str.find(":");
	if(delim != str.npos) {
		if(str[delim-1] == ' ') {
			str = str.substr(0, delim-1) + str.substr(delim);
		}
	}
}

bool Assemble::isLineLegal(std::string& codeLine) {
	size_t commandIndex = codeLine.find(' ');
	std::string command = codeLine.substr(0, commandIndex);
	Node* node = new Node();

	if(codeLine[commandIndex - 1] == ':') {
		{
			const auto end = command.end();
			command.replace(end-1, end, "");
		}

		isCollision(command, node);
		size_t stop = codeLine.find(' ', commandIndex + 1);
		command = codeLine.substr(commandIndex + 1, stop - commandIndex - 1);
		commandIndex = stop;
	}

	// largest size of vector: 2
	const std::vector<char> accepts = StrMap::keywordMap[command];

	if(accepts.size() < 1) { // command not found
		return 0;
	}

	if(accepts[0] == 1) {
		node->isGo2 = 1;
		node->specifier = codeLine.substr(commandIndex + 1);
		node->instruction = KeywordMap::keywordMap[command];

		if(code.size() > 0) {
			code.back()->right = node;
		}

		code.emplace_back(node);
		isCollision(node->specifier, node);
		return 1;
	}

	const size_t size = codeLine.size();
	const char back = std::tolower(codeLine.back());
	for(char ii : accepts) {
		if(back == ii) { // specifier found
			node->specifier = codeLine.substr(commandIndex + 1);
			node->instruction = KeywordMap::keywordMap[command];

			if(code.size() > 0) {
				code.back()->right = node;
			}

			code.emplace_back(node);
			return 1;
		}
	}

	return 0; // specifier not found
}

Graph* Assemble::getOrder(void) { return order; }

void Assemble::throwError(const Errors& error, const std::string& codeLine,
	const size_t& lineNum) const {
	std::string errorMsg;

	switch(error) {
		case Errors::CommandNotFound:
			errorMsg = "Command Not legal:\n" + codeLine + "\nLine: "
				+ std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::FileNotOpen:
			errorMsg = "The file could be opened\n" + codeLine +
				std::to_string(lineNum);
			throw errorMsg;
			break;
	}
}
