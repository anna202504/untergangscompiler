
#ifndef OPTIMIERUNG__H
#define OPTIMIERUNG__H

struct treeNode *eliminateDoubleNegations(struct treeNode *root);
struct treeNode *moveNegations(struct treeNode *root);
struct treeNode *replaceImplicationsAndEquivalences(struct treeNode *root);
struct treeNode *evaluateBooleanOperations(struct treeNode *root);

#endif // OPTIMIERUNG__H