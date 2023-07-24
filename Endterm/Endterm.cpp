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

    // メモリのサイズやばいからstaticで？
static char data[ROW + 1][COLUMN][BUFFSIZE];

struct ROW_st {
    int row_no;
    int col_no;
    char* value;
    ROW_st* next_row;
};

struct DB_st {
    int col_no;
    char* header;
    ROW_st* value;
    DB_st* next_header;
};


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

          if(a==0)printf("\n");
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
    Equal = strchr(condition, '=');
    Greater_than = strchr(condition, '>');
    Less_than = strchr(condition, '<');


    if (Equal) {
        sign = (char*)"=";
        conditionColumn = strtok_s(condition, "=", &next_token);
        conditionValue = strtok_s(next_token, " ", &next_token);
    }
    if (Greater_than) {
        sign = (char*)">";
        conditionColumn = strtok_s(condition, ">", &next_token);
        conditionValue = strtok_s(next_token, "", &next_token);
    }
    if (Less_than) {
        sign = (char*)"<";
        conditionColumn = strtok_s(condition, "<", &next_token);
        conditionValue = strtok_s(next_token, "", &next_token);
    }

    //condition = next_token;

    return std::forward_as_tuple(conditionColumn, conditionValue, sign);
}

// 改行コード削除
void remove_newline(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')    str[len - 1] = '\0'; 
}

// 出力関数
void  output (int selectRow, int selectCol) {
    if (selectCol > 0)   printf("%s\n", data[selectRow][selectCol]);
    else {
        for (int i = 0; i < COLUMN; i++) {
            printf("%s ", data[selectRow][i]);
        }
        printf("\n");
    }
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

    CSV2Array(file_name, data);

    ////////////////////////
    DB_st* firstDB = NULL;  // 最初のDB構造体を追跡するポインタ
    DB_st* previousDB = NULL;  // 前のDB構造体を追跡するポインタ

    for (int DBIndex = 0; DBIndex < COLUMN; DBIndex++) {
        DB_st* newDB = new DB_st();
        remove_newline(data[0][DBIndex]);
        newDB->header = data[0][DBIndex];
        newDB->col_no = DBIndex;
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




    // 入力受付
    char select[BUFFSIZE];
    char where[BUFFSIZE];
    char* conditionColumn = NULL;
    char* conditionValue = NULL;
    char* sign = NULL;
    int conditionValue_int = 0;

    printf_s("---------------------------------------------\n");
    printf_s("select: ");
    scanf_s("%255s", &select, 255);

    printf_s("where: ");
    scanf_s("%255s", &where, 255);

    printf_s("---------------------------------------------\n");

    std::tie(conditionColumn, conditionValue, sign) = checkCondition(where);

    if (sign == ">" || sign == "<") {
        conditionValue_int = atof(conditionValue);
    }

    int selectRow = -1;
    int selectCol = -1;

    DB_st* DB_forHeader = firstDB;
    while (DB_forHeader != NULL) {
        if (strcmp(DB_forHeader->header, select) == 0) {
            selectCol = DB_forHeader->col_no;
        }
        DB_forHeader = DB_forHeader->next_header;
    }

    DB_st* currentDB = firstDB;

    while (currentDB != NULL) {

        if (strcmp(currentDB->header, conditionColumn) == 0) {
            ROW_st* currentRow = currentDB->value;

            while (currentRow != NULL) {
                if (sign=="=" && strcmp(currentRow->value, conditionValue) == 0) {
                    selectRow = currentRow->row_no;
                    output(selectRow, selectCol);
                }
                else if (sign == ">" && (atof(currentRow->value) > conditionValue_int)) {
                    selectRow = currentRow->row_no;
                    output(selectRow, selectCol);
                }
                else if (sign == "<" && (atof(currentRow->value) < conditionValue_int)) {
                    selectRow = currentRow->row_no;
                    output(selectRow, selectCol);
                }

                currentRow = currentRow->next_row;
            }
        }    
        currentDB = currentDB->next_header;
    }

    return 0;



}