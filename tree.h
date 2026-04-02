#ifndef TREE_H
#define TREE_H

#include "symbol_table.h"

enum NodeType {
    NODE_QUANTOR,
    NODE_BINARY_OPERATOR,
    NODE_UNARY_OPERATOR,
    NODE_PREDICATE,
    NODE_FUNCTION,
    NODE_VARIABLE,
    NODE_NUMBER,
    NODE_BOOL
};

enum QuantorType {
    FORALL,
    EXISTS
};

enum BinaryOperatorType {
    BINOP_AND,
    BINOP_OR,
    BINOP_IMPLIES,
    BINOP_IFF
};

enum UnaryOperatorType {
    UOP_NOT
};


struct treeNode {
    int nodeType;
    struct treeNode *next;
    union {
        struct qType
        {
            enum QuantorType quantorType;
            struct tableEntry *var;
            struct treeNode *formula;
        }  quantorType;
        struct bType
        {
            struct treeNode *left;
            struct treeNode *right;
            enum BinaryOperatorType operatorType;
        } binaryType;
        struct uType
        {
            struct treeNode *child;
            enum UnaryOperatorType operatorType;
        } unaryType;
        struct pType
        {
            struct tableEntry *entry;
            struct treeNode *arguments;

        } predicatType;
        struct fType
        {
            struct tableEntry *entry;
            struct treeNode *arguments;
        } functionType;
        struct vType
        {
            struct tableEntry *entry;
        } variableType;
        struct nType
        {
            int value;
        } numberType;
        struct booType
        {
            int value;
        } boolType;
    
    } treeTypes;
};

struct treeNode *makeNode(int nodeType);
struct treeNode *copyTree(struct treeNode *root);
struct treeNode *deleteTree(struct treeNode *root);
void printTree(struct treeNode *root, int level);

#endif // TREE_H
