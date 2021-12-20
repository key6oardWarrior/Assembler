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

void Runtime::execute(Node*& node) {
	switch(node->instruction) {
		case Key::adda:
			addAddressingMode(regesters->accumulator, node->specifier);

			if((regesters->accumulator > u_max) ||
				(regesters->accumulator < min)) {
				throwError(Rte::Overflow);
			}

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
			break;

		case Key::addx:
			addAddressingMode(regesters->index, node->specifier);

			if((regesters->index > u_max) || (regesters->index < min)) {
				throwError(Rte::Overflow);
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
			
			if(number < 0) {
				number = abs(number);
			} else {
				number = 0 - number;
			}

			if(number == regesters->accumulator) {
				Z = 1;
				C = 1;
			} else if(number < regesters->accumulator) {
				C = 1;
			} else {
				N = 1;
			}
			break;
		}

		case Key::cpwx: {
			int number;
			
			if(node->specifier[-1] == 'i') {
				number = findInt(node->specifier);
			} else {
				number = memory[findInt(node->specifier)];
			}

			if(number < 0) {
				number = abs(number);
			} else {
				number = 0 - number;
			}

			if(number == regesters->index) {
				Z = 1;
				C = 1;
			} else if(number < regesters->index) {
				C = 1;
			} else {
				N = 1;
			}
			break;
		}

		case Key::deci: {
			const size_t index = node->specifier.find(',');
			const std::string var = node->specifier.substr(0, index);
			std::stringstream ctoi;
			char input;
			int number;

			std::cin >> input;
			if(isdigit(input)) {
				ctoi << input;
			} else {
				throwError(Rte::NonNumeric);
			}

			ctoi >> number;
			if((number > u_max) || (number < min)) {
				throwError(Rte::Overflow);
			}

			if(vars.find(var) != vars.end()) {
				if(vars[var]->is_const()) {
					throwError(Rte::NotModifiable);
				}

				size_t itr = vars[var]->size()-1;
				bool isReset = 0;

				while(number >= 2) {
					// put the bit into the bits vector
					vars[var]->modify(itr, number % 2, isReset);
					number /= 2;
					itr--;
					isReset = 1;
				}
			} else {
				memory[findInt(var)] = number;
			}
		}
	}
}

void Runtime::run(void) {
	Node* node = order->getRoot();
	const Key end = Key::end;
	const Key stop = Key::stop;

	while((node->instruction != end) && (node->instruction != stop)) {
		if(node->instruction == Key::br) {
			node = node->left;
		} else {
			execute(node);

			if(node->isGo2) {
				node = node->right;
			}
		}
		line++;
	}
}

void Runtime::throwError(const Rte& rte) const {
	std::string errorMsg;

	switch(rte) {
		case Rte::Overflow:
			errorMsg = "Overflow on line: " + std::to_string(line);
			throw errorMsg;
			break;

		case Rte::NonNumeric:
			errorMsg = "The value entered is not a number. Line: " +
				std::to_string(line);
			throw errorMsg;
			break;

		case Rte::NotModifiable:
			errorMsg = "The variable you are attemping to modify is not\
		modifiable. Line: " + std::to_string(line);
			throw errorMsg;
			break;
	}
}