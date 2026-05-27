#include <stdio.h>
#include <string.h>

#define TOKEN_NUM 1
#define TOKEN_PLUS 2
#define TOKEN_LPAREN 3
#define TOKEN_RPAREN 4
#define TOKEN_EOF 5
#define TOKEN_ERROR 6

int token;
char token_text[100];

char input[1000];
int pos = 0;

int has_error = 0;

void next_token();
void parse_S(int depth);
void parse_S_prime(int depth);
void parse_E(int depth);
void print_indent(int depth);
void error();
void match(int expected);

void print_indent(int depth) {
    int i;
    for (i = 0; i < depth; i++) {
        printf("  ");
    }
}

void error() {
    has_error = 1;
}

void next_token() {
    int i = 0;
    char c;

    while (
        input[pos] == ' ' ||
        input[pos] == '\n' ||
        input[pos] == '\t' ||
        input[pos] == '\r'
    ) {
        pos++;
    }

    c = input[pos];

    if (c == '\0') {
        token = TOKEN_EOF;
        strcpy(token_text, "EOF");
        return;
    }

    if (c >= '0' && c <= '9') {

        while (
            input[pos] >= '0' &&
            input[pos] <= '9'
        ) {
            if (i < 99) {
                token_text[i++] = input[pos];
            }
            pos++;
        }

        token_text[i] = '\0';
        token = TOKEN_NUM;
        return;
    }

    if (c == '+') {
        token = TOKEN_PLUS;
        strcpy(token_text, "+");
        pos++;
        return;
    }

    if (c == '(') {
        token = TOKEN_LPAREN;
        strcpy(token_text, "(");
        pos++;
        return;
    }

    if (c == ')') {
        token = TOKEN_RPAREN;
        strcpy(token_text, ")");
        pos++;
        return;
    }

    token = TOKEN_ERROR;
    token_text[0] = c;
    token_text[1] = '\0';
    pos++;
}

void match(int expected) {
    if (token == expected) {
        next_token();
    } else {
        error();
    }
}

void parse_S(int depth) {
    if (has_error) return;

    print_indent(depth);
    printf("S -> E S'\n");

    if (token == TOKEN_NUM || token == TOKEN_LPAREN) {
        parse_E(depth + 1);
        parse_S_prime(depth + 1);
    } else {
        error();
    }
}

void parse_S_prime(int depth) {
    if (has_error) return;

    if (token == TOKEN_PLUS) {

        print_indent(depth);
        printf("S' -> + S\n");

        match(TOKEN_PLUS);

        parse_S(depth + 1);

    } else if (
        token == TOKEN_RPAREN ||
        token == TOKEN_EOF
    ) {

        print_indent(depth);
        printf("S' -> epsilon\n");

    } else {
        error();
    }
}

void parse_E(int depth) {
    if (has_error) return;

    if (token == TOKEN_NUM) {

        print_indent(depth);
        printf("E -> num\n");

        print_indent(depth + 1);
        printf("%s\n", token_text);

        match(TOKEN_NUM);

    } else if (token == TOKEN_LPAREN) {

        print_indent(depth);
        printf("E -> ( S )\n");

        match(TOKEN_LPAREN);

        parse_S(depth + 1);

        if (token == TOKEN_RPAREN) {
            match(TOKEN_RPAREN);
        } else {
            error();
        }

    } else {
        error();
    }
}

int main() {

    printf("Input: ");
    fgets(input, sizeof(input), stdin);

    next_token();

    parse_S(0);

    if (!has_error && token == TOKEN_EOF) {
        printf("Parsing completed successfully.\n");
    } else {
        printf("Parsing error.\n");
    }

    return 0;
}
