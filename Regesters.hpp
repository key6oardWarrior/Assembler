#pragma once

struct Regesters {
	__int16 accumulator;
	__int16 index;
};

static volatile Regesters* regesters =
	reinterpret_cast<volatile Regesters*>(65535);
