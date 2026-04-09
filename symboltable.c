#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"



void addSymbolEntry(struct tableEntry **head, const char *identifier, const char *type, int arity) {
    struct tableEntry *newEntry = (struct tableEntry *)malloc(sizeof(struct tableEntry));
    newEntry->identifier = strdup(identifier);
    newEntry->type = strdup(type);
    newEntry->arity = arity;
    newEntry->next = *head; 
    *head = newEntry; 
}

struct tableEntry *getSymbolEntry(struct tableEntry *head, const char *identifier){
    struct tableEntry *current = head;
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current; 
        }
        current = current->next; 
    }
    return NULL;
}

void clearSymbolTable(struct tableEntry **head) {
    struct tableEntry *current = *head;
    while (current != NULL) {
        struct tableEntry *next = current->next;
        free(current->identifier);
        free(current->type);
        free(current);
        current = next;
    }
    *head = NULL;
}