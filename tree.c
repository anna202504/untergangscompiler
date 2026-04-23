
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symboltable.h"

// Hilfsfunktion: String anhängen (mit Realloc)
static void strappend(char **dest, const char *src) {
    if (!src) return;
    size_t oldlen = *dest ? strlen(*dest) : 0;
    size_t addlen = strlen(src);
    *dest = realloc(*dest, oldlen + addlen + 1);
    strcpy(*dest + oldlen, src);
}

// Hilfsfunktion: Argumentliste als String
static void args_to_string(struct node *args, char **out) {
    if (!args) return;
    if (args->nodeType == NODE_BINARY && args->types.binaryType.op == OP_ARGLIST) {
        args_to_string(args->types.binaryType.left, out);
        strappend(out, ", ");
        args_to_string(args->types.binaryType.right, out);
    } else if (args->nodeType == NODE_ARG) {
        args_to_string(args->types.binaryType.left, out);
        if (args->types.binaryType.right) {
            strappend(out, ", ");
            args_to_string(args->types.binaryType.right, out);
        }
    } else {
        char *tmp = tree_to_pl1_formula(args);
        strappend(out, tmp);
        free(tmp);
    }
}

// Hauptfunktion: Baum zu PL1-Formel
char *tree_to_pl1_formula(struct node *tree) {
    if (!tree) return NULL;
    char *out = calloc(1, 1);
    char buf[64];
    switch (tree->nodeType) {
        case NODE_BOOL:
            strappend(&out, tree->types.boolType.value ? "TRUE" : "FALSE");
            break;
        case NODE_NUMBER:
            snprintf(buf, sizeof(buf), "%d", tree->types.numberType.value);
            strappend(&out, buf);
            break;
        case NODE_VARIABLE:
        case NODE_VAR:
            strappend(&out, tree->types.variableType.entry->identifier);
            break;
        case NODE_PREDICATE:
        case NODE_PRED:
            strappend(&out, tree->types.predicateType.entry->identifier);
            if (tree->types.predicateType.arguments) {
                strappend(&out, "(");
                args_to_string(tree->types.predicateType.arguments, &out);
                strappend(&out, ")");
            }
            break;
        case NODE_FUNCTION:
        case NODE_FUNC:
            strappend(&out, tree->types.functionType.entry->identifier);
            if (tree->types.functionType.arguments) {
                strappend(&out, "(");
                args_to_string(tree->types.functionType.arguments, &out);
                strappend(&out, ")");
            }
            break;
        case NODE_UNARY:
            switch (tree->types.unaryType.op) {
                case OP_NOT:
                    strappend(&out, "~");
                    break;
            }
            {
                char *sub = tree_to_pl1_formula(tree->types.unaryType.formula);
                strappend(&out, "(");
                strappend(&out, sub);
                strappend(&out, ")");
                free(sub);
            }
            break;
        case NODE_BINARY: {
            const char *op = NULL;
            switch (tree->types.binaryType.op) {
                case OP_AND: op = "&"; break;
                case OP_OR: op = "|"; break;
                case OP_IMPLIES: op = "=>"; break;
                case OP_EQUIV: op = "<=>"; break;
                case OP_ARGLIST: // sollte nicht als Formel erscheinen
                    return tree_to_pl1_formula(tree->types.binaryType.left);
            }
            char *l = tree_to_pl1_formula(tree->types.binaryType.left);
            char *r = tree_to_pl1_formula(tree->types.binaryType.right);
            strappend(&out, "(");
            strappend(&out, l);
            strappend(&out, " ");
            strappend(&out, op);
            strappend(&out, " ");
            strappend(&out, r);
            strappend(&out, ")");
            free(l); free(r);
            break;
        }
        case NODE_QUANTOR: {
            const char *q = (tree->types.quantorType.op == OP_FORALL) ? "ALL" : "EX";
            strappend(&out, q);
            strappend(&out, " ");
            char *v = tree_to_pl1_formula(tree->types.quantorType.variable);
            strappend(&out, v);
            free(v);
            strappend(&out, ": ");
            char *f = tree_to_pl1_formula(tree->types.quantorType.formula);
            strappend(&out, f);
            free(f);
            break;
        }
        case NODE_ARG:
            // Argumentkette: nur Argumente ausgeben, ohne Klammern
            args_to_string(tree, &out);
            break;
    }
    return out;
}


// ...existing code...
// Neue Knoten für explizite Baumstruktur (wie im Bild)
struct node* create_arg_node(struct node *left, struct node *right) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_ARG;
    n->types.binaryType.left = left;
    n->types.binaryType.right = right;
    fprintf(stderr, "SYT: ARG Node created\n");
    return n;
}

struct node* create_var_node(struct tableEntry *entry) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_VAR;
    n->types.variableType.entry = entry;
    fprintf(stderr, "SYT: VAR Node created\n");
    return n;
}

struct node* create_pred_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_PRED;
    n->types.predicateType.entry = entry;
    n->types.predicateType.arguments = args;
    fprintf(stderr, "SYT: PRED Node created\n");
    return n;
}

struct node* create_func_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_FUNC;
    n->types.functionType.entry = entry;
    n->types.functionType.arguments = args;
    fprintf(stderr, "SYT: FUNC Node created\n");
    return n;
}
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
    fprintf(stderr, "SYT: Variable Node created\n");
    return n;
}

struct node* create_predicate_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_PREDICATE;
    n->types.predicateType.entry = entry;
    n->types.predicateType.arguments = args;
    if (args) {
        fprintf(stderr, "SYT: Argument Node created\n");
    }
    fprintf(stderr, "SYT: Predicate Node created\n");
    return n;
}

struct node* create_function_node(struct tableEntry *entry, struct node *args) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_FUNCTION;
    n->types.functionType.entry = entry;
    n->types.functionType.arguments = args;
    if (args) {
        fprintf(stderr, "SYT: Argument Node created\n");
    }
    fprintf(stderr, "SYT: Function Node created\n");
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
    
    const char *op_name;
    switch (op) {
        case OP_AND: op_name = "&"; break;
        case OP_OR: op_name = "|"; break;
        case OP_IMPLIES: op_name = "=>"; break;
        case OP_EQUIV: op_name = "<=>"; break;
        case OP_ARGLIST: 
            fprintf(stderr, "SYT: Argument Node added to list\n");
            return n;
        default: op_name = "UNKNOWN"; break;
    }
    fprintf(stderr, "SYT: Binary Node created - Type %s\n", op_name);
    return n;
}

struct node* create_quantor_node(enum quantorOp op, struct tableEntry *var, struct node *formula) {
    struct node *n = malloc(sizeof(struct node));
    n->nodeType = NODE_QUANTOR;
    n->types.quantorType.op = op;
    n->types.quantorType.variable = create_variable_node(var);
    n->types.quantorType.formula = formula;
    
    const char *op_name = (op == OP_FORALL) ? "FORALL" : "EXIST";
    fprintf(stderr, "SYT: Quantor Node created - Type %s\n", op_name);
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
        // Neue explizite Knoten für Bildstruktur
        case NODE_ARG:
            fprintf(out, "%s", indent); // keine STP: Zeile für arg, nur Einrückung
            if (tree->types.binaryType.left)
                print_tree(out, tree->types.binaryType.left, depth + 1);
            if (tree->types.binaryType.right)
                print_tree(out, tree->types.binaryType.right, depth + 1);
            break;
        case NODE_VAR:
            fprintf(out, "STP: %sVariable: %s\n", indent, tree->types.variableType.entry->identifier);
            break;
        case NODE_PRED:
            fprintf(out, "STP: %sPredicate: %s\n", indent, tree->types.predicateType.entry->identifier);
            if (tree->types.predicateType.arguments)
                print_tree(out, tree->types.predicateType.arguments, depth + 1);
            break;
        case NODE_FUNC:
            fprintf(out, "STP: %sFunction: %s\n", indent, tree->types.functionType.entry->identifier);
            if (tree->types.functionType.arguments)
                print_tree(out, tree->types.functionType.arguments, depth + 1);
            break;
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

// Count the number of arguments in an argument list
// An argument list is represented as either:
// - NULL (no arguments, arity 0)
// - A single term node (one argument)
// - A chain of OP_ARGLIST binary nodes (multiple arguments)
int count_arguments(struct node *args) {
    if (!args) {
        return 0;
    }
    // ARGLIST-Kette (alt)
    if (args->nodeType == NODE_BINARY && args->types.binaryType.op == OP_ARGLIST) {
        int count = 1;
        struct node *current = args;
        while (current != NULL && current->nodeType == NODE_BINARY && current->types.binaryType.op == OP_ARGLIST) {
            struct node *left = current->types.binaryType.left;
            if (left && left->nodeType == NODE_BINARY && left->types.binaryType.op == OP_ARGLIST) {
                count++;
                current = left;
            } else {
                count++;
                break;
            }
        }
        return count;
    }
    // Neue arg-Kette (NODE_ARG)
    if (args->nodeType == NODE_ARG) {
        int count = 0;
        struct node *current = args;
        while (current != NULL && current->nodeType == NODE_ARG) {
            if (current->types.binaryType.left)
                count += count_arguments(current->types.binaryType.left);
            current = current->types.binaryType.right;
        }
        if (current)
            count += count_arguments(current);
        return count;
    }
    // Einzelnes Argument
    return 1;
}
