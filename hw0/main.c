#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define Linked List Node */
struct Node {
    char data;
    int count;
    struct Node *next;
};

/* Function to search for an existing character in the list */
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

/* Function to add a new character node to the list */
struct Node* add_node(struct Node *head, char target) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) return head;
    new_node->data = target;
    new_node->count = 1;
    new_node->next = head;
    return new_node;
}

/* Bonus: Function to sort the list by character count (Descending) */
void sort_list(struct Node *head) {
    if (head == NULL) return;
    struct Node *i, *j;
    int temp_count;
    char temp_data;

    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (j->count > i->count) {
                // Swap Count
                temp_count = i->count;
                i->count = j->count;
                j->count = temp_count;
                // Swap Character
                temp_data = i->data;
                i->data = j->data;
                j->data = temp_data;
            }
        }
    }
}

int main() {
    struct Node *head = NULL;
    int ch;

    /* 1. Open this source file itself */
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        printf("Error: Could not open file!\n");
        return 1;
    }

    /* 2. Read characters and build linked list */
    while ((ch = fgetc(fp)) != EOF) {
        if (!update_if_exists(head, (char)ch)) {
            head = add_node(head, (char)ch);
        }
    }
    fclose(fp);

    /* 3. Sort the list for bonus points */
    sort_list(head);

    /* 4. Print the statistics */
    printf("Character Statistics (Sorted by Count):\n");
    printf("---------------------------------------\n");
    struct Node *current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("'\\n' : %d\n", current->count);
        } else if (current->data == ' ') {
            printf("'Space' : %d\n", current->count);
        } else if (current->data == '\t') {
            printf("'Tab' : %d\n", current->count);
        } else {
            printf("'%c' : %d\n", current->data, current->count);
        }
        current = current->next;
    }

    /* 5. Free memory */
    current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
