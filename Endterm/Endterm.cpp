// Endterm.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include "Setting.h"


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

int main() {
    
    // iniから読み込み
    char current_dir[BUFFSIZE];
    getCurrentDirectory(current_dir);
    char settingFile[BUFFSIZE];
    sprintf_s(settingFile, "%s\\setting.ini", current_dir);

    char file_name[BUFFSIZE];
    readStr("section1", "file_name", "", file_name, sizeof(file_name), settingFile);

    CSV2Array(file_name, data);

    DB_st* firstDB = NULL;  
    DB_st* previousDB = NULL;  

    for (int DBIndex = 0; DBIndex < COLUMN; DBIndex++) {
        DB_st* newDB = new DB_st();
        remove_newline(data[0][DBIndex]);
        newDB->header = data[0][DBIndex];
        newDB->col_no = DBIndex;
        newDB->value = NULL;
        newDB->next_header = NULL;

        ROW_st* previousRow = NULL;

        for (int ROWIndex = 1; ROWIndex <= ROW; ROWIndex++) {
            ROW_st* tmp = new ROW_st();
            remove_newline(data[ROWIndex][DBIndex]);
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

        if (firstDB == NULL) {
            firstDB = newDB;
        }
        else {
            previousDB->next_header = newDB;
        }

        previousDB = newDB;
    }

    // 入力受付
    FILE *fp;
    errno_t error;
    error = fopen_s(&fp, "query_n_view.log", "a");
    if (error != 0) {
        printf("failed to open.");
    }
    else {
        char select[BUFFSIZE];
        char where[BUFFSIZE];
        char* conditionColumn = NULL;
        char* conditionValue = NULL;
        char* sign = NULL;
        int conditionValue_int = 0;

        printf_s("---------------------------------------------\n");
        fprintf(fp, "------------------------------------------\n");
        printf_s("SELECT: ");
        scanf_s("%255s", &select, 255);
        fprintf(fp, "SELECT: %s\n", select);

        printf_s("WHERE: ");
        scanf_s("%255s", &where, 255);
        fprintf(fp, "WHERE: %s\n", where);
        printf_s("---------------------------------------------\n");
        fprintf(fp, "------------------------------------------\n");

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
                    if (sign == "=" && strcmp(currentRow->value, conditionValue) == 0) {
                        selectRow = currentRow->row_no;
                        output(fp, error, selectRow, selectCol);
                    }
                    else if (sign == ">" && (atof(currentRow->value) > conditionValue_int)) {
                        selectRow = currentRow->row_no;
                        output(fp, error, selectRow, selectCol);
                    }
                    else if (sign == "<" && (atof(currentRow->value) < conditionValue_int)) {
                        selectRow = currentRow->row_no;
                        output(fp, error, selectRow, selectCol);
                    }

                    currentRow = currentRow->next_row;
                }
            }
            currentDB = currentDB->next_header;
        }
    }
    fprintf(fp, "\n");
    fclose(fp);

    return 0;
}

// 出力関数
void  output(FILE* fp, errno_t error, int selectRow, int selectCol) {

    if (selectCol >= 0) {
        printf("%s\n", data[selectRow][selectCol]);
        fprintf(fp, "%s\n", data[selectRow][selectCol]);
    }
    else {
        for (int i = 0; i < COLUMN; i++) {
            printf("%s ", data[selectRow][i]);
            fprintf(fp, "%s ", data[selectRow][i]);
        }
        printf("\n");
        fprintf(fp, "%s", "\n");
    }
}