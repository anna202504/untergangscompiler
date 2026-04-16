# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "symbol_table.h"

void addSymbolEntry(struct tableEntry **head, char *identifier, char *type, int arity) {
    struct tableEntry *newEntry = (struct tableEntry *)malloc(sizeof(struct tableEntry));
    newEntry->identifier = strdup(identifier);
    newEntry->type = strdup(type);
    newEntry->arity = arity;

    newEntry->next = *head;
    *head = newEntry;
}

struct tableEntry *getSymbolEntry(struct tableEntry *head, char *identifier) {
    struct tableEntry *current = head;
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printSymbolTable(struct tableEntry *head) {
    struct tableEntry *current = head;

    fprintf(stderr, "\n--- Symbol Table ---\n");

    while (current != NULL) {
        fprintf(stderr, "Identifier: %s | Type: %s | Arity: %d\n",
               current->identifier,
               current->type,
               current->arity);
        current = current->next;
    }

    fprintf(stderr, "---------------------\n");
}

void clearSymbolTable(struct tableEntry **head) {
    struct tableEntry *current = *head;
    while (current != NULL) {
        struct tableEntry *temp = current;
        current = current->next;
        free(temp->identifier);
        free(temp->type);
        free(temp);
    }
    *head = NULL;
}
