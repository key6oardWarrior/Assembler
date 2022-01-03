#include "..\pch.h"
#include "Runtime.hpp"

short Runtime::findInt(const std::string& str) const {
	const size_t index = str.find(',');
	const std::string number = str.substr(0, index);
	std::stringstream str2Int;
	short num;

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
		case Key::adda: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) != vars.end()) { // if data to store is in var
				if(node->specifier.back() == 'd') { // direct addressing or not
					regesters->accumulator += memory->getData(vars[varName]->getIndex());
				} else {
					const int value = vars[varName]->getIndex() + regesters->index;

					if(value < 0) {
						throwError(Rte::OutOfRange, line);
					}
					regesters->accumulator += memory->getData(value);
				}
			} else {
				short value;
				std::stringstream strtoi(varName);
				strtoi >> value;

				if(node->specifier.back() == 'i') { // indirect addressing
					regesters->accumulator += value;
				} else {
					regesters->accumulator += memory->getData(value);
				}
			}

			if((regesters->accumulator > u_max) ||
				(regesters->accumulator < min)) {
				throwError(Rte::Overflow, line);
			}

			// set flags
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
		}
		break;

		case Key::addx: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) != vars.end()) { // if data to store is in var
				if(node->specifier.back() == 'd') { // direct addressing or not
					regesters->index += memory->getData(vars[varName]->getIndex());
				} else {
					const int value = vars[varName]->getIndex() + regesters->index;

					if(value < 0) {
						throwError(Rte::OutOfRange, line);
					}
					regesters->index += memory->getData(value);
				}
			} else {
				short value;
				std::stringstream strtoi(varName);
				strtoi >> value;

				if(node->specifier.back() == 'i') { // indirect addressing
					regesters->index += value;
				} else {
					regesters->index += memory->getData(value);
				}
			}

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
		}
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
		}
		break;

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
		}
		break;

		case Key::deci: {
			const size_t index = node->specifier.find(',');
			const std::string var = node->specifier.substr(0, index);
			std::stringstream ctoi;
			std::string input;
			int number;

			// ensure user input is clean
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
		}
		break;

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
		}
		break;

		case Key::ldwa:{
			const char backChar = node->specifier.back();

			if(backChar == 'i') {
				regesters->accumulator = findInt(node->specifier);
			} else {
				const size_t index = node->specifier.find(',');
				const std::string varName = node->specifier.substr(0, index);

				// check if value is a literal or stored in memory
				if(vars.find(varName) == vars.end()) {
					regesters->accumulator = memory->getData(findInt(varName));
				} else {
					if(backChar == 'd') {
						regesters->accumulator =
							memory->getData(vars[varName]->getIndex());
					} else {
						const int valueIndex = vars[varName]->getIndex() +
							regesters->accumulator;

						if(valueIndex < 0) {
							throwError(RuntimeErrors::OutOfRange, line);
						}
						regesters->accumulator = memory->getData(valueIndex);
					}
				}
			}
		}
		break;

		case Key::ldwx: {
			const char backChar = node->specifier.back();

			if(backChar == 'i') {
				regesters->index = findInt(node->specifier);
			} else {
				const size_t index = node->specifier.find(',');
				const std::string varName = node->specifier.substr(0, index);

				if(vars.find(varName) == vars.end()) {
					regesters->index = memory->getData(findInt(varName));
				} else {
					if(backChar == 'd') {
						regesters->index =
							memory->getData(vars[varName]->getIndex());
					} else {
						const int valueIndex = vars[varName]->getIndex() +
							regesters->index;

						if(valueIndex < 0) {
							throwError(RuntimeErrors::OutOfRange, line);
						}
						regesters->index = memory->getData(valueIndex);
					}
				}
			}
		}
		break;

		case Key::nega:
			regesters->accumulator = 0 - regesters->accumulator;
			break;

		case Key::negx:
			regesters->index = 0 - regesters->index;
			break;

		case Key::stro: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) != vars.end()) {
				if(node->specifier.back() == 'd') {
					std::cout << vars[varName]->getStr();
				} else {
					std::cout << vars[varName]->getStr().substr(regesters->index, 1);
				}
			} else {
				std::cout << char(memory->getData(findInt(varName)));
			}
		}
		break;

		case Key::stwa: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) == vars.end()) {
				const int number = regesters->accumulator;
				memory->insert(findInt(varName), number);
			} else {
				const int number = regesters->accumulator;
				const size_t valueIndex = (node->specifier.back() == 'x') ?
					regesters->index : vars[varName]->getIndex();
				memory->insert(valueIndex, number);
			}
		}
		break;

		case Key::stwx: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) == vars.end()) {
				const int number = regesters->index;
				memory->insert(findInt(varName), number);
			} else {
				const int number = regesters->index;
				const size_t valueIndex = (node->specifier.back() == 'x') ?
					regesters->index : vars[varName]->getIndex();
				memory->insert(valueIndex, number);
			}
		}
		break;

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