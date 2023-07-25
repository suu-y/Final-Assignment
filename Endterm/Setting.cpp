#include "Setting.h"
#include <Windows.h>


void readStr(const char* section, const char* keyword,
	const char* defaultValue, char* returnValue, int buffSize, const char* filePath) {

	//char buff[124];
	GetPrivateProfileString(section, keyword, defaultValue, returnValue, buffSize, filePath);

	//return buff;
}


void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(BUFFSIZE, currentDirectory);
}
