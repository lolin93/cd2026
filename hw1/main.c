#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. 定義 Token 類型 (嚴格遵循投影片命名) --- */
typedef enum {
    TYPE_TOKEN, MAIN_TOKEN, IF_TOKEN, ELSE_TOKEN, WHILE_TOKEN,
    ID_TOKEN, LITERAL_TOKEN, ASSIGN_TOKEN, EQUAL_TOKEN,
    GREATEREQUAL_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, LESS_TOKEN,
    PLUS_TOKEN, MINUS_TOKEN, SEMICOLON_TOKEN,
    LEFTPAREN_TOKEN, REFTPAREN_TOKEN,  /* 注意：REFT */
    LEFTBRACE_TOKEN, REFTBRACE_TOKEN   /* 注意：REFT */
} TokenType;

/* --- 2. 結構化存儲：Linked List --- */
typedef struct TokenNode {
    char value[256];
    TokenType type;
    struct TokenNode *next;
} TokenNode;

/* 建立新節點 */
TokenNode* create_node(const char* val, TokenType type) {
    TokenNode* new_node = (TokenNode*)malloc(sizeof(TokenNode));
    if (!new_node) return NULL;
    strncpy(new_node->value, val, 255);
    new_node->value[255] = '\0';
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

/* 將 Token 加入串列末端 */
void append_token(TokenNode** head, const char* val, TokenType type) {
    TokenNode* new_node = create_node(val, type);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    TokenNode* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = new_node;
}

/* --- 3. 工具函式：手動實作字元分類 --- */
int is_alpha_underscore(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}
int is_num(int c) {
    return (c >= '0' && c <= '9');
}
int is_whitespace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* 將關鍵字轉換為對應 Token 類型 */
TokenType get_word_type(const char* s) {
    if (strcmp(s, "int") == 0)   return TYPE_TOKEN;
    if (strcmp(s, "main") == 0)  return MAIN_TOKEN;
    if (strcmp(s, "if") == 0)    return IF_TOKEN;
    if (strcmp(s, "else") == 0)  return ELSE_TOKEN;
    if (strcmp(s, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

/* 取得 Token 名稱字串 (嚴格對應投影片輸出) */
const char* get_token_name(TokenType type) {
    switch (type) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case REFTPAREN_TOKEN:    return "REFTPAREN_TOKEN";
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case REFTBRACE_TOKEN:    return "REFTBRACE_TOKEN";
        default:                 return "UNKNOWN";
    }
}

/* --- 4. 主程式掃描邏輯 --- */
int main() {
    // 優先讀取 input.c，若無則讀取標準輸入 (更靈活)
    FILE* src = fopen("input.c", "r");
    if (!src) src = stdin;

    TokenNode* head = NULL;
    int c;

    while ((c = fgetc(src)) != EOF) {
        if (is_whitespace(c)) continue;

        // 處理 ID 或關鍵字
        if (is_alpha_underscore(c)) {
            char buf[256];
            int i = 0;
            buf[i++] = (char)c;
            while (is_alpha_underscore(c = fgetc(src)) || is_num(c)) {
                if (i < 255) buf[i++] = (char)c;
            }
            buf[i] = '\0';
            if (c != EOF) ungetc(c, src);
            append_token(&head, buf, get_word_type(buf));
        }
        // 處理數字
        else if (is_num(c)) {
            char buf[256];
            int i = 0;
            buf[i++] = (char)c;
            while (is_num(c = fgetc(src))) {
                if (i < 255) buf[i++] = (char)c;
            }
            buf[i] = '\0';
            if (c != EOF) ungetc(c, src);
            append_token(&head, buf, LITERAL_TOKEN);
        }
        // 處理符號與 Relop
        else {
            switch (c) {
                case '=': {
                    int next = fgetc(src);
                    if (next == '=') append_token(&head, "==", EQUAL_TOKEN);
                    else { ungetc(next, src); append_token(&head, "=", ASSIGN_TOKEN); }
                    break;
                }
                case '>': {
                    int next = fgetc(src);
                    if (next == '=') append_token(&head, ">=", GREATEREQUAL_TOKEN);
                    else { ungetc(next, src); append_token(&head, ">", GREATER_TOKEN); }
                    break;
                }
                case '<': {
                    int next = fgetc(src);
                    if (next == '=') append_token(&head, "<=", LESSEQUAL_TOKEN);
                    else { ungetc(next, src); append_token(&head, "<", LESS_TOKEN); }
                    break;
                }
                case '+': append_token(&head, "+", PLUS_TOKEN); break;
                case '-': append_token(&head, "-", MINUS_TOKEN); break;
                case ';': append_token(&head, ";", SEMICOLON_TOKEN); break;
                case '(': append_token(&head, "(", LEFTPAREN_TOKEN); break;
                case ')': append_token(&head, ")", REFTPAREN_TOKEN); break;
                case '{': append_token(&head, "{", LEFTBRACE_TOKEN); break;
                case '}': append_token(&head, "}", REFTBRACE_TOKEN); break;
            }
        }
    }

    // 印出結果
    TokenNode* curr = head;
    while (curr) {
        printf("%s: %s\n", curr->value, get_token_name(curr->type));
        curr = curr->next;
    }

    // 釋放記憶體
    while (head) {
        TokenNode* tmp = head;
        head = head->next;
        free(tmp);
    }

    if (src != stdin) fclose(src);
    return 0;
}
