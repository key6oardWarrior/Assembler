#include "pch.h"
#include "Assemble.hpp"

int main(void) {
	std::cout << "Enter assembly file: ";

	std::string file;
	std::getline(std::cin, file);

	std::fstream io = std::fstream(file);
	Assemble assembled = Assemble(io);

	char answer;
	do {
		std::cout << "Assemble (a), or Run (r): ";
		std::cin >> answer;
		answer = std::tolower(answer);

		if(answer == 'a') {
			// assembled.assemble();
		} else if(answer == 'r') {
			// assembled.assemble();
			// Execute execute = Execute(assembled);
			// execute.run();
		}

		std::cout << std::endl;
	} while((answer != 'a') && (answer != 'r'));

	return 0;
}