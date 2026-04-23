#include "klammer.h"
#include "symbol_table.h"
#include "tree.h"
#include "optimierung1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct treeNode *eliminateDoubleNegations(struct treeNode *root) {
    struct treeNode *innerNot;
    struct treeNode *replacement;

    if (root == NULL) {
        return NULL;
    }

    root->next = eliminateDoubleNegations(root->next);

    switch (root->nodeType) {
        case NODE_QUANTOR:
            root->treeTypes.quantorType.formula =
                eliminateDoubleNegations(root->treeTypes.quantorType.formula);
            return root;

        case NODE_BINARY_OPERATOR:
            root->treeTypes.binaryType.left =
                eliminateDoubleNegations(root->treeTypes.binaryType.left);
            root->treeTypes.binaryType.right =
                eliminateDoubleNegations(root->treeTypes.binaryType.right);
            return root;

        case NODE_UNARY_OPERATOR:
            root->treeTypes.unaryType.child =
                eliminateDoubleNegations(root->treeTypes.unaryType.child);

            if (root->treeTypes.unaryType.operatorType == UOP_NOT &&
                root->treeTypes.unaryType.child != NULL &&
                root->treeTypes.unaryType.child->nodeType == NODE_UNARY_OPERATOR &&
                root->treeTypes.unaryType.child->treeTypes.unaryType.operatorType == UOP_NOT) {

                innerNot = root->treeTypes.unaryType.child;
                replacement = innerNot->treeTypes.unaryType.child;

                free(innerNot);
                free(root);

                return replacement;
            }

            return root;

        case NODE_PREDICATE:
            root->treeTypes.predicatType.arguments =
                eliminateDoubleNegations(root->treeTypes.predicatType.arguments);
            return root;

        case NODE_FUNCTION:
            root->treeTypes.functionType.arguments =
                eliminateDoubleNegations(root->treeTypes.functionType.arguments);
            return root;

        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
        default:
            return root;
    }
}

struct treeNode *moveNegations(struct treeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    root->next = moveNegations(root->next);

    switch (root->nodeType) {
        case NODE_QUANTOR:
            root->treeTypes.quantorType.formula =
                moveNegations(root->treeTypes.quantorType.formula);
            return root;

        case NODE_BINARY_OPERATOR:
            root->treeTypes.binaryType.left =
                moveNegations(root->treeTypes.binaryType.left);
            root->treeTypes.binaryType.right =
                moveNegations(root->treeTypes.binaryType.right);
            return root;

        case NODE_UNARY_OPERATOR: {
            root->treeTypes.unaryType.child =
                moveNegations(root->treeTypes.unaryType.child);

            if (root->treeTypes.unaryType.operatorType == UOP_NOT &&
                root->treeTypes.unaryType.child != NULL) {

                struct treeNode *child = root->treeTypes.unaryType.child;

                if (child->nodeType == NODE_BINARY_OPERATOR &&
                    child->treeTypes.binaryType.operatorType == BINOP_AND) {

                    fprintf(stderr, "OPT: ~(A & B) -> ~A | ~B\n");

                    struct treeNode *leftNot = makeNode(NODE_UNARY_OPERATOR);
                    leftNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    leftNot->treeTypes.unaryType.child = child->treeTypes.binaryType.left;

                    struct treeNode *rightNot = makeNode(NODE_UNARY_OPERATOR);
                    rightNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    rightNot->treeTypes.unaryType.child = child->treeTypes.binaryType.right;

                    child->treeTypes.binaryType.operatorType = BINOP_OR;
                    child->treeTypes.binaryType.left = moveNegations(leftNot);
                    child->treeTypes.binaryType.right = moveNegations(rightNot);

                    free(root);
                    return child;
                }

                if (child->nodeType == NODE_BINARY_OPERATOR &&
                    child->treeTypes.binaryType.operatorType == BINOP_OR) {

                    fprintf(stderr, "OPT: ~(A | B) -> ~A & ~B\n");

                    struct treeNode *leftNot = makeNode(NODE_UNARY_OPERATOR);
                    leftNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    leftNot->treeTypes.unaryType.child = child->treeTypes.binaryType.left;

                    struct treeNode *rightNot = makeNode(NODE_UNARY_OPERATOR);
                    rightNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    rightNot->treeTypes.unaryType.child = child->treeTypes.binaryType.right;

                    child->treeTypes.binaryType.operatorType = BINOP_AND;
                    child->treeTypes.binaryType.left = moveNegations(leftNot);
                    child->treeTypes.binaryType.right = moveNegations(rightNot);

                    free(root);
                    return child;
                }

                if (child->nodeType == NODE_QUANTOR &&
                    child->treeTypes.quantorType.quantorType == FORALL) {

                    fprintf(stderr, "OPT: ~ALL[x]A -> EXIST[x]~A\n");

                    struct treeNode *newNot = makeNode(NODE_UNARY_OPERATOR);
                    newNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    newNot->treeTypes.unaryType.child = child->treeTypes.quantorType.formula;

                    child->treeTypes.quantorType.quantorType = EXISTS;
                    child->treeTypes.quantorType.formula = moveNegations(newNot);

                    free(root);
                    return child;
                }

                if (child->nodeType == NODE_QUANTOR &&
                    child->treeTypes.quantorType.quantorType == EXISTS) {

                    fprintf(stderr, "OPT: ~EXIST[x]A -> ALL[x]~A\n");

                    struct treeNode *newNot = makeNode(NODE_UNARY_OPERATOR);
                    newNot->treeTypes.unaryType.operatorType = UOP_NOT;
                    newNot->treeTypes.unaryType.child = child->treeTypes.quantorType.formula;

                    child->treeTypes.quantorType.quantorType = FORALL;
                    child->treeTypes.quantorType.formula = moveNegations(newNot);

                    free(root);
                    return child;
                }
            }

            return root;
        }

        case NODE_PREDICATE:
            root->treeTypes.predicatType.arguments =
                moveNegations(root->treeTypes.predicatType.arguments);
            return root;

        case NODE_FUNCTION:
            root->treeTypes.functionType.arguments =
                moveNegations(root->treeTypes.functionType.arguments);
            return root;

        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
        default:
            return root;
    }
}

struct treeNode *replaceImplicationsAndEquivalences(struct treeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    root->next = replaceImplicationsAndEquivalences(root->next);

    switch (root->nodeType) {
        case NODE_QUANTOR:
            root->treeTypes.quantorType.formula =
                replaceImplicationsAndEquivalences(root->treeTypes.quantorType.formula);
            return root;

        case NODE_UNARY_OPERATOR:
            root->treeTypes.unaryType.child =
                replaceImplicationsAndEquivalences(root->treeTypes.unaryType.child);
            return root;

        case NODE_BINARY_OPERATOR: {
            root->treeTypes.binaryType.left =
                replaceImplicationsAndEquivalences(root->treeTypes.binaryType.left);
            root->treeTypes.binaryType.right =
                replaceImplicationsAndEquivalences(root->treeTypes.binaryType.right);

            struct treeNode *left = root->treeTypes.binaryType.left;
            struct treeNode *right = root->treeTypes.binaryType.right;

            if (root->treeTypes.binaryType.operatorType == BINOP_IMPLIES) {
                fprintf(stderr, "OPT: A -> B => ~A | B\n");

                struct treeNode *notLeft = makeNode(NODE_UNARY_OPERATOR);
                notLeft->treeTypes.unaryType.operatorType = UOP_NOT;
                notLeft->treeTypes.unaryType.child = left;

                root->treeTypes.binaryType.operatorType = BINOP_OR;
                root->treeTypes.binaryType.left = notLeft;
                root->treeTypes.binaryType.right = right;

                return root;
            }

            if (root->treeTypes.binaryType.operatorType == BINOP_IFF) {
                fprintf(stderr, "OPT: A <-> B => (A & B) | (~A & ~B)\n");

                struct treeNode *leftCopy = copyTree(left);
                struct treeNode *rightCopy = copyTree(right);

                struct treeNode *and1 = makeNode(NODE_BINARY_OPERATOR);
                and1->treeTypes.binaryType.operatorType = BINOP_AND;
                and1->treeTypes.binaryType.left = left;
                and1->treeTypes.binaryType.right = right;

                struct treeNode *notLeft = makeNode(NODE_UNARY_OPERATOR);
                notLeft->treeTypes.unaryType.operatorType = UOP_NOT;
                notLeft->treeTypes.unaryType.child = leftCopy;

                struct treeNode *notRight = makeNode(NODE_UNARY_OPERATOR);
                notRight->treeTypes.unaryType.operatorType = UOP_NOT;
                notRight->treeTypes.unaryType.child = rightCopy;

                struct treeNode *and2 = makeNode(NODE_BINARY_OPERATOR);
                and2->treeTypes.binaryType.operatorType = BINOP_AND;
                and2->treeTypes.binaryType.left = notLeft;
                and2->treeTypes.binaryType.right = notRight;

                root->treeTypes.binaryType.operatorType = BINOP_OR;
                root->treeTypes.binaryType.left = and1;
                root->treeTypes.binaryType.right = and2;

                return root;
            }

            return root;
        }

        case NODE_PREDICATE:
            root->treeTypes.predicatType.arguments =
                replaceImplicationsAndEquivalences(root->treeTypes.predicatType.arguments);
            return root;

        case NODE_FUNCTION:
            root->treeTypes.functionType.arguments =
                replaceImplicationsAndEquivalences(root->treeTypes.functionType.arguments);
            return root;

        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
        default:
            return root;
    }
}

static void freeTree(struct treeNode *root) {
    deleteTree(root);
}

struct treeNode *evaluateBooleanOperations(struct treeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    root->next = evaluateBooleanOperations(root->next);

    switch (root->nodeType) {
        case NODE_QUANTOR:
            root->treeTypes.quantorType.formula =
                evaluateBooleanOperations(root->treeTypes.quantorType.formula);
            return root;

        case NODE_UNARY_OPERATOR: {
            root->treeTypes.unaryType.child =
                evaluateBooleanOperations(root->treeTypes.unaryType.child);

            struct treeNode *child = root->treeTypes.unaryType.child;

            if (root->treeTypes.unaryType.operatorType == UOP_NOT &&
                child != NULL &&
                child->nodeType == NODE_BOOL) {

                fprintf(stderr, "OPT: ~%s -> %s\n",
                        child->treeTypes.boolType.value ? "TRUE" : "FALSE",
                        child->treeTypes.boolType.value ? "FALSE" : "TRUE");

                child->treeTypes.boolType.value = !child->treeTypes.boolType.value;
                free(root);
                return child;
            }

            return root;
        }

        case NODE_BINARY_OPERATOR: {
            root->treeTypes.binaryType.left =
                evaluateBooleanOperations(root->treeTypes.binaryType.left);
            root->treeTypes.binaryType.right =
                evaluateBooleanOperations(root->treeTypes.binaryType.right);

            struct treeNode *left = root->treeTypes.binaryType.left;
            struct treeNode *right = root->treeTypes.binaryType.right;
            int op = root->treeTypes.binaryType.operatorType;

            if (op == BINOP_AND) {
                if (left != NULL && left->nodeType == NODE_BOOL) {
                    if (left->treeTypes.boolType.value == 1) {
                        fprintf(stderr, "OPT: TRUE & A -> A\n");
                        root->treeTypes.binaryType.left = NULL;
                        freeTree(left);
                        struct treeNode *result = right;
                        free(root);
                        return result;
                    } else {
                        fprintf(stderr, "OPT: FALSE & A -> FALSE\n");
                        root->treeTypes.binaryType.left = NULL;
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(right);
                        struct treeNode *result = left;
                        free(root);
                        return result;
                    }
                }

                if (right != NULL && right->nodeType == NODE_BOOL) {
                    if (right->treeTypes.boolType.value == 1) {
                        fprintf(stderr, "OPT: A & TRUE -> A\n");
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(right);
                        struct treeNode *result = left;
                        free(root);
                        return result;
                    } else {
                        fprintf(stderr, "OPT: A & FALSE -> FALSE\n");
                        root->treeTypes.binaryType.left = NULL;
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(left);
                        struct treeNode *result = right;
                        free(root);
                        return result;
                    }
                }
            }

            if (op == BINOP_OR) {
                if (left != NULL && left->nodeType == NODE_BOOL) {
                    if (left->treeTypes.boolType.value == 1) {
                        fprintf(stderr, "OPT: TRUE | A -> TRUE\n");
                        root->treeTypes.binaryType.left = NULL;
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(right);
                        struct treeNode *result = left;
                        free(root);
                        return result;
                    } else {
                        fprintf(stderr, "OPT: FALSE | A -> A\n");
                        root->treeTypes.binaryType.left = NULL;
                        freeTree(left);
                        struct treeNode *result = right;
                        free(root);
                        return result;
                    }
                }

                if (right != NULL && right->nodeType == NODE_BOOL) {
                    if (right->treeTypes.boolType.value == 1) {
                        fprintf(stderr, "OPT: A | TRUE -> TRUE\n");
                        root->treeTypes.binaryType.left = NULL;
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(left);
                        struct treeNode *result = right;
                        free(root);
                        return result;
                    } else {
                        fprintf(stderr, "OPT: A | FALSE -> A\n");
                        root->treeTypes.binaryType.right = NULL;
                        freeTree(right);
                        struct treeNode *result = left;
                        free(root);
                        return result;
                    }
                }
            }

            return root;
        }

        case NODE_PREDICATE:
            root->treeTypes.predicatType.arguments =
                evaluateBooleanOperations(root->treeTypes.predicatType.arguments);
            return root;

        case NODE_FUNCTION:
            root->treeTypes.functionType.arguments =
                evaluateBooleanOperations(root->treeTypes.functionType.arguments);
            return root;

        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
        default:
            return root;
    }
}