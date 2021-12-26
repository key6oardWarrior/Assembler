#pragma once

class DataType {
private:
	std::string name;
	std::string type;
	std::vector<Queue*> bits;
	bool isString = 0;
	bool isConst = 0; // size cannot change if true
	bool initStatus = 0; // if Queue has been init

public:
	DataType(const std::string& name, const std::string& type) {
		this->name = name;
		this->type = type;
	}

	~DataType(void) = default;

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add each char/int to each index in bits vector.
	*/
	void defineType(const std::string&);

	/*
	* Put all needed bits in the vector to define the type
	*
	* @param data - add zeros to each index in bits vector.
	*/
	void defineType(const int&);

	/*
	* Modify the bits vector
	*
	* @param bit - The bit to be inserted into the bits vector
	* @param index - 1st dimension index of array
	*/
	void modify(const bool& bit, const size_t& index) const {
		bits[index]->enqueue(bit);
	}

	/*
	* @returns the bits, number, or char Queue
	*/
	Queue* getInstance(const int&);

	/*
	* @returns The instance's data type
	*/
	std::string getType(void);

	bool is_string(void);
	bool is_const(void);

	/*
	* @returns the size bits vector
	*/
	size_t size(void) const;
};