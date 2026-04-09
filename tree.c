#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symboltable.h"


struct node* create_bool_node(bool value) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_BOOL;
    n->types.boolType.value = value;
    return n;
}

struct node* create_number_node(int value) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_NUMBER;
    n->types.numberType.value = value;
    return n;
}

struct node* create_variable_node(struct tableEntry *entry) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_VARIABLE;
    n->types.variableType.entry = entry;
    return n;
}

struct node* create_predicate_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_PREDICATE;
    n->types.predicateType.entry = entry;
    n->types.predicateType.arguments = args;
    return n;
}

struct node* create_function_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_FUNCTION;
    n->types.functionType.entry = entry;
    n->types.functionType.arguments = args;
    return n;
}

struct node* create_unary_node(enum unaryOp op, struct node *formula) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_UNARY;
    n->types.unaryType.op = op;
    n->types.unaryType.formula = formula;
    return n;
}

struct node* create_binary_node(enum binaryOp op, struct node *left, struct node *right) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_BINARY;
    n->types.binaryType.op = op;
    n->types.binaryType.left = left;
    n->types.binaryType.right = right;
    return n;
}

struct node* create_quantor_node(enum quantorOp op, struct tableEntry *var, struct node *formula) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_QUANTOR;
    n->types.quantorType.op = op;
    n->types.quantorType.variable = create_variable_node(var);
    n->types.quantorType.formula = formula;
    return n;
}


// tree print with depth
void print_tree(FILE *out, struct node *tree, int depth) {
    if (!tree) return;
    
    char indent[256];
    indent[0] = '\0';
    for (int i = 0; i < depth; i++) strcat(indent, ".");
    
    switch (tree->nodeType) {
        case NODE_BOOL:
            fprintf(out, "STP: %s%s\n", indent, tree->types.boolType.value ? "TRUE" : "FALSE");
            break;
        case NODE_NUMBER:
            fprintf(out, "STP: %sNumber: %d\n", indent, tree->types.numberType.value);
            break;
        case NODE_VARIABLE:
            fprintf(out, "STP: %sVariable: %s\n", indent, tree->types.variableType.entry->identifier);
            break;
        case NODE_PREDICATE:
            fprintf(out, "STP: %sPredicate: %s\n", indent, tree->types.predicateType.entry->identifier);
            if (tree->types.predicateType.arguments)
                print_tree(out, tree->types.predicateType.arguments, depth + 1);
            break;
        case NODE_FUNCTION:
            fprintf(out, "STP: %sFunction: %s\n", indent, tree->types.functionType.entry->identifier);
            if (tree->types.functionType.arguments)
                print_tree(out, tree->types.functionType.arguments, depth + 1);
            break;
        case NODE_UNARY:
            switch (tree->types.unaryType.op) {
                case OP_NOT:
                    fprintf(out, "STP: %sNOT\n", indent);
                    break;
            }
            print_tree(out, tree->types.unaryType.formula, depth + 1);
            break;
        case NODE_BINARY: {
            const char *op_name;
            switch (tree->types.binaryType.op) {
                case OP_AND: op_name = "AND"; break;
                case OP_OR: op_name = "OR"; break;
                case OP_IMPLIES: op_name = "IMPLICATION"; break;
                case OP_EQUIV: op_name = "EQUIVALENT"; break;
                case OP_ARGLIST: op_name = NULL; break; 
                default: op_name = "UNKNOWN"; break;
            }
            if (op_name) {
                fprintf(out, "STP: %s%s\n", indent, op_name);
                print_tree(out, tree->types.binaryType.left, depth + 1);
                print_tree(out, tree->types.binaryType.right, depth + 1);
            } else {
                print_tree(out, tree->types.binaryType.left, depth);
                print_tree(out, tree->types.binaryType.right, depth);
            }
            break;
        }
        case NODE_QUANTOR: {
            const char *op_name = (tree->types.quantorType.op == OP_FORALL) ? "ALL" : "EXIST";
            fprintf(out, "STP: %s%s\n", indent, op_name);
            print_tree(out, tree->types.quantorType.variable, depth + 1);
            print_tree(out, tree->types.quantorType.formula, depth + 1);
            break;
        }
    }
}

void free_tree(struct node *tree) {
    if (!tree) return;
    
    switch (tree->nodeType) {
        case NODE_BOOL:
        case NODE_NUMBER:
        case NODE_VARIABLE:
            break;
        case NODE_PREDICATE:
            free_tree(tree->types.predicateType.arguments);
            break;
        case NODE_FUNCTION:
            free_tree(tree->types.functionType.arguments);
            break;
        case NODE_UNARY:
            free_tree(tree->types.unaryType.formula);
            break;
        case NODE_BINARY:
            free_tree(tree->types.binaryType.left);
            free_tree(tree->types.binaryType.right);
            break;
        case NODE_QUANTOR:
            free_tree(tree->types.quantorType.variable);
            free_tree(tree->types.quantorType.formula);
            break;
    }
    free(tree);
}
