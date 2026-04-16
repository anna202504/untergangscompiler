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

void validate_arity(struct tableEntry *entry, int actual_count) {
    if (!entry) {
        return;
    }
    
    if (entry->arity != actual_count) {
        fprintf(stderr, "Error: %s %s declared with arity %d but called with %d arguments\n",
                entry->type, entry->identifier, entry->arity, actual_count);
        exit(1);
    }
}

void printSymbolTable(FILE *out, struct tableEntry *head) {
    struct tableEntry *current = head;
    fprintf(out, "Symbol Table:\n");
    while (current != NULL) {
        fprintf(out, "Identifier: %s, Type: %s, Arity: %d\n", current->identifier, current->type, current->arity);
        current = current->next;
    }
    fprintf(out, "------------------\n");
}

// Liest eine Symboltabelle im printSymbolTable-Format aus FILE *in ein
// und gibt einen Pointer auf den Kopf der Liste zurück
struct tableEntry* readSymbolTableFromFile(FILE *in) {
    char line[256];
    struct tableEntry *head = NULL;
    while (fgets(line, sizeof(line), in)) {
        if (strncmp(line, "Identifier:", 11) == 0) {
            char identifier[128], type[64];
            int arity;
            if (sscanf(line, "Identifier: %127[^,], Type: %63[^,], Arity: %d", identifier, type, &arity) == 3) {
                addSymbolEntry(&head, identifier, type, arity);
            }
        }
    }
    return head;
}

// Erzeugt eine tiefe Kopie der Symboltabelle (verkettete Liste)
struct tableEntry* cloneSymbolTable(struct tableEntry *original) {
    struct tableEntry *newHead = NULL;
    struct tableEntry **ptr = &newHead;
    for (const struct tableEntry *cur = original; cur != NULL; cur = cur->next) {
        struct tableEntry *copy = malloc(sizeof(struct tableEntry));
        copy->identifier = strdup(cur->identifier);
        copy->type = strdup(cur->type);
        copy->arity = cur->arity;
        copy->next = NULL;
        *ptr = copy;
        ptr = &copy->next;
    }
    return newHead;
}