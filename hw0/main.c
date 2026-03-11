#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char data;
    int count;
    struct Node *next;
};

int update_if_exists(struct Node *head, char target) {
    struct Node *current = head;
    while (current != NULL) {
        if (current->data == target) {
            current->count++;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

struct Node* add_node(struct Node *head, char target) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) return head;
    new_node->data = target;
    new_node->count = 1;
    new_node->next = head;
    return new_node;
}

int main() {
    struct Node *head = NULL;
    int ch; // Use int for EOF comparison

    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        printf("Error: Could not open file!\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        if (!update_if_exists(head, (char)ch)) {
            head = add_node(head, (char)ch);
        }
    }

    printf("Character Statistics:\n");
    printf("------------------------\n");
    struct Node *current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("'\\n' : %d\n", current->count);
        } else if (current->data == ' ') {
            printf("'Space' : %d\n", current->count);
        } else {
            printf("'%c' : %d\n", current->data, current->count);
        }
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    fclose(fp);
    return 0;
}
