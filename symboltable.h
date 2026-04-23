struct tableEntry* readSymbolTableFromFile(FILE *in);
struct tableEntry* cloneSymbolTable(struct tableEntry *original);
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
void addSymbolEntry(struct tableEntry **head, const char *identifier, const char *type, int arity);

struct tableEntry *getSymbolEntry(struct tableEntry *head, const char *identifier);

void clearSymbolTable(struct tableEntry **head);

void validate_arity(struct tableEntry *entry, int actual_count);

void printSymbolTable(FILE *out, struct tableEntry *head);

#endif // SYMBOL_TABLE_H