#pragma once

static std::map<std::string, std::vector<char>> keywords {
	// write to regesters
	{"ldwa", {'i', 'd'}},
	{"ldwx", {'i', 'd'}},

	// store regesters in memory
	{"stwa", {'d'}},
	{"stwx", {'d'}},

	// compare regesters
	{"cpwa", {'i', 'd'}},
	{"cpwx", {'i', 'd'}},

	// arithmetic on regesters
	{"adda", {'i', 'd'}},
	{"addx", {'i', 'd'}},

	// branching instructions
	{"br", {1}},
	{"brgt", {1}},
	{"brlt", {1}},
	{"breq", {1}},
	{"brne", {1}},
	{"brge", {1}},
	{"brle", {1}},

	// input
	{"deci", {'d'}},
	{"stri", {'d'}},

	// output
	{"deco", {1}},
	{"stro", {1}}
};