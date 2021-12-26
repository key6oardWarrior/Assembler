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
			
			if(number < 0) {
				number = abs(number);
			} else {
				number = 0 - number;
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

			if((number < 0) || (number == u_max)) {
				N = 1;
				Z = 0;
			}

			const size_t value = (node->specifier.back() == 'x') ?
				regesters->index : 0;
			if(vars.find(var) != vars.end()) {
				if(vars[var]->is_const()) {
					throwError(Rte::NotModifiable, line);
				}

				number = abs(number);

				if(value < 0) {
					throwError(Rte::OutOfRange, line);
				} else if(value > vars.size()) {
					throwError(Rte::OutOfRange, line);
				}

				if(vars[var]->getType() != ".word") {
					while(number >= 2) {
						// put the bit into the bits vector

						vars[var]->modify(number%2, value);
						number /= 2;
					}

					vars[var]->modify(number, value);
				} else {
					vars[var]->modify(number, 0);
				}
			} else {
				memory[findInt(var)] = number;
			}

			if(N == 1) {
				vars[var]->getInstance(value)->negate();
			}
			node = node->right;
			break;
		}

		case Key::deco:
			const size_t index = node->specifier.find(',');
			const std::string varName = node->specifier.substr(0, index);

			if(vars.find(varName) != vars.end()) {
				const size_t value = (node->specifier.back() == 'x') ?
					regesters->index : 0;

				if(vars.size() >= value) {
					std::cout << vars[varName]->getInstance(value)->getValue();
				} else {
					std::cout << memory[value];
				}
			} else {
				std::cout << memory[findInt(varName)];
			}

			node = node->right;
			break;
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
		}
		line++;
	}
}