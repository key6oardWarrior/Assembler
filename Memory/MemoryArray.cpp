#include "..\pch.h"

MemoryArray::MemoryArray(void) {
	size_t ii = 0;

	while(ii <= u_max) {
		memoryMap.insert(std::pair<size_t, HexValue*>(ii, new HexValue()));
		ii++;
	}
}

MemoryArray::~MemoryArray(void) {
	auto itr = memoryMap.begin();

	while(itr != memoryMap.end()) {
		delete itr->second;
		itr->second = nullptr;
	}
}

std::string MemoryArray::baseConverter(const int& data) const {
	std::stringstream bytes;
	bytes << std::hex << data;
	std::string hex = bytes.str();
	hex += std::string(4-hex.size(), '0');

	return hex;
}

void MemoryArray::insert(const size_t& index, const int& data) {
	std::string hex = baseConverter(data);
	hex += std::string(4-hex.size(), '0');

	memoryMap[index] = new HexValue(hex.substr(0, 2));
	memoryMap[index+1] = new HexValue(hex.substr(2));
}

int MemoryArray::getData(const size_t& index) {
	return memoryMap[index]->getInt();
}

short HexValue::getInt(void) {
	std::stringstream bytes;
	unsigned int uint;

	bytes << std::hex << byte << next->byte;
	bytes >> uint;

	return uint;
}