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
	NonNumeric
};