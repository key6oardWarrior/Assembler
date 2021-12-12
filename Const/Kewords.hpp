#pragma once

namespace KeywordMap {
	enum struct keywords {
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

		br,
		brgt,
		brlt,
		breq,
		brne,
		brge,
		brle,

		deci,
		stri,

		deco,
		stro,
		ascii,

		stop,
		end
	};

	static std::map<std::string, keywords> keywordMap {
		{"ldwa", keywords::ldwa},
		{"ldwx", keywords::ldwx},

		{"stwa", keywords::stwa},
		{"stwx", keywords::stwx},

		{".block", keywords::block},
		{".equate", keywords::equate},
		{".word", keywords::word},
		
		{"cpwa", keywords::cpwa},
		{"cpwx", keywords::cpwx},

		{"adda", keywords::adda},
		{"addx", keywords::addx},
		{"suba", keywords::suba},
		{"subx", keywords::subx},

		{"br", keywords::br},
		{"brgt", keywords::brgt},
		{"brlt", keywords::brlt},
		{"breq", keywords::breq},
		{"brne", keywords::brne},
		{"brge", keywords::brge},
		{"brle", keywords::brle},

		{"deci", keywords::deci},
		{"stri", keywords::stri},

		{"deco", keywords::deco},
		{"stro", keywords::stro},

		{".ascii", keywords::ascii},

		{"stop", keywords::stop},
		{".end", keywords::end}
	};
}

static std::map<std::string, std::vector<char>> keywordMap {
	// write to regesters
	{"ldwa", {'i', 'd'}},
	{"ldwx", {'i', 'd'}},

	// store regesters in memory
	{"stwa", {'d'}},
	{"stwx", {'d'}},

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
	{"stro", {1}},

	// strings
	{".ascii", {1}},

	// ending program
	{"stop", {1}},
	{".end", {1}}
};
