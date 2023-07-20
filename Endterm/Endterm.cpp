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

struct DB {
    char* col1;
    char* col2;
    char* col3;
    char* col4;
    char* col5;
    char* col6;
    char* col7;
    char* col8;
    char* col9;
    char* col10;
};

void getCurrentDirectory(char* currentDirectory) {
    GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

// csvの文字コードはSHIFT-JIS, 
void CSV2Array(const char* fileName, char data[ROW][COLUMN][BUFFSIZE]) {
    FILE* fp;
    char s[BUFFSIZE];
    
    char* p0;
    char* p1;
    char* p2;
    char* p3;
    char* p4;
    char* p5;
    char* p6;
    char* p7;
    char* p8;
    char* p9;
    char* p10;

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
            p1 = strtok_s(s, ",", &p0);
            strncpy_s(data[i][0], BUFFSIZE, p1, _TRUNCATE);
            //p0 = strtok_s(s, ",", &p1);
            while (p1 != NULL) {
                p2 = strtok_s(p0, ",", &p1);;
                strncpy_s(data[i][1], BUFFSIZE, p2, _TRUNCATE);
                p3 = strtok_s(p1, ",", &p2);
                strncpy_s(data[i][2], BUFFSIZE, p3, _TRUNCATE);
                p4 = strtok_s(p2, ",", &p3);
                strncpy_s(data[i][3], BUFFSIZE, p4, _TRUNCATE);
                p5 = strtok_s(p3, ",", &p4);
                strncpy_s(data[i][4], BUFFSIZE, p5, _TRUNCATE);
                p6 = strtok_s(p4, ",", &p5);
                strncpy_s(data[i][5], BUFFSIZE, p6, _TRUNCATE);
                p7 = strtok_s(p5, ",", &p6);
                strncpy_s(data[i][6], BUFFSIZE, p7, _TRUNCATE);
                p8 = strtok_s(p6, ",", &p7);
                strncpy_s(data[i][7], BUFFSIZE, p8, _TRUNCATE);
                p9 = strtok_s(p7, ",", &p8);
                strncpy_s(data[i][8], BUFFSIZE, p9, _TRUNCATE);
                p10 = strtok_s(p8, "\n", &p9);
                strncpy_s(data[i][9], BUFFSIZE, p10, _TRUNCATE);

                break;
            }
            i++;
        }
        fclose(fp);
    }
    
      for (int a = 0; a < ROW; a++) {
          for (int b = 0; b < COLUMN; b++) {
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

    char data[ROW][COLUMN][BUFFSIZE];

    CSV2Array(file_name, data);
}