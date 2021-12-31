#include "..\pch.h"
#include "Runtime.hpp"

int Runtime::findInt(const std::string& str) const {
	const size_t index = str.find(',');
	const std::string number = str.substr(0, index);
	std::stringstream str2Int;
	int num;

	if(number.substr(0, 2) == "0x") {
		str2Int << std::hex << number;
	} else {
		str2Int << number;
	}

	str2Int >> num;
	return num;
}

void Runtime::execute(void) {
	switch(node->instruction) {
		case Key::adda:
			addAddressingMode(regesters->accumulator, node->specifier);

			if((regesters->accumulator > u_max) ||
				(regesters->accumulator < min)) {
				throwError(Rte::Overflow, line);
			}

			if(regesters->accumulator == u_max) {
				Z = 0;
				N = 1;
			} else if(regesters->accumulator > 0) {
				Z = 0;
				N = 0;
			} else if(regesters->accumulator < 0) {
				N = 1;
				Z = 0;
			} else {
				Z = 1;
				N = 0;
			}

			node = node->right;
			break;

		case Key::addx:
			addAddressingMode(regesters->index, node->specifier);

			if((regesters->index > u_max) || (regesters->index < min)) {
				throwError(Rte::Overflow, line);
			}

			if(regesters->index == u_max) {
				Z = 0;
				N = 1;
			} else if(regesters->index > 0) {
				Z = 0;
				N = 0;
			} else if(regesters->index < 0) {
				N = 1;
				Z = 0;
			} else {
				Z = 1;
				N = 0;
			}

			node = node->right;
			break;

		case Key::breq:
			if(Z)
				node = node->left;
			else
				node = node->right;
			break;

		case Key::brge:
			if((Z) || ((Z == 0) && (N == 0)))
				node = node->left;
			else
				node = node->right;
			break;

		case Key::brgt:
			if((Z == 0) && (N == 0))
				node = node->left;
			else
				node = node->right;
			break;

		case Key::brle:
			if((Z) || (N))
				node = node->left;
			else
				node = node->right;
			break;

		case Key::brlt:
			if(N)
				node = node->left;
			else
				node = node->right;
			break;

		case Key::brne:
			if(Z == 0)
				node = node->left;
			else
				node = node->right;
			break;

		case Key::cpwa: {
			int number = findInt(node->specifier);
			
			if(node->specifier.back() == 'i') {
				number = findInt(node->specifier);
			} else {
				number = memory->getData(findInt(node->specifier));
			}

			if(number == regesters->accumulator) {
				Z = 1;
				C = 1;
				N = 0;
			} else if(number < regesters->accumulator) {
				C = 1;
				Z = 0;
			} else {
				N = 1;
			}

			node = node->right;
			break;
		}

		case Key::cpwx: {
			int number;
			
			if(node->specifier.back() == 'i') {
				number = findInt(node->specifier);
			} else {
				number = memory->getData(findInt(node->specifier));
			}

			if(number == regesters->index) {
				Z = 1;
				C = 1;
			} else if(number < regesters->index) {
				C = 1;
			} else {
				N = 1;
			}

			node = node->right;
			break;
		}

		case Key::deci: {
			const size_t index = node->specifier.find(',');
			const std::string var = node->specifier.substr(0, index);
			std::stringstream ctoi;
			std::string input;
			int number;

			std::cin >> input;
			if(input.find_first_of("0123456789") != input.npos) {
				ctoi << input;
			} else {
				throwError(Rte::NonNumeric, line);
			}

			ctoi >> number;
			if((number > u_max) || (number < min)) {
				throwError(Rte::Overflow, line);
			}

			if(number < 0) {
				N = 1;
				Z = 0;
			}

			if(number == u_max) {
				N = 0;
				Z = 0;
				number = -1;
			}

			const size_t value = (node->specifier.back() == 'x') ?
				regesters->index : vars[var]->getIndex();
  
			if(vars.find(var) != vars.end()) { // if var name exist
				if(value < 0) { // arrays can't have negative index
					throwError(Rte::OutOfRange, line);
				}

				memory->insert(vars[var]->getIndex()+value, number);
			} else {
				/*
				* if the var name does not exist, then direct addressing
				* was used and insert number at the memory address
				*/
				memory->insert(findInt(var), number);
			}

			node = node->right;
			break;
		}

		case Key::deco: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) != vars.end()) {
				const size_t value = (node->specifier.back() == 'x') ?
					regesters->index : 0;

				std::cout << memory->getData(vars[varName]->getIndex()+value);
			} else {
				std::cout << memory->getData(findInt(varName));
			}

			node = node->right;
			break;
		}

		default: // ONLY FOR TESTING. If there is no case for a line of code in the file.
			node = node->right;
			break;
	}
}

void Runtime::run(void) {
	const Key end = Key::end;
	const Key stop = Key::stop;

	while((node->instruction != end) && (node->instruction != stop)) {
		if(node->instruction == Key::br) {
			node = node->left;
		} else {
			execute();
		}
		line++;
	}
}