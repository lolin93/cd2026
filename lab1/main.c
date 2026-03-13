#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 1. Define Linked List Node */
struct Node {
    char data;
    int count; /* Kept for structure, though not used for counting now */
    struct Node *next;
};

/* 2. Modified add_node: Always adds to the TAIL to keep the original order */
struct Node* add_node(struct Node *head, char target) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) return head;
    
    new_node->data = target;
    new_node->count = 1;
    new_node->next = NULL;

    if (head == NULL) {
        return new_node;
    }

    struct Node *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return head;
}

int main() {
    struct Node *head = NULL;
    int ch;

    /* 3. Open this source file itself */
    FILE *fp = fopen("main.c", "r"); 
    if (fp == NULL) {
        return 1;
    }

    /* 4. Build the Linked List by reading the file character by character */
    while ((ch = fgetc(fp)) != EOF) {
        /* No update_if_exists check: Add every single character as a new node */
        head = add_node(head, (char)ch);
    }
    fclose(fp);

    /* 5. Output: Traverse the Linked List to print each character */
    struct Node *current = head;
    while (current != NULL) {
        /* Print the data stored in the current node */
        if (current->data == '\n') {
            printf("'\\n'");
        } else if (current->data == ' ') {
            printf("'Space'");
        } else if (current->data == '\t') {
            printf("'Tab'");
        } else {
            printf("'%c'", current->data);
        }

        /* Print comma and space if it's NOT the last node */
        if (current->next != NULL) {
            printf(", ");
        }
        
        /* Move to the next node in the list */
        current = current->next;
    }
    printf("\n");

    /* 6. Free memory */
    current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
