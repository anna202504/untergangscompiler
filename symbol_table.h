#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Strukturdefinition
struct tableEntry {
    char *identifier;
    char *type;
    int arity;
    struct tableEntry *next;
} tableEntry;

// Funktionsprototypen
void addSymbolEntry(struct tableEntry **head, char *identifier, char *type, int arity);

struct tableEntry *getSymbolEntry(struct tableEntry *head, char *identifier);

#endif // SYMBOL_TABLE_H