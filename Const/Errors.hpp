#pragma once

enum struct AssembleErrors {
	CommandNotFound,
	FileNotOpen,
	NonNumeric,
	Overflow,
	NotEnoughBytes,
	MissingEnd,
	AlreadyDefined,
	Undefined
};

enum struct RuntimeErrors {
	Overflow,
	NonNumeric,
	NotModifiable,
	OutOfRange,
};


typedef RuntimeErrors Rte;
static void throwError(const Rte& rte, const size_t& line) {
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

		case Rte::OutOfRange:
			errorMsg = "Array out of range. Line: " + std::to_string(line);
			throw errorMsg;
			break;
	}
}