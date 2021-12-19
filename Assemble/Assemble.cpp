#include "..\pch.h"

void Assemble::removeBadSpacing(std::string& str) const {
	auto itr = str.begin();
	const auto begining = itr;
	int cnt = 0;
	size_t index = 0;

	while(itr != str.end()) {
		if(*itr == ' ') {
			cnt++;
		} else {
			cnt = 0;
		}

		if(cnt > 1) {
			if(*(itr+1) != ' ') {
				str = str.replace(itr-cnt, ++itr, " ");
				index -= cnt;
				itr -= cnt;
				cnt = 0;
			}
		}

		itr++;
		index++;
	}

	if(str[0] == ' ') {
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
				throwError(Errors::Undefined, itr->second->specifier, 1);
			}
		}
		itr++;
	}
}

void Assemble::isCollision(const std::string& str, Node* node) {
	if(brMap.find(str) != brMap.end()) {
		if(brMap[str]->isGo2) {
			if(brMap[str]->left == NULL) {
				brMap[str]->left = node;
			} else {
				throwError(Errors::AlreadyDefined, str, brMap.size() + 1);
			}
		} else {
			if(node->left == NULL) {
				node->left = brMap[str];
			} else {
				throwError(Errors::AlreadyDefined, str, brMap.size() + 1);
			}
		}
	} else { // add node to the map
		brMap.insert(std::pair<std::string, Node*>(str, node));
	}
}

void Assemble::assembleCode(void) {
	std::string fileCode;
	size_t line = 0;

	if(file.is_open() == 0) {
		throwError(Errors::FileNotOpen, "", 1);
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
			throwError(Errors::CommandNotFound, fileCode, line);
		}

		if(fileCode == ".end") {
			checkGraph();
			file.close();
			return;
		}
	}

	file.close();
	if(fileCode != ".end") {
		throwError(Errors::MissingEnd, fileCode, line);
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
					throwError(Errors::NonNumeric, memorySize, brMap.size()+1);
				}

				if(memorySize.size() > 4) {
					throwError(Errors::NonNumeric, memorySize, brMap.size()+1);
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
		throwError(Errors::Overflow, memorySize, brMap.size()+1);
	}
	return number;
}

void Assemble::declareVars(const std::string& declaration, const size_t& index)
{
	const size_t space = declaration.rfind(' ');
	const std::string memoryType = declaration.substr(index+1, space-(index+1));
	const std::string memorySize = declaration.substr(space+1);
	const std::string varName = declaration.substr(0, index-1);
	const int number = (memoryType != ".ascii") ? findInt(memorySize) : 0;

	if(KeywordMap::keywordMap.find(memoryType) != KeywordMap::keywordMap.end())
	{
		DataType type = DataType(varName, memoryType);

		if(memoryType == ".block") {
			if(number < 2) {
				throwError(Errors::NotEnoughBytes, declaration, brMap.size()+1);
			}
			type.defineType(number);
			vars.insert(std::pair<std::string, DataType>(varName, type));

		} else if(memoryType == ".equate") {
			type.defineType(number);
			vars.insert(std::pair<std::string, DataType>(varName, type));

		} else if(memoryType == ".word") {
			lastVar = varName;
			isArr = 1;
			const auto end = vars.find(varName);

			if(end != vars.end()) {
				end->second.defineType(number);
			} else {
				type.defineType(number);
				vars.insert(std::pair<std::string, DataType>(varName, type));
			}

		} else if(memoryType == ".ascii") {
			if(isArr == 0) {
				type.defineType(memorySize);
				vars.insert(std::pair<std::string, DataType>(varName, type));
			} else {
				isArr = 0;
			}
		} else {
			throwError(Errors::CommandNotFound, declaration, brMap.size()+1);
		}

	} else {
 		throwError(Errors::CommandNotFound, declaration, brMap.size()+1);
	}
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

	if(codeLine[0] == '.') {
		delete node;
		node = nullptr;

		if(isArr) {
			if(lastVar != "") {
				codeLine = lastVar + ": " + codeLine;
				commandIndex = codeLine.find(' ');
			}
			declareVars(codeLine, commandIndex);
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

void Assemble::throwError(const Errors& error, const std::string& codeLine,
	const size_t& lineNum) const {
	std::string errorMsg;

	switch(error) {
		case Errors::CommandNotFound:
			errorMsg = "Command Not legal:\n" + codeLine + "\nLine: " +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::FileNotOpen:
			errorMsg = "The file could be opened\n" + codeLine +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::NonNumeric:
			errorMsg = "The value: " + codeLine + " is not a numeric type\
				\nLine: " + std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::Overflow:
			errorMsg = "Out of range of values -32768 and 65535: " + codeLine +
				" \nLine" + std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::MissingEnd:
			errorMsg = "Missing .END statement. \nLine: " +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::AlreadyDefined:
			errorMsg = "Symbol " + codeLine + " already defined\nLine" +
				std::to_string(lineNum);
			throw errorMsg;
			break;

		case Errors::Undefined:
			errorMsg = "Symbol " + codeLine + " undefined";
			throw errorMsg;
			break;
	}
}
