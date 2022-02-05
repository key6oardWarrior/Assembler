#include "..\pch.h"
#include "Runtime.hpp"

Runtime::~Runtime(void) {
	delete order;
	order = nullptr;

	delete memory;
	memory = nullptr;

	auto itr = vars.begin();
	while(itr != vars.end()) {
		delete itr->second;
		itr->second = nullptr;
		itr++;
	}

	// no need to clean up node see Graph::~Graph()
}

short Runtime::findInt(const std::string& str) const {
	const std::string number = str.substr(0, str.find(','));
	std::stringstream str2Int;
	unsigned int uint;

	if(number.substr(0, 2) == "0x") {
		str2Int << std::hex << number;
	} else {
		str2Int << number;
	}

	str2Int >> uint;
	return uint;
}

int Runtime::addressingMode(void) {
	const char back = node->specifier.back();
	int number;

	if(back == 'i') { // indirect memory addressing
		number = findInt(node->specifier);
	} else if(back == 'd') { // direct memory addressing
		const int index = findInt(node->specifier);
		if(index < 0) {
			throwError(Rte::OutOfRange, node->line);
		}

		number = memory->getData(index);
	} else if(back == 'x') { // indexing using index regester
		const size_t index = node->specifier.find(',');
		const std::string varName = node->specifier.substr(0, index);
		int valueIndex;

		if(vars.find(varName) != vars.end()) {
			valueIndex = vars[varName]->getIndex() + regesters->index;
		} else {
			valueIndex = findInt(node->specifier);
		}

		if(valueIndex < 0) {
			throwError(Rte::OutOfRange, node->line);
		} else if(valueIndex > u_max) {
			throwError(Rte::Overflow, node->line);
		}

		number = memory->getData(valueIndex);
	} else {
		throwError(Rte::MalformedAddrMode, node->line);
	}

	return number;
}

void Runtime::execute(void) {
	switch(node->instruction) {
		case Key::adda:
			regesters->accumulator += addressingMode();

			if((regesters->accumulator > u_max) ||
				(regesters->accumulator < min)) {
				throwError(Rte::Overflow, node->line);
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
			break;

		case Key::addx:
			regesters->index += addressingMode();

			if((regesters->index > u_max) || (regesters->index < min)) {
				throwError(Rte::Overflow, node->line);
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
			int number = addressingMode();

			// set flags
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
			int number = addressingMode();

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
				throwError(Rte::NonNumeric, node->line);
			}

			ctoi >> number;
			if((number > u_max) || (number < min)) {
				throwError(Rte::Overflow, node->line);
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
					throwError(Rte::OutOfRange, node->line);
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

		case Key::ldwa:
			regesters->accumulator = addressingMode();
			node = node->right;
			break;

		case Key::ldwx:
			regesters->index = addressingMode();
			node = node->right;
			break;

		case Key::nega:
			regesters->accumulator = 0 - regesters->accumulator;
			node = node->right;
			break;

		case Key::negx:
			regesters->index = 0 - regesters->index;
			node = node->right;
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
			node = node->right;
		}
		break;

		case Key::stwa: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) == vars.end()) { // store data in memory addr
				const int number = regesters->accumulator;
				memory->insert(findInt(varName), number);
			} else { // store data in var's memory address
				const int number = regesters->accumulator;

				if(node->specifier.back() == 'x') {
					memory->insert(vars[varName]->getIndex()+regesters->index,
						number);
				} else {
					memory->insert(vars[varName]->getIndex(), number);
				}
				
			}
			node = node->right;
		}
		break;

		case Key::stwx: {
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) == vars.end()) { // store data in memory addr
				const int number = regesters->index;
				memory->insert(findInt(varName), number);
			} else { // store data in var's memory address
				const int number = regesters->index;

				if(node->specifier.back() == 'x') {
					memory->insert(vars[varName]->getIndex()+regesters->index,
						number);
				} else {
					memory->insert(vars[varName]->getIndex(), number);
				}
				
			}
		}
		break;

		case Key::suba: {
			regesters->accumulator -= addressingMode();

			if(regesters->accumulator < min) {
				throwError(Rte::Overflow, node->line);
			}

			// set flags
			if(regesters->accumulator > 0) {
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

		case Key::subx: {
			regesters->index -= addressingMode();

			if(regesters->index < min) {
				throwError(Rte::Overflow, node->line);
			}

			if(regesters->index > 0) {
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
	}
}