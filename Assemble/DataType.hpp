#pragma once

class DataType {
private:
	std::string name;
	std::string type;
	std::string str;
	size_t index;
	size_t size;

public:
	bool isConst = 0; // size cannot change if true

	DataType(void) = default;
	DataType(const std::string& name, const std::string& type,
		const bool& isConst) {
		
		this->type = type;
		this->name = name;
		this->isConst = isConst;
	}

	~DataType(void) = default;

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add each char/int to each index in bits vector.
	*/
	void defineType(const std::string& str) { this->str = str; }

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param index - where the data's high byte is stored
	*/
	void defineType(const size_t& index, const size_t& size) {
		this->index = index;
		this->size = size;
	}

	const size_t& getSize(void);
	const bool& is_const(void);
	const size_t& getIndex(void);
	const std::string& getStr(void);
	const std::string& getType(void);
};