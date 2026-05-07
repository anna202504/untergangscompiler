#include <stdio.h>
#include <stdlib.h>
#include "multiple.h"
#include "klammer.h"

void addFormula(struct formulaList **head, struct formulaList **tail, struct treeNode *f) {
    struct formulaList *n = malloc(sizeof(struct formulaList));
    n->formula = f;
    n->next = NULL;

    if (*head == NULL) {
        *head = *tail = n;
    } else {
        (*tail)->next = n;
        *tail = n;
    }
}


void printAllFormulas(struct formulaList *head) {
    struct formulaList *cur = head;
    while (cur != NULL) {
        printFormula(cur->formula);
        fprintf(stdout, " ;\n");
        if (cur->next != NULL) {
            fprintf(stdout, "\n");
        }
        cur = cur->next;
    }
}

void deleteAllFormulas(struct formulaList **head, struct formulaList **tail) {
    struct formulaList *cur = *head;
    while (cur != NULL) {
        struct formulaList *tmp = cur;
        cur = cur->next;
        deleteTree(tmp->formula);
        free(tmp);
    }
    *head = NULL;
    *tail = NULL;
}
