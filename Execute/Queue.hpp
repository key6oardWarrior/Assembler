#pragma once

struct LinkedList {
	LinkedList* next = NULL;
	LinkedList* prev = NULL;
	char data;
	int num;

	LinkedList(const char& data) { this->data = data; }
	LinkedList(const int& num) { this->num = num; }

	~LinkedList(void) = default;
};

class Queue {
private:
	LinkedList* top = NULL;
	LinkedList* bottom = NULL;
	bool isNeg = 0;
	size_t size = 0;

public:
	Queue(void) = default;
	~Queue(void);

	/*
	* Add elements to the Queue
	*
	* @param value - the int that will be added
	*/
	void enqueue(const int&);

	/*
	* Add elements to the Queue
	* 
	* @param data - the char that will be added
	*/
	void enqueue(const char&);
	
	/*
	* Find the 2's complement
	*/
	void negate(void);

	/*
	* @returns the base 10 value of the elements in the Queue
	*/
	int getValue(void) const;

	/*
	* @returns the string that is stored in the Queue
	*/
	std::string getString(void) const;

	/*
	* @returns the size of the Queue
	*/
	size_t getSize(void);
};