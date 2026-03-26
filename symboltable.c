#include <stdio.h>
#include <stdlib.h>

struct tableEntry {
    char *identifier; 
    char *type; // e.g., "int", "float", "function"
    int arity; // number of parameters for functions, 0 for variables
    struct tableEntry *next; 
};

void addSymbolEntry(struct tableEntry **head, const char *identifier, const char *type, int arity) {
    struct tableEntry *newEntry = (struct tableEntry *)malloc(sizeof(struct tableEntry));
    newEntry->identifier = strdup(identifier);
    newEntry->type = strdup(type);
    newEntry->arity = arity;
    newEntry->next = *head; 
    *head = newEntry; 
}

getSymbolEntry(struct tableEntry *head, const char *identifier){
    struct tableEntry *current = head;
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current; 
        }
        current = current->next; 
    }
    return NULL;

}