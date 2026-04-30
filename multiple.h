#ifndef MULTIPLE_H
#define MULTIPLE_H
#include "tree.h"

struct formulaList {
    struct treeNode *formula;
    struct formulaList *next;
};

void addFormula(struct formulaList **head, struct formulaList **tail, struct treeNode *f);
void printAllFormulas(struct formulaList *head);
void deleteAllFormulas(struct formulaList **head, struct formulaList **tail);

#endif // MULTIPLE_H