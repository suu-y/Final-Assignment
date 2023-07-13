// Endterm.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <string>
#include "Setting.h"

#define BUFFSIZE 256
#define COLUMN 10
#define ROW 20

void getCurrentDirectory(char* currentDirectory) {
    GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

// csvの文字コードはSHIFT-JIS, 
void CSV2Array(const char* fileName, char data[ROW][2][BUFFSIZE]) {
    FILE* fp;
    char s[BUFFSIZE];

    char* p1;
    char* p2;
    char* p3;

    errno_t error;
    error = fopen_s(&fp, fileName, "r");
    if (error != 0) {
        char error_m[BUFFSIZE];
        strerror_s(error_m, sizeof(error_m), error);
        fprintf_s(stderr, "failed to open file : %s\n", error_m);
    }

    else {
        int i = 0;
        fgets(s, BUFFSIZE, fp);  // ヘッダ行を読み飛ばす
        while (fgets(s, BUFFSIZE, fp) != NULL) {
            char* token;
            char* day;
            day = strtok_s(s, ",", &p1);
            while (p1 != NULL) {
                p2 = strtok_s(NULL, ",", &p1);
                //data[i][0] = atof(p2);
                strncpy_s(data[i][0], BUFFSIZE, p2, _TRUNCATE);
                p2 = strtok_s(p1, "\n", &p3);
                //data[i][1] = atof(p2);
                strncpy_s(data[i][1], BUFFSIZE, p2, _TRUNCATE);
                break;
            }
            i++;
        }
        fclose(fp);
    }
    
      for (int a = 0; a < ROW; a++) {
          for (int b = 0; b < 2; b++) {
              printf("%s ", data[a][b]);
          }

          printf("\n");
      }

}


int main() {
    // iniから読み込み
    char current_dir[BUFFSIZE];
    getCurrentDirectory(current_dir);
    char settingFile[BUFFSIZE];
    sprintf_s(settingFile, "%s\\setting.ini", current_dir);

    char file_name[BUFFSIZE];
    readStr("section1", "file_name", "", file_name, sizeof(file_name), settingFile);

    char data[ROW][2][BUFFSIZE];

    CSV2Array(file_name, data);
}