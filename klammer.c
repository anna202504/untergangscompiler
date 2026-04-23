#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "klammer.h"
#include "tree.h"


// Symboltabelle 
void printDeclaration(struct tableEntry *entry) {
    if (entry == NULL) {
		return;
	}
    printDeclaration(entry->next);

    if (strcmp(entry->type, "variable") == 0) {
        fprintf(stdout, "DECLARE VARIABLE %s : int\n", entry->identifier);
    } else if (strcmp(entry->type, "predicate") == 0) {
        fprintf(stdout, "DECLARE PREDICATE %s : %d\n", entry->identifier, entry->arity);
    } else if (strcmp(entry->type, "function") == 0) {
        fprintf(stdout, "DECLARE FUNCTION %s : %d\n", entry->identifier, entry->arity);
    }
}

static int getPrecendence(struct treeNode *node) {
    if (node == NULL) return 100;

    switch (node->nodeType) {
        case NODE_BINARY_OPERATOR:
            switch (node->treeTypes.binaryType.operatorType) {
                case BINOP_IFF: return 1;
                case BINOP_IMPLIES: return 2;
                case BINOP_OR: return 3;
                case BINOP_AND: return 4;
            }
            return 0;
        case NODE_UNARY_OPERATOR:
            return 5;
        case NODE_QUANTOR:
            return 6;
        case NODE_PREDICATE:
        case NODE_FUNCTION:
        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
            return 7;
        }   
    return 0;
}
// Syntaxbaum
void printFormulaRec(struct treeNode *root, int parentPrecedence) {
    if (root == NULL) return;

    int myPrecedence = getPrecendence(root);
    int needParens = myPrecedence < parentPrecedence;

    if (needParens) {
        fprintf(stdout, "(");
    }

    switch (root->nodeType) {
        case NODE_QUANTOR: {
            struct treeNode *f = root->treeTypes.quantorType.formula;

            if (root->treeTypes.quantorType.quantorType == FORALL) {
                fprintf(stdout, "ALL[%s]", root->treeTypes.quantorType.var->identifier);
            } else {
                fprintf(stdout, "EXIST[%s]", root->treeTypes.quantorType.var->identifier);
            }

            if (f != NULL) {
                if (f->nodeType == NODE_BINARY_OPERATOR) {
                    fprintf(stdout, "(");
                    printFormulaRec(f, 0);
                    fprintf(stdout, ")");
                } else if (f->nodeType == NODE_QUANTOR ||
                        f->nodeType == NODE_UNARY_OPERATOR ||
                        f->nodeType == NODE_PREDICATE) {
                    printFormulaRec(f, 0);
                } else {
                    fprintf(stdout, " ");
                    printFormulaRec(f, 0);
                }
            }
            break;
        }

        case NODE_BINARY_OPERATOR: {
            struct treeNode *left  = root->treeTypes.binaryType.left;
            struct treeNode *right = root->treeTypes.binaryType.right;

            if (needsParensInBinary(left, root, 0)) {
                fprintf(stdout, "(");
                printFormulaRec(left, 0);
                fprintf(stdout, ")");
            } else {
                printFormulaRec(left, myPrecedence);
            }

            switch (root->treeTypes.binaryType.operatorType) {
                case BINOP_AND:     fprintf(stdout, " & ");   break;
                case BINOP_OR:      fprintf(stdout, " | ");   break;
                case BINOP_IMPLIES: fprintf(stdout, " -> ");  break;
                case BINOP_IFF:     fprintf(stdout, " <-> "); break;
            }

            if (needsParensInBinary(right, root, 1)) {
                fprintf(stdout, "(");
                printFormulaRec(right, 0);
                fprintf(stdout, ")");
            } else {
                printFormulaRec(right, myPrecedence);
            }
            break;
        }

        case NODE_UNARY_OPERATOR: {
            struct treeNode *child = root->treeTypes.unaryType.child;

            if (child != NULL &&
                (child->nodeType == NODE_BINARY_OPERATOR ||
                 child->nodeType == NODE_QUANTOR)) {
                fprintf(stdout, "~");
            } else {
                fprintf(stdout, "~ ");
            }

            printFormulaRec(child, myPrecedence);
            break;
        }

        case NODE_PREDICATE: {
            fprintf(stdout, "%s(", root->treeTypes.predicatType.entry->identifier);
            struct treeNode *arg = root->treeTypes.predicatType.arguments;
            while (arg != NULL) {
                printFormulaRec(arg, 0);
                arg = arg->next;
                if (arg != NULL) fprintf(stdout, ",");
            }
            fprintf(stdout, ")");
            break;
        }

        case NODE_FUNCTION: {
            fprintf(stdout, "%s", root->treeTypes.functionType.entry->identifier);
            struct treeNode *arg = root->treeTypes.functionType.arguments;
            if (arg != NULL) {
                fprintf(stdout, "(");
                while (arg != NULL) {
                    printFormulaRec(arg, 0);
                    arg = arg->next;
                    if (arg != NULL) fprintf(stdout, ",");
                }
                fprintf(stdout, ")");
            }
            break;
        }

        case NODE_VARIABLE:
            fprintf(stdout, "%s", root->treeTypes.variableType.entry->identifier);
            break;

        case NODE_NUMBER:
            fprintf(stdout, "%d", root->treeTypes.numberType.value);
            break;

        case NODE_BOOL:
            fprintf(stdout, "%s", root->treeTypes.boolType.value ? "TRUE" : "FALSE");
            break;
    }

    if (needParens) {
        fprintf(stdout, ")");
    }
}

void printFormula(struct treeNode *root) {
    printFormulaRec(root, 0);
}


// Hilfsfunktion um zu entscheiden, ob bei einem Kind eines binären Operators Klammern nötig sind
static int needsParensInBinary(struct treeNode *child, struct treeNode *parent, int isRightChild) {
    if (child == NULL || parent == NULL) return 0;
    if (parent->nodeType != NODE_BINARY_OPERATOR) return 0;

    int childPrec = getPrecendence(child);
    int parentPrec = getPrecendence(parent);

    if (childPrec < parentPrec) {
        return 1;
    }

    if (childPrec > parentPrec) {
        return 0;
    }

    if (child->nodeType == NODE_BINARY_OPERATOR) {
        int parentOp = parent->treeTypes.binaryType.operatorType;
        int childOp  = child->treeTypes.binaryType.operatorType;

        if (parentOp == BINOP_IFF || parentOp == BINOP_IMPLIES) {
            return 1;
        }

        if (parentOp != childOp) {
            return 1;
        }
    }

    return 0;
}




