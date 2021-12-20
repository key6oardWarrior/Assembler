#include "pch.h"
#include "Execute\Runtime.hpp"

int main(void) {
	std::cout << "Enter assembly file: ";

	std::string file;
	std::getline(std::cin, file);

	Assemble* assembled = NULL;
	Runtime* excute = NULL;
	char answer;
	do {
		std::cout << "\nAssemble (a), Run (r), Reassemble (s), \
Assemble new file (n), Exit (e): ";
		std::cin >> answer;
		answer = std::tolower(answer);

		switch(answer) {
			case 'a':
				assembled = new Assemble(file);
				assembled->assembleCode();
				break;

			case 'r':
				if(assembled == NULL) {
					assembled = new Assemble(file);
					assembled->assembleCode();
				}

				excute = new Runtime(assembled->getOrder(),
					assembled->getVars());
				excute->run();
				break;

			case 's':
				delete assembled;
				assembled = new Assemble(file);
				assembled->assembleCode();
				break;

			case 'n':
				delete assembled;
				std::getline(std::cin, file);

				assembled = new Assemble(file);
				assembled->assembleCode();
				break;
			default:
				break;
		}
	} while(answer != 'r');

	delete assembled;
	assembled = nullptr;

	delete excute;
	excute = nullptr;

	return 0;
}
