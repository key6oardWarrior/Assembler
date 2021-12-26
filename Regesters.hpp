#pragma once

struct Regesters {
    int accumulator;
    int index;
};

static Regesters ref = Regesters();
static volatile Regesters* regesters = reinterpret_cast<volatile Regesters*>(&ref);