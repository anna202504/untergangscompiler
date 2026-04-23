#ifndef OPTIMIERUNG_H
#define OPTIMIERUNG_H

#include "tree.h"

struct treeNode *eliminateDoubleNegations(struct treeNode *root);
struct treeNode *pushNegationsToPredicates(struct treeNode *root);
struct treeNode *eliminateImplications(struct treeNode *root);
struct treeNode *eliminateEquivalences(struct treeNode *root);
struct treeNode *eliminateBooleanConstants(struct treeNode *root);

#endif // OPTIMIERUNG_H
