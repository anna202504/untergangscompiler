# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct tableEntry {
    char *identifier;
    char *type;
    int arity;
    struct tableEntry *next;
};

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
    fprintf(stderr, "Error: Identifier '%s' not found in symbol table.\n", identifier);
    return NULL;
}
