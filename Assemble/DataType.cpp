#include "..\pch.h"

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

void DataType::modify(const int& index, const int& data, const bool& isSame) {
	if(isSame == 0) {
		for(auto ii = bits.begin(); ii != bits.end(); ii++) {
			*ii = 0;
		}
	}

	bits[index-1] = bits[index];
	bits[index] = data;
}

std::string DataType::getType(void) { return type; }
int DataType::size(void) const { return bits.size(); }
bool DataType::is_const(void) { return isConst; }
bool DataType::is_string(void) { return isString; }