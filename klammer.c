#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "klammer.h"
#include "tree.h"


// Symboltabelle 
// Function that reeds the entries of the symbol table and prints them to the console as the original input (declarations)
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

// Syntaxbaum
// Function that reeds the syntax tree and prints it to the console as prolog formula
void printFormula(struct treeNode *root) {
    if (root == NULL) return;

    switch (root->nodeType) {
        case NODE_QUANTOR:
            if (root->treeTypes.quantorType.quantorType == FORALL) {
                fprintf(stdout, "ALL[%s] ", root->treeTypes.quantorType.var->identifier);
            } else {
                fprintf(stdout, "EXIST[%s] ", root->treeTypes.quantorType.var->identifier);
            }
            printFormula(root->treeTypes.quantorType.formula);
            break;
        case NODE_BINARY_OPERATOR:
            printFormula(root->treeTypes.binaryType.left);
            switch (root->treeTypes.binaryType.operatorType) {
                case BINOP_AND: fprintf(stdout, " & "); break;
                case BINOP_OR: fprintf(stdout, " | "); break;
                case BINOP_IMPLIES: fprintf(stdout,     " -> "); break;
                case BINOP_IFF: fprintf(stdout, " <-> "); break;
            }
            printFormula(root->treeTypes.binaryType.right);
            break;
        case NODE_UNARY_OPERATOR:
            fprintf(stdout, "~ ");
            printFormula(root->treeTypes.unaryType.child);
            break;
        case NODE_PREDICATE:
            fprintf(stdout, "%s(", root->treeTypes.predicatType.entry->identifier);
            struct treeNode *arg = root->treeTypes.predicatType.arguments;
            while (arg != NULL) {
                printFormula(arg);
                arg = arg->next;
                if (arg != NULL) fprintf(stdout, ",");
            }
            fprintf(stdout, ")");
            break;
        case NODE_FUNCTION: {
            fprintf(stdout, "%s", root->treeTypes.functionType.entry->identifier);
            struct treeNode *arg = root->treeTypes.functionType.arguments;
            if (arg != NULL) {
                fprintf(stdout, "(");
                while (arg != NULL) {
                    printFormula(arg);
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
}




