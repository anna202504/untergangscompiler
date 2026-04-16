#ifndef KLAMMER_H
#define KLAMMER_H

#include <stdio.h>

#include "tree.h"
#include "symbol_table.h"

void printDeclarationsFromSymbolTable(struct tableEntry *head);
void printFormulaFromSyntaxTree(struct treeNode *root);






#endif // KLAMMER_H
