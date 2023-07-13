#pragma once
#include <string>

void readStr(const char* section, const char* keyword,
	const char* defaultValue, char* returnValue, int buffSize, const char* filePath);

int readInt(const char* section, const char* keyword, int defaultValue, const char* filePath);