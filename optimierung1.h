
#ifndef OPTIMIERUNG1__H
#define OPTIMIERUNG1__H

struct treeNode *eliminateDoubleNegations(struct treeNode *root);
struct treeNode *moveNegations(struct treeNode *root);
struct treeNode *replaceImplicationsAndEquivalences(struct treeNode *root);
struct treeNode *evaluateBooleanOperations(struct treeNode *root);

#endif // OPTIMIERUNG1__H