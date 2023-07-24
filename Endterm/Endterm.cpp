// Endterm.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <string>
#include <tuple>
#include "Setting.h"

#define BUFFSIZE 256
#define COLUMN 10
#define ROW 20

struct ROW_st {
    int row_no;
    int col_no;
    char* value;
    ROW_st* next_row;
};

struct DB_st {
    char* header;
    ROW_st* value;
    DB_st* next_header;
};

//DB* data_in;

void getCurrentDirectory(char* currentDirectory) {
    GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

// csvの文字コードはSHIFT-JIS, 
void CSV2Array(const char* fileName, char data[ROW+1][COLUMN][BUFFSIZE]) {
    FILE* fp;
    char s[BUFFSIZE];

    //data_in = (DB*)malloc(sizeof(DB)); // DBの初期化

    errno_t error;
    error = fopen_s(&fp, fileName, "r");
    if (error != 0) {
        char error_m[BUFFSIZE];
        strerror_s(error_m, sizeof(error_m), error);
        fprintf_s(stderr, "failed to open file : %s\n", error_m);
    }

    else {
        int i = 0;
        while (fgets(s, BUFFSIZE, fp) != NULL) { // ヘッダ以降
            char* token;
            char* next_token = NULL;
            int j = 0;

            // Start tokenizing the string
            token = strtok_s(s, ",", &next_token);
            while (token != NULL && j < COLUMN) {
                //strncpy_s(data[i][j], BUFFSIZE, token, _TRUNCATE);
                strncpy_s(data[i][j], BUFFSIZE, token, _TRUNCATE);

                // Move to next token
                token = strtok_s(NULL, ",", &next_token);
                ++j;
            }

            ++i;
        }
        fclose(fp);
    }
    
      for (int a = 0; a < ROW+1; a++) {
          for (int b = 0; b < COLUMN; b++) {
              printf("%s ", data[a][b]);
          }

          //printf("\n");
      }

}

std::tuple<char*, char*, char*> checkCondition(char* condition) {
    bool Equal;
    bool Greater_than;
    bool Less_than;
    bool AND;
    char* next_token = NULL;
    char* conditionColumn = NULL;
    char* conditionValue = NULL;
    char* sign = NULL;


    // 条件式の内容を確認
    AND = strstr(condition, "AND");
    Equal = strchr(condition, '=');
    Greater_than = strchr(condition, '<');
    Less_than = strchr(condition, '>');
    if (Equal) {
        sign = (char*)"=";
        conditionColumn = strtok_s(condition, "=", &next_token);
        conditionValue = strtok_s(next_token, " ", &next_token);
    }
    if (Greater_than) {
        sign = (char*)">";
        conditionColumn = strtok_s(condition, ">", &next_token);
        conditionValue = strtok_s(next_token, " ", &next_token);
    }
    if (Less_than) {
        sign = (char*)"<";
        conditionColumn = strtok_s(condition, "<", &next_token);
        conditionValue = strtok_s(next_token, " ", &next_token);
    }

    return std::forward_as_tuple(conditionColumn, conditionValue, sign);
}



int main() {
    
    //const char* whereClause[BUFFSIZE];
    // iniから読み込み
    char current_dir[BUFFSIZE];
    getCurrentDirectory(current_dir);
    char settingFile[BUFFSIZE];
    sprintf_s(settingFile, "%s\\setting.ini", current_dir);

    char file_name[BUFFSIZE];
    readStr("section1", "file_name", "", file_name, sizeof(file_name), settingFile);

    // メモリのサイズやばいからstaticで？
    static char data[ROW+1][COLUMN][BUFFSIZE];

    CSV2Array(file_name, data);

    ////////////////////////
    DB_st* firstDB = NULL;  // 最初のDB構造体を追跡するポインタ
    DB_st* previousDB = NULL;  // 前のDB構造体を追跡するポインタ

    for (int DBIndex = 0; DBIndex < COLUMN; DBIndex++) {
        DB_st* newDB = new DB_st();
        newDB->header = data[0][DBIndex];
        newDB->value = NULL;
        newDB->next_header = NULL;

        // Columnの生成と連結
        ROW_st* previousRow = NULL;

        for (int ROWIndex = 1; ROWIndex <= ROW; ROWIndex++) {
            ROW_st* tmp = new ROW_st();
            tmp->row_no = ROWIndex;
            tmp->col_no = DBIndex;
            tmp->value = data[ROWIndex][DBIndex];
            tmp->next_row = NULL;

            if (previousRow == NULL) {
                newDB->value = tmp;
            }
            else {
                previousRow->next_row = tmp;
            }

            previousRow = tmp;

        }

        // DBの連結
        if (firstDB == NULL) {
            firstDB = newDB;
        }
        else {
            previousDB->next_header = newDB;
        }

        previousDB = newDB;
    }

    // 出力
/*
    DB_st* currentDB = firstDB;

    while (currentDB != NULL) {
        fprintf_s(stdout, "DB_st header = %s\n", currentDB->header);

        ROW_st* currentRow = currentDB->value;

        while (currentRow != NULL) {
            currentRow = currentRow->next_row;
        }

        currentDB = currentDB->next_header;
    }
*/
    ////////////////////////


    // 入力受付
    char select[BUFFSIZE];
    char where[BUFFSIZE];
    char* conditionColumn = NULL;
    char* conditionValue = NULL;
    char* sign = NULL;
    printf_s("select: ");
    scanf_s("%255s", &select, 255);

    printf_s("where: ");
    scanf_s("%255s", &where, 255);

    std::tie(conditionColumn, conditionValue, sign) = checkCondition(where);

    int selectRow = -1;

    DB_st* currentDB = firstDB;

    while (currentDB != NULL) {
        //fprintf_s(stdout, "DB_st header = %s\n", currentDB->header);
        if (strcmp(currentDB->header, conditionColumn) == 0) {
            ROW_st* currentRow = currentDB->value;

            while (currentRow != NULL) {
                if (strcmp(currentRow->value, conditionValue) == 0) {
                    fprintf_s(stdout, "%d, %d\n", currentRow->row_no, currentRow->col_no);
                    selectRow = currentRow->row_no;
                }

                currentRow = currentRow->next_row;
            }
        }    
        currentDB = currentDB->next_header;
    }

    currentDB = firstDB;

    while (currentDB != NULL) {
        if (strcmp(currentDB->header, select) == 0) {
            ROW_st* currentRow = currentDB->value;

            while (currentRow != NULL) {
                if (currentRow->row_no == selectRow) {
                    printf("%s\n", currentRow->value);
                    break;
                }

                currentRow = currentRow->next_row;
            }
        }

        currentDB = currentDB->next_header;
    }

    return 0;



}