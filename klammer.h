#ifndef KLAMMER_H
#define KLAMMER_H

<<<<<<< HEAD
#include "symboltable.h"

void demo_symboltable_roundtrip(struct tableEntry *original);

#endif // KLAMMER_H
=======
#include <stdio.h>

#include "tree.h"
#include "symbol_table.h"

void printDeclarationsFromSymbolTable(struct tableEntry *head);
void printFormulaFromSyntaxTree(struct treeNode *root);






#endif // KLAMMER_H
>>>>>>> e5300ccfbce3b1a9622f5059f20fa5bbe17f4e2a
