#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Strukturdefinition
struct tableEntry {
    char *identifier;
    char *type;
    int arity;
    struct tableEntry *next;
};

// Funktionsprototypen
void addSymbolEntry(struct tableEntry **head, char *identifier, char *type, int arity);

struct tableEntry *getSymbolEntry(struct tableEntry *head, char *identifier);

void printSymbolTable(struct tableEntry *head);

void clearSymbolTable(struct tableEntry **head);

#endif // SYMBOL_TABLE_H