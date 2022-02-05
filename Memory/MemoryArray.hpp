#pragma once

struct HexValue {
	HexValue* next = NULL;
	std::string byte = "00";

	HexValue(const std::string& byte) { this->byte = byte; }
	HexValue(void) = default;
	~HexValue(void) = default;

	/*
	* @returns The signed base 10 value of byte + next->byte
	*/
	short getInt(void);
};

class MemoryArray {
private:
	// key = index, value = HexValue
	std::map<size_t, HexValue**> memoryMap;
	HexValue* first = new HexValue();

	/*
	* Convert int from base 10 to base 16
	* 
	* @param data - data to be converted
	*/
	std::string baseConverter(const short&) const; 

public:
	MemoryArray(void);
	~MemoryArray(void);

	/*
	* Insert data at a give index
	*
	* @param index - Where to insert data
	* @param data - The data to put into
	*/
	void insert(const size_t&, const int& data=0);

	/*
	* @returns The data at the give index
	*/
	int getData(const size_t&);
};