#pragma once

class DataType {
private:
	std::string name;
	std::string type;
	std::vector<int> bits;
	bool isString = 0;
	bool isConst = 0; // size cannot change if true

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
	void defineType(const std::string& data) {
		isString = 1;

		for(char ii : data) {
			bits.emplace_back(ii);
		}
	}

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add zeros to each index in bits vector.
	*/
	void defineType(const int& data);

	/*
	* Modify the bits vector
	* 
	* @param index - the index to input data
	* @param data - Data to be inserted into the bits vector
	* @param isSame - does the data in bits need to reset
	*/
	void modify(const int&, const int&, const bool&);

	/*
	* @returns The instance's data type
	*/
	std::string getType(void);

	bool is_string(void);
	bool is_const(void);

	/*
	* @returns the size bits vector
	*/
	int size(void) const;
};