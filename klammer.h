#ifndef KLAMMER_H
#define KLAMMER_H
#include "symbol_table.h"
#include "tree.h"

void printDeclaration(struct tableEntry *head);
void printFormulaRec(struct treeNode *root, int parentPrecedence);
void printFormula(struct treeNode *root);

static int getPrecendence(struct treeNode *node);
static int needsParensInBinary(struct treeNode *child, struct treeNode *parent, int isRightChild);

#endif // KLAMMER_H
