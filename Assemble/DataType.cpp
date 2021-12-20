#include "..\pch.h"

void DataType::defineType(const std::string& data) {
	isString = 1;

	for(char ii : data) {
		bits.emplace_back(ii);
	}
}

void DataType::defineType(const int& data) {
	if(type != ".word") {
		for(int ii = 0; ii < data; ii++) {
			bits.emplace_back(0);
		}
	} else { // aka is an array
		isConst = 1;
		bits.emplace_back(data);
	}
}

std::string DataType::getType(void) { return type; }