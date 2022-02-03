#include "..\pch.h"

MemoryArray::MemoryArray(void) {
	size_t ii = 0;
	HexValue** current = std::addressof(first);

	while(ii <= u_max) {
		memoryMap.insert(std::pair<size_t, HexValue**>(ii, current));
		ii++;
		(*current)->next = new HexValue();
		current = std::addressof((*current)->next);
	}
}

MemoryArray::~MemoryArray(void) {
	HexValue* current = first;

	while(current != NULL) {
		HexValue* next = current->next;
		delete current;

		current = next;
	}

	first = nullptr;
}

std::string MemoryArray::baseConverter(const int& data) const {
	std::stringstream bytes;
	bytes << std::hex << data;
	std::string hex = bytes.str();
	hex = std::string(4-hex.size(), '0') + hex;

	return hex;
}

void MemoryArray::insert(const size_t& index, const int& data) {
	std::string hex = baseConverter(data);
	hex = std::string(4-hex.size(), '0') + hex; // if hex smaller than 4 bits

	(*memoryMap[index])->byte = hex.substr(0, 2);
	(*memoryMap[index])->next->byte = hex.substr(2);
}

int MemoryArray::getData(const size_t& index) {
	return (*memoryMap[index])->getInt();
}

short HexValue::getInt() {
	std::stringstream bytes;
	unsigned int uint;

	bytes << std::hex << HexValue::byte << HexValue::next->byte;
	bytes >> uint;

	return uint;
}