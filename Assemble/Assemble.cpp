#include "..\pch.h"

void Assemble::removeBadSpacing(std::string& str) const {
	auto itr = str.begin();
	const auto begining = itr;
	int cnt = 0;
	size_t index = 0;

	while(itr != str.end()) {
		if((*itr == '"') || (*itr == '\'') || (*itr == ';')) {
			break;
		}

		if(*itr == ' ') {
			cnt++;
		} else {
			cnt = 0;
		}

		if((cnt > 1) || (*itr == '\t')) {
			const auto c_itr = itr + 1;
			if((*c_itr != ' ') && (*c_itr != '\t')) {
				str = str.replace(itr-cnt, c_itr, " ");
				index -= cnt;
				itr -= (*itr != str[0]) ? ++cnt : 0;
				cnt = 0;
			}
		} else {
			itr++;
		}
		index++;
	}

	size_t delim = str.find(";");
	if(delim != str.npos) {
		str = str.substr(0, delim);
	}

	if(str[0] == ' ') {
		str = str.substr(1);
	} else if(str[0] == '\t') {
		str = str.substr(1);
	}

	const char back = str.back();
	if(back == '\t') {
		str = str.substr(0, str.size()-1);
	} else if(back == ' ') {
		str = str.substr(0, str.size()-1);
	}

	delim = str.find(":");
	if(delim != str.npos) {
		if(str[delim-1] == ' ') {
			str = str.substr(0, delim-1) + str.substr(delim);
		}
	}

	delim = str.find(",");
	if(delim != str.npos) {
		if(str[delim-1] == ' ') {
			str = str.substr(0, delim-1) + str.substr(delim);
		}
	}

	delim = str.find(".");
	if(delim != str.npos) {
		if(str[delim+1] == ' ') {
			str = str.substr(0, delim+1) + str.substr(delim+2);
		}
	}

	delim = str.find("\\");
	if(delim != str.npos) {
		str = str.substr(0, delim) + "\"";
	}
}

void Assemble::checkGraph(void) const {
	auto itr = brMap.begin();

	while(itr != brMap.end()) {
		if(itr->second->isGo2) {
			if(itr->second->left == NULL) {
				throwError(AssembleErrors::Undefined, itr->second->specifier, 1);
			}
		}
		itr++;
	}
}

void Assemble::isCollision(const std::string& str, Node*& node) {
	if(brMap.find(str) != brMap.end()) {
		if(brMap[str]->isGo2) {
			if(brMap[str]->left == NULL) {
				brMap[str]->left = node;
			} else {
				if(node->specifier == "") {
					throwError(AssembleErrors::AlreadyDefined, str, brMap.size()+1);
				}
				node->left = brMap[str]->left;
			}
		} else {
			node->left = brMap[str];
		}
	} else { // add node to the map
		brMap.insert(std::pair<std::string, Node*>(str, node));
	}
}

void Assemble::assembleCode(void) {
	std::string fileCode;
	size_t line = 0;

	if(file.is_open() == 0) {
		throwError(AssembleErrors::FileNotOpen, "", 1);
	}

	while(file.eof() == 0) {
		std::getline(file, fileCode);
		line++;

		// ignore whitespace and comments
		if((fileCode == "") || (fileCode == "\n") || (fileCode == "\t") ||
			(fileCode == " ") || (fileCode[0] == ';')) {
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
			throwError(AssembleErrors::CommandNotFound, fileCode, line);
		}

		if(fileCode == ".end") {
			checkGraph();
			file.close();
			return;
		}
	}

	file.close();
	if(fileCode != ".end") {
		throwError(AssembleErrors::MissingEnd, fileCode, line);
	}
	checkGraph();
}

int Assemble::findInt(const std::string& memorySize) const {
	std::stringstream str2int;
	int number;

	if(memorySize.substr(0, 2) == "0x") {
		str2int << std::hex << memorySize;
	} else {
		auto itr = memorySize.begin();
		const auto end = memorySize.end();

		while(itr != end) {
			if(isdigit(*itr) == 0) {
				if((*itr != '\'') && (*itr != '"')) {
					throwError(AssembleErrors::NonNumeric, memorySize, brMap.size()+1);
				}

				if(memorySize.size() > 4) {
					throwError(AssembleErrors::NonNumeric, memorySize, brMap.size()+1);
				}

				number = memorySize[1];			
				if((memorySize[2] != '"') && (memorySize[2] != '\'')) {
					number += memorySize[2];
				}

				return number;
			}
			itr++;
		}

		str2int << memorySize;
	}

	str2int >> number;
	if((number > u_max) || (number < min)) {
		throwError(AssembleErrors::Overflow, memorySize, brMap.size()+1);
	}
	return number;
}

void Assemble::declareVars(const std::string& declaration, const size_t& index)
{
	DataType* type;
	const size_t space = declaration.rfind(' ');
	const std::string varName = declaration.substr(0, index-1);
	const std::string memoryType = declaration.substr(index+1, space-(index+1));

	if(declaration.find(".ascii ") != declaration.npos) {
		type = new DataType();
		const size_t start = declaration.find_first_of("\"");

		type->defineType(declaration.substr(start));
		vars.insert(std::pair<std::string, DataType*>(varName, type));
		isArr = 0;

	} else if((declaration.find(".word ")) != declaration.npos) {
		if(lastVar == "") {
			type = new DataType(varName, memoryType, 1);
			type->defineType(this->index, 1);
			vars.insert(std::pair<std::string, DataType*>(varName, type));
			lastVar = varName;
		}

		memory->insert(this->index, findInt(declaration.substr(space+1)));
		isArr = 1;

	} else if(KeywordMap::keywordMap.find(memoryType) !=
		KeywordMap::keywordMap.end()) {

		isArr = 0;
		if(memoryType == ".equate") {
			type = new DataType(varName, memoryType, 1);
		} else {
			type = new DataType(varName, memoryType, 0);
		}

		const std::string memorySize = declaration.substr(space+1);
		const int number = findInt(memorySize);

		memory->insert(0);
		type->defineType(this->index, findInt(memorySize));
		vars.insert(std::pair<std::string, DataType*>(varName, type));
	} else {
		throwError(AssembleErrors::CommandNotFound, declaration,
			brMap.size()-1);
	}

	this->index += 2;
}

bool Assemble::isLineLegal(std::string& codeLine) {
	size_t commandIndex = codeLine.find(' ');
	std::string command = codeLine.substr(0, commandIndex);
	Node* node = new Node();

	if((codeLine == "stop") || (codeLine == ".end")) {
		node->instruction = KeywordMap::keywordMap[codeLine];
		node->specifier = codeLine;
		prev->right = node;
		prev = node;
		return 1;
	}

	if(commandIndex == codeLine.npos) {
		if(KeywordMap::keywordMap.find(codeLine) ==
			KeywordMap::keywordMap.end()) {
			return 0;
		}

		node->instruction = KeywordMap::keywordMap[codeLine];
		node->specifier = codeLine;
		prev->right = node;
		return 1;
	}

	if(codeLine[0] == '.') {
		delete node;
		node = nullptr;

		if(isArr) {
			if(lastVar != "") {
				codeLine = lastVar + ": " + codeLine;
				commandIndex = codeLine.find(' ');
			}
			declareVars(codeLine, commandIndex);
		} else {
			lastVar = "";
		}

		return 1;
	}

	isArr = 0;
	if(codeLine[commandIndex-1] == ':') {
		if(codeLine[commandIndex+1] == '.') {
			declareVars(codeLine, commandIndex);
			delete node;
			node = nullptr;
			return 1;
		}

		{
			const auto end = command.end();
			command.replace(end-1, end, "");
		}

		isCollision(command, node);
		size_t stop = codeLine.find(' ', commandIndex+1);
		command = codeLine.substr(commandIndex+1, stop-commandIndex-1);
		commandIndex = stop;
	}

	// largest possible size of vector: 2
	const std::vector<char> accepts = keywordMap[command];

	if(accepts.size() < 1) { // command not found
		return 0;
	}

	if(accepts[0] == 1) {
		node->isGo2 = 1;
		node->specifier = codeLine.substr(commandIndex+1);
		node->instruction = KeywordMap::keywordMap[command];

		if(prev != NULL) {
			prev->right = node;
		} else {
			order->setRoot(node);
		}

		prev = node;
		isCollision(node->specifier, node);
		return 1;
	}

	const size_t size = codeLine.size();
	const char back = std::tolower(codeLine.back());
	for(char ii : accepts) {
		if(back == ii) { // specifier found
			node->specifier = codeLine.substr(commandIndex + 1);
			node->instruction = KeywordMap::keywordMap[command];

			if(prev != NULL) {
				prev->right = node;
			} else {
				order->setRoot(node);
			}

			prev = node;
			return 1;
		}
	}

	return 0; // specifier not found
}

Graph* Assemble::getOrder(void) { return order; }

std::map<std::string, DataType*> Assemble::getVars(void) { return vars; }

MemoryArray* Assemble::getMemory(void) { return memory; }

void Assemble::throwError(const AssembleErrors& error, const std::string& codeLine,
	const size_t& lineNum) const {
	std::string errorMsg;

	switch(error) {
		case AssembleErrors::CommandNotFound:
			errorMsg = "Command Not legal:\n" + codeLine + "\nLine: " +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::FileNotOpen:
			errorMsg = "The file could be opened\n" + codeLine +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::NonNumeric:
			errorMsg = "The value: " + codeLine + " is not a numeric type\
				\nLine: " + std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::Overflow:
			errorMsg = "Out of range of values -32768 and 65535: " + codeLine +
				" \nLine" + std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::MissingEnd:
			errorMsg = "Missing .END statement. \nLine: " +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::AlreadyDefined:
			errorMsg = "Symbol " + codeLine + " already defined\nLine" +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case AssembleErrors::Undefined:
			errorMsg = "Symbol " + codeLine + " undefined";
			throw errorMsg;
			break;
	}
}
