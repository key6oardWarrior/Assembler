#include "..\pch.h"
#include "Queue.hpp"

Queue::~Queue(void) {
	LinkedList* current = top;

	while(current != NULL) {
		LinkedList* next = current->next;
		delete current;
		current = next;
	}
}

void Queue::negate(void) {
	if(size == 0) {
		return;
	}

	LinkedList* current = top;

	int diff = size - 16;
	while(diff < 0) {
		enqueue(false);
		diff++;
	}

	while(current != NULL) { // 1's complement
		current->num = !(current->num);
		current = current->next;
	}

	current = top;
	bool carry = 1;
	while(current != NULL) { // 2's complement
		switch(carry) {
			case 1:
				current->num = !(current->num);

				switch(current->num) {
					case 1:
						current = NULL;
						break;
					default:
						current = current->next;
						break;
				}

				break;

			default:
				current = NULL;
				break;
		}
	}

	isNeg = 1;
}

void Queue::enqueue(const int& value) {
	if(size == 0) {
		top = new LinkedList(value);
		bottom = top;
	} else {
		bottom->next = new LinkedList(value);
		bottom->next->prev = bottom;
		bottom = bottom->next;
	}

	size++;
}

void Queue::enqueue(const char& data) {
	if(size == 0) {
		top = new LinkedList(data);
		bottom = top;
	} else {
		bottom->next = new LinkedList(data);
		bottom->next->prev = bottom;
		bottom = bottom->next;
	}

	size++;
}

std::string Queue::getString(void) const {
	if(size == 0) {
		return "";
	}

	LinkedList* current = top;
	std::string str;

	while(current != NULL) {
		str += current->data;
		current = current->next;
	}

	delete current;
	current = nullptr;

	return str;
}

int Queue::getValue(void) const {
	if(size == 0) {
		return 0;
	}

	LinkedList* current = top;
	size_t expon = 1;
	int value = (isNeg) ? -65536 : 0;

	while(current != NULL) {
		if(current->num == 1) {
			value += expon;
		}

		expon *= 2;
		current = current->next;
	}

	return value;
}

size_t Queue::getSize(void) { return size; }