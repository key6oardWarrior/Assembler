#pragma once

#define u_max 65535
#define max 32767
#define min -32768

namespace KeywordMap {
	enum struct Keywords {
		ldwa,
		ldwx,

		stwa,
		stwx,

		block,
		equate,
		word,

		cpwa,
		cpwx,

		adda,
		addx,
		suba,
		subx,

		nega,
		negx,

		br,
		brgt,
		brlt,
		breq,
		brne,
		brge,
		brle,

		deci,

		deco,
		stro,
		ascii,

		stop,
		end
	};

	static std::map<std::string, Keywords> keywordMap {
		{"ldwa", Keywords::ldwa},
		{"ldwx", Keywords::ldwx},

		{"stwa", Keywords::stwa},
		{"stwx", Keywords::stwx},

		{".block", Keywords::block},
		{".equate", Keywords::equate},
		{".word", Keywords::word},
		
		{"cpwa", Keywords::cpwa},
		{"cpwx", Keywords::cpwx},

		{"adda", Keywords::adda},
		{"addx", Keywords::addx},
		{"suba", Keywords::suba},
		{"subx", Keywords::subx},

		{"nega", Keywords::nega},
		{"negx", Keywords::negx},

		{"br", Keywords::br},
		{"brgt", Keywords::brgt},
		{"brlt", Keywords::brlt},
		{"breq", Keywords::breq},
		{"brne", Keywords::brne},
		{"brge", Keywords::brge},
		{"brle", Keywords::brle},

		{"deci", Keywords::deci},

		{"deco", Keywords::deco},
		{"stro", Keywords::stro},

		{".ascii", Keywords::ascii},

		{"stop", Keywords::stop},
		{".end", Keywords::end}
	};
}

static std::map<std::string, std::vector<char>> keywordMap {
	// write to regesters
	{"ldwa", {'i', 'd'}},
	{"ldwx", {'i', 'd'}},

	// store regesters in memory
	{"stwa", {'d', 'x'}},
	{"stwx", {'d', 'x'}},

	// create blocks of memory
	{".block", {1}},
	{".equate", {1}},
	{".word", {1}},

	// compare regesters
	{"cpwa", {'i', 'd'}},
	{"cpwx", {'i', 'd'}},

	// arithmetic on regesters
	{"adda", {'i', 'd'}},
	{"addx", {'i', 'd'}},
	{"suba", {'i', 'd'}},
	{"subx", {'i', 'd'}},

	// negate regesters
	{"nega", {1}},
	{"negx", {1}},

	// branching instructions
	{"br", {1}},
	{"brgt", {1}},
	{"brlt", {1}},
	{"breq", {1}},
	{"brne", {1}},
	{"brge", {1}},
	{"brle", {1}},

	// input
	{"deci", {'d', 'x'}},

	// output
	{"deco", {'d', 'x'}},
	{"stro", {'d', 'x'}},

	// strings
	{".ascii", {1}},

	// ending program
	{"stop", {1}},
	{".end", {1}}
};