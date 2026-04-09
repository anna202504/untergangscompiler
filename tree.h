#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

enum nodeType {
    NODE_UNARY,
    NODE_BINARY,
    NODE_QUANTOR,
    NODE_PREDICATE,
    NODE_FUNCTION,
    NODE_VARIABLE,
    NODE_BOOL,
    NODE_NUMBER
};

enum unaryOp {
    OP_NOT
};

enum binaryOp {
    OP_AND,
    OP_OR,
    OP_IMPLIES,
    OP_EQUIV,
    OP_ARGLIST
};

enum quantorOp {
    OP_FORALL,
    OP_EXISTS
};

struct node {
    int nodeType;
    union {
        struct uType {
            enum unaryOp op;
            struct node *formula;
        } unaryType;

        struct bType {
            enum binaryOp op;
            struct node *left;
            struct node *right;
        } binaryType;

        struct qType {
            enum quantorOp op;
            struct node *variable;
            struct node *formula;
        } quantorType;

        struct pType {
            struct tableEntry *entry;
            struct node *arguments;
        } predicateType;

        struct fType {
            struct tableEntry *entry;
            struct node *arguments;
        } functionType;

        struct vType {
            struct tableEntry *entry;
        } variableType;

        struct booType {
            bool value;
        } boolType;

        struct nType {
            int value;
        } numberType;


    } types;
};

// functions 
struct node* create_bool_node(bool value);
struct node* create_number_node(int value);
struct node* create_variable_node(struct tableEntry *entry);
struct node* create_predicate_node(struct tableEntry *entry, struct node *args);
struct node* create_function_node(struct tableEntry *entry, struct node *args);
struct node* create_unary_node(enum unaryOp op, struct node *formula);
struct node* create_binary_node(enum binaryOp op, struct node *left, struct node *right);
struct node* create_quantor_node(enum quantorOp op, struct tableEntry *var, struct node *formula);
void print_tree(FILE *out, struct node *tree, int depth);
void free_tree(struct node *tree);

#endif