#include <stdio.h>
#include <stdlib.h>

struct Node {
    char data;
    struct Node *next;
};

void add_node(struct Node **head, char target) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) return;
    new_node->data = target;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        struct Node *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int main() {
    struct Node *head = NULL;
    int ch;

    /* Open the current source file */
    FILE *fp = fopen("main.c", "r"); 
    if (fp == NULL) {
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        add_node(&head, (char)ch);
    }
    fclose(fp);

    struct Node *current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("\\n ");
        } else if (current->data == ' ') {
            printf("Space ");
        } else if (current->data == '\t') {
            printf("Tab ");
        } else {
            printf("%c ", current->data);
        }
        current = current->next;
    }
    printf("\n");

    /* Free allocated memory */
    current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
