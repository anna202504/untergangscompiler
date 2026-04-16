#include  <stdio.h>
#include  <stdlib.h>
#include  "tree.h"

struct treeNode *makeNode(int nodeType) {
    struct treeNode *newNode = malloc(sizeof(struct treeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new tree node.\n");
        exit(EXIT_FAILURE);
    }

    newNode->nodeType = nodeType;
    newNode->next = NULL;

    switch (nodeType) {
        case NODE_QUANTOR: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_BINARY_OPERATOR: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_UNARY_OPERATOR: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_PREDICATE: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_FUNCTION   : 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_VARIABLE: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_NUMBER: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        case NODE_BOOL: 
            fprintf(stderr, "Creating node type %d\n", nodeType);
            break;
        default:
            fprintf(stderr, "Error: Invalid node type %d.\n", nodeType);
            free(newNode);
            exit(EXIT_FAILURE);
        }
    return newNode;
}


struct treeNode *deleteTree(struct treeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    switch (root->nodeType) {
        case NODE_QUANTOR: 
            deleteTree(root->treeTypes.quantorType.formula);
            break;
        case NODE_BINARY_OPERATOR:
            deleteTree(root->treeTypes.binaryType.left);
            deleteTree(root->treeTypes.binaryType.right);
            break;
        case NODE_UNARY_OPERATOR: 
            deleteTree(root->treeTypes.unaryType.child);
            break;
        case NODE_PREDICATE: 
            deleteTree(root->treeTypes.predicatType.arguments);
            break;
        case NODE_FUNCTION: 
            deleteTree(root->treeTypes.functionType.arguments);
            break;
        default:
            break; // Variable, Number, Bool have no children
    }

    deleteTree(root->next);

    free(root);
    return NULL;
}

void printTree(struct treeNode *root, int level) {
    if (root == NULL) {
        return;
    }

    fprintf(stderr, "STP: ");
    for (int i = 0; i < level; i++) {
        fprintf(stderr, ".");
    }


    switch (root->nodeType) {
        case NODE_QUANTOR: 
            if (root->treeTypes.quantorType.quantorType == FORALL) {
                fprintf(stderr, "ALL\n");
            } else {
                fprintf(stderr, "EXIST\n");
            }
            fprintf(stderr, "STP: ");
            for (int i = 0; i <= level; i++) {
                fprintf(stderr, ".");
            }
            fprintf(stderr, "Variable: %s\n", root->treeTypes.quantorType.var->identifier);
            printTree(root->treeTypes.quantorType.formula, level + 1);
            break;
        case NODE_BINARY_OPERATOR:
            switch (root->treeTypes.binaryType.operatorType) {
                case BINOP_AND:
                    fprintf(stderr, "AND\n");
                    break;
                case BINOP_OR:
                    fprintf(stderr, "OR\n");
                    break;
                case BINOP_IMPLIES:
                    fprintf(stderr, "IMPLICATION\n");
                    break;
                case BINOP_IFF:
                    fprintf(stderr, "EQUIVALENT\n");
                    break;
                default:
                    fprintf(stderr, "Error: Invalid binary operator type %d.\n", root->treeTypes.binaryType.operatorType);
            }
            printTree(root->treeTypes.binaryType.left, level + 1);
            printTree(root->treeTypes.binaryType.right, level + 1);
            break;
        case NODE_UNARY_OPERATOR: 
            switch (root->treeTypes.unaryType.operatorType) {
                case UOP_NOT:
                    fprintf(stderr, "NOT\n");
                    break;
                default:
                    fprintf(stderr, "Error: Invalid unary operator type %d.\n", root->treeTypes.unaryType.operatorType);
            }
            printTree(root->treeTypes.unaryType.child, level + 1);
            break;
        case NODE_PREDICATE: 
            fprintf(stderr, "Predicate: %s\n", root->treeTypes.predicatType.entry->identifier);
            printTree(root->treeTypes.predicatType.arguments, level + 1);
            break;
        case NODE_FUNCTION: 
            fprintf(stderr, "Function: %s\n", root->treeTypes.functionType.entry->identifier);
            printTree(root->treeTypes.functionType.arguments, level + 1);
            break;
        case NODE_VARIABLE: 
            fprintf(stderr, "Variable: %s\n", root->treeTypes.variableType.entry->identifier);
            break;
        case NODE_NUMBER: 
            fprintf(stderr, "Number: %d\n", root->treeTypes.numberType.value);
            break;
        case NODE_BOOL: 
            fprintf(stderr, "%s\n", root->treeTypes.boolType.value ? "TRUE" : "FALSE");
            break;
        default:
            fprintf(stderr, "Error: Invalid node type %d.\n", root->nodeType);
    }

    printTree(root->next, level);
}

struct treeNode *copyTree(struct treeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    struct treeNode *newNode = makeNode(root->nodeType);
    newNode->next = copyTree(root->next);

    switch (root->nodeType) {
        case NODE_QUANTOR: 
            newNode->treeTypes.quantorType.var = root->treeTypes.quantorType.var; // Shallow copy
            newNode->treeTypes.quantorType.formula = copyTree(root->treeTypes.quantorType.formula);
            newNode->treeTypes.quantorType.quantorType = root->treeTypes.quantorType.quantorType;
            break;
        case NODE_BINARY_OPERATOR:
            newNode->treeTypes.binaryType.left = copyTree(root->treeTypes.binaryType.left);
            newNode->treeTypes.binaryType.right = copyTree(root->treeTypes.binaryType.right);
            newNode->treeTypes.binaryType.operatorType = root->treeTypes.binaryType.operatorType;
            break;
        case NODE_UNARY_OPERATOR: 
            newNode->treeTypes.unaryType.child = copyTree(root->treeTypes.unaryType.child);
            newNode->treeTypes.unaryType.operatorType = root->treeTypes.unaryType.operatorType;
            break;
        case NODE_PREDICATE: 
            newNode->treeTypes.predicatType.entry = root->treeTypes.predicatType.entry; // Shallow copy
            newNode->treeTypes.predicatType.arguments = copyTree(root->treeTypes.predicatType.arguments);
            break;
        case NODE_FUNCTION: 
            newNode->treeTypes.functionType.entry = root->treeTypes.functionType.entry; // Shallow copy
            newNode->treeTypes.functionType.arguments = copyTree(root->treeTypes.functionType.arguments);
            break;
        case NODE_VARIABLE: 
            newNode->treeTypes.variableType.entry = root->treeTypes.variableType.entry; // Shallow copy
            break;
        case NODE_NUMBER: 
            newNode->treeTypes.numberType.value = root->treeTypes.numberType.value;
            break;
        case NODE_BOOL: 
            newNode->treeTypes.boolType.value = root->treeTypes.boolType.value;
            break;
        default:
            fprintf(stderr, "Error: Invalid node type %d.\n", root->nodeType);
    }

    return newNode;
}

int countArguments(struct treeNode *argList) {
    int count = 0;
    struct treeNode *current = argList;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
