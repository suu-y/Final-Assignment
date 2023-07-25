#include "Setting.h"


// 改行コード削除
void remove_newline(char* str) {
	int len = strlen(str);
	if (len > 0 && str[len - 1] == '\n')    str[len - 1] = '\0';
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

    return std::forward_as_tuple(conditionColumn, conditionValue, sign);
}

// csvの文字コードはSHIFT-JIS, 
void CSV2Array(const char* fileName, char data[ROW + 1][COLUMN][BUFFSIZE]) {
    FILE* fp;
    char s[BUFFSIZE];

    errno_t error;
    error = fopen_s(&fp, fileName, "r");
    if (error != 0) {
        char error_m[BUFFSIZE];
        strerror_s(error_m, sizeof(error_m), error);
        fprintf_s(stderr, "failed to open file : %s\n", error_m);
    }

    else {
        int i = 0;
        while (fgets(s, BUFFSIZE, fp) != NULL) {
            char* token;
            char* next_token = NULL;
            int j = 0;

            token = strtok_s(s, ",", &next_token);
            while (token != NULL && j < COLUMN) {
                strncpy_s(data[i][j], BUFFSIZE, token, _TRUNCATE);

                token = strtok_s(NULL, ",", &next_token);
                ++j;
            }
            ++i;
        }
        fclose(fp);
    }

    for (int a = 0; a < ROW + 1; a++) {
        for (int b = 0; b < COLUMN; b++) {
            printf("%s ", data[a][b]);
        }

        if (a == 0)printf("\n");
    }

}
