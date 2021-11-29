#include "Assemble.hpp"
#include "Graph.hpp"

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

			removeBadSpacing(fileCode);

			if(isLineLegal(fileCode, line) == 0) {
				throwError(Errors::CommandNotFound, fileCode, line);
			}
		}
	}

	file.close();
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

		if(cnt >= 2) {
			if(*(itr+1) == ' ') {
				itr++;
			} else {
				str.replace(itr-cnt, ++itr, " ");
			}
		} else {
			itr++;
		}
	}

	if(*(str.begin()) == ' ') {
		str = str.substr(1);
	}

	if(str.back() == ' ') {
		str = str.substr(0, str.size()-1);
	}
}

void Assemble::isCollision(const std::string& str, Node* node) {
	if(brMap.find(str) != brMap.end()) {
		if(brMap[str]->isGo2) {
			brMap[str]->left = node;
		} else {
			node->left = brMap[str];
		}
	} else {
		brMap.insert(std::pair<std::string, Node*>(str, node));
	}
}

bool Assemble::isLineLegal(std::string& codeLine, const int& order) {
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
		code.insert(std::pair<int, Node*>(order, node));
		isCollision(node->specifier, node);
		return 1;
	}

	const size_t size = codeLine.size();
	const char back = std::tolower(codeLine.back());
	for(char ii : accepts) {
		if(back == ii) { // specifier found
			node->specifier = codeLine.substr(commandIndex + 1);
			node->instruction = KeywordMap::keywordMap[command];
			code.insert(std::pair<int, Node*>(order, node));
			return 1;
		}
	}

	return 0; // specifier not found
}

void Assemble::assemble(void) {
	auto itr = code.begin();
	Graph* graph = new Graph(itr->second);

	const auto end = code.end();
	while(itr != end) {
		Node* node = itr->second;
		itr++;

		if(itr != end) {
			node->right = itr->second;
		}
	}
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
