#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <string>
#include <tuple>
#include <string>

#define BUFFSIZE 256
#define COLUMN 10
#define ROW 20

static char data[ROW + 1][COLUMN][BUFFSIZE];

void readStr(const char* section, const char* keyword,
	const char* defaultValue, char* returnValue, int buffSize, const char* filePath);

int readInt(const char* section, const char* keyword, int defaultValue, const char* filePath);

void getCurrentDirectory(char* currentDirectory);

// 改行コード削除
void remove_newline(char* str);

// 条件判定
std::tuple<char*, char*, char*> checkCondition(char* condition);

void CSV2Array(const char* fileName, char data[ROW + 1][COLUMN][BUFFSIZE]);


void  output(FILE* fp, errno_t error, int selectRow, int selectCol);
void write_log(char* log);