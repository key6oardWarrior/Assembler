#include "Assemble.hpp"

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
				itr = begining + index;
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
			file.close();
			return;
		}
	}

	file.close();
	if(fileCode != ".end") {
		throwError(Errors::MissingEnd, fileCode, line);
	}
}

int Assemble::findInt(const std::string& memorySize) const {
	std::stringstream str2int;
	int number;

	if(memorySize.substr(0, 2) == "0x") {
		str2int << std::hex << memorySize;
	} else {
		auto itr = memorySize.begin();
		while(itr != memorySize.end()) {
			if(isdigit(*itr) == 0)
				throwError(Errors::NonNumeric, memorySize, brMap.size()+1);
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
	const std::string varName = declaration.substr(0, index-2);
	const int number = findInt(memorySize);

	if(memoryType == ".block") {
		if(number < 1) {
			throwError(Errors::NotEnoughBytes, declaration, brMap.size()+1);
		}
	}

	if(KeywordMap::keywordMap.find(memoryType) != KeywordMap::keywordMap.end())
	{
		vars.insert(std::pair<std::string, int>(varName, number));
	} else {
		throwError(Errors::CommandNotFound, declaration, brMap.size()+1);
	}
}

bool Assemble::isLineLegal(std::string& codeLine) {
	size_t commandIndex = codeLine.find(' ');
	std::string command = codeLine.substr(0, commandIndex);
	Node* node = new Node();

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
	}
}
