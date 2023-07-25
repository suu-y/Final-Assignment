#include "Setting.h"
#include <Windows.h>


void readStr(const char* section, const char* keyword,
	const char* defaultValue, char* returnValue, int buffSize, const char* filePath) {

	//char buff[124];
	GetPrivateProfileString(section, keyword, defaultValue, returnValue, buffSize, filePath);

	//return buff;
}

int readInt(const char* section, const char* keyword,
	int defaultValue, const char* filePath) {

	return GetPrivateProfileInt(section, keyword, defaultValue, filePath);
}

void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(BUFFSIZE, currentDirectory);
}
