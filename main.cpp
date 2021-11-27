#include "pch.h"
#include "Assemble.hpp"

int main(void) {
	std::cout << "Enter assembly file: ";

	std::string file;
	std::getline(std::cin, file);

	std::fstream io = std::fstream(file);
	Assemble assembled = Assemble(io);

	Assemble* assembled = 0;
	char answer;
	do {
		std::cout << "\nAssemble (a), Run (r), Exit (e): ";
		std::cin >> answer;
		answer = std::tolower(answer);

		switch(answer) {
			case 'a':
				assembled = new Assemble(io);
				break;
			case 'r':
				assembled = new Assemble(io);
				// Excute execute = Excute(assembled);
				// excute.run();
				break;
			default:
				break;
		}
	} while((answer != 'a') && (answer != 'r') && (answer != 'e'));

	delete assembled;
	assembled = nullptr;

	return 0;
}
