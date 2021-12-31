#include "..\pch.h"

bool DataType::is_const(void) { return isConst; }

size_t DataType::getIndex(void) { return index; }

size_t DataType::getSize(void) { return size; }

std::string& DataType::getStr(void) { return str; }

std::string& DataType::getType(void) { return type; }