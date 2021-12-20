#pragma once

class DataType {
private:
	std::string name;
	std::string type;
	std::vector<int> bits;
	bool isString = 0;
	bool isConst = 0; // size cannot change

public:
	DataType(const std::string& name, const std::string& type) {
		this->name = name;
		this->type = type;
	}

	~DataType(void) = default;

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add each char to each index in bits vector.
	*/
	void defineType(const std::string& data);

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add zeros to each index in bits vector.
	*/
	void defineType(const int& data);

	/*
	* @returns The instance's data type
	*/
	std::string getType(void);
};