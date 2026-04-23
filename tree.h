#ifndef TREE_H
#define TREE_H

<<<<<<< HEAD
#include <stdbool.h>

enum nodeType {
    NODE_UNARY,
    NODE_BINARY,
    NODE_QUANTOR,
    NODE_PREDICATE,
    NODE_FUNCTION,
    NODE_VARIABLE,
    NODE_BOOL,
    NODE_NUMBER,
    NODE_ARG,      // expliziter Argument-Knoten
    NODE_VAR,      // expliziter Var-Knoten (für Bildstruktur)
    NODE_PRED,     // expliziter Pred-Knoten (für Bildstruktur)
    NODE_FUNC      // expliziter Func-Knoten (für Bildstruktur)
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

// Neue Knoten für Bildstruktur
struct node* create_arg_node(struct node *left, struct node *right);
struct node* create_var_node(struct tableEntry *entry);
struct node* create_pred_node(struct tableEntry *entry, struct node *args);
struct node* create_func_node(struct tableEntry *entry, struct node *args);

void print_tree(FILE *out, struct node *tree, int depth);
void free_tree(struct node *tree);
int count_arguments(struct node *args);

// Wandelt einen Syntaxbaum in eine PL1-V-Formel um (liefert neuen String, muss mit free() freigegeben werden)
char *tree_to_pl1_formula(struct node *tree);

#endif
=======
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

int countArguments(struct treeNode *argList);

#endif // TREE_H
>>>>>>> e5300ccfbce3b1a9622f5059f20fa5bbe17f4e2a
