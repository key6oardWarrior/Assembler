#include "pch.h"
#include "Assemble\Assemble.hpp"

int main(void) {
	std::cout << "Enter assembly file: ";

	std::string file;
	std::getline(std::cin, file);

	Assemble* assembled = NULL;
	char answer;
	do {
		std::cout << "\nAssemble (a), Run (r), Reassemble (ra), \
Assemble new file (nf), Exit (e): ";
		std::cin >> answer;
		answer = std::tolower(answer);

		switch(answer) {
			case 'a':
				assembled = new Assemble(file);
				assembled->assembleCode();
				break;

			case 'r':
				assembled = new Assemble(file);
				assembled->assembleCode();
				break;

			case 'ra':
				delete assembled;
				assembled = new Assemble(file);
				assembled->assembleCode();
				break;

			case 'nf':
				delete assembled;
				std::getline(std::cin, file);

				assembled = new Assemble(file);
				assembled->assembleCode();
			default:
				break;
		}
	} while((answer != 'a') && (answer != 'r') && (answer != 'e'));

	delete assembled;
	assembled = nullptr;

	return 0;
}
