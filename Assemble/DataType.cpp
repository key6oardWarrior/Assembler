#include "..\pch.h"

void DataType::defineType(const std::string& str) {
	isString = 1;
	auto itr = str.begin()+1;

	while(itr != str.end()) {
		if(initStatus) {
			bits[0]->enqueue(*itr++);
		} else {
			bits.emplace_back(new Queue());
			initStatus = 1;
		}
	}
}

void DataType::defineType(const int& number) {
	if(type == ".word") {
		if(initStatus) {
			bits[0]->enqueue(number);
		} else {
			bits.emplace_back(new Queue());
			initStatus = 1;
		}

		isConst = 1;
	} else {
		for(int ii = 0; ii < number; ii++) {
			bits.emplace_back(new Queue());
		}

		if(type == ".equate") {
			isConst = 1;
		}
	}
}

Queue* DataType::getInstance(const int& index) { return bits[index]; }
std::string DataType::getType(void) { return type; }
size_t DataType::size(void) const { return bits.size(); }
bool DataType::is_const(void) { return isConst; }
bool DataType::is_string(void) { return isString; }