#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int countArguments(struct node *head)
{
    int count = 0;
    struct node *cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}


struct node *makeNode(int nodeType)
{
    struct node *newNode = malloc(sizeof(struct node));
    if (!newNode)
        return NULL;

    newNode->nodeType = nodeType;
    newNode->next = NULL;
    return newNode;
}

struct node *copyNode(struct node *originalNode)
{
    if (originalNode == NULL)
        return NULL;

    struct node *copy = makeNode(originalNode->nodeType);
    if (!copy)
        return NULL;

    switch (originalNode->nodeType)
    {
        case QType:
            copy->qType.quantorOperator = originalNode->qType.quantorOperator;
            copy->qType.quantorVariable = copyNode(originalNode->qType.quantorVariable);
            copy->qType.quantorBody = copyNode(originalNode->qType.quantorBody);
            break;

        case BType:
            copy->bType.binaryOperator = originalNode->bType.binaryOperator;
            copy->bType.leftnode = copyNode(originalNode->bType.leftnode);
            copy->bType.rightnode = copyNode(originalNode->bType.rightnode);
            break;

        case UType:
            copy->uType.unaryOperator = originalNode->uType.unaryOperator;
            copy->uType.operand = copyNode(originalNode->uType.operand);
            break;

        case PType:
            copy->pType.symbolTableEntry = originalNode->pType.symbolTableEntry;
            copy->pType.arguments = copyNode(originalNode->pType.arguments);
            break;

        case FType:
            copy->fType.symbolTableEntry = originalNode->fType.symbolTableEntry;
            copy->fType.arguments = copyNode(originalNode->fType.arguments);
            break;

        case VType:
            copy->vType.symbolTableEntry = originalNode->vType.symbolTableEntry;
            break;

        case BooType:
            copy->booType.value = originalNode->booType.value;
            break;

        case NType:
            copy->nType.value = originalNode->nType.value;
            break;
    }

    copy->next = copyNode(originalNode->next);

    return copy;
}

void deleteNode(struct node *node)
{
    if (node == NULL)
        return;

    switch (node->nodeType)
    {
        case QType:
            deleteNode(node->qType.quantorVariable);
            deleteNode(node->qType.quantorBody);
            break;

        case BType:
            deleteNode(node->bType.leftnode);
            deleteNode(node->bType.rightnode);
            break;

        case UType:
            deleteNode(node->uType.operand);
            break;

        case PType:
            deleteNode(node->pType.arguments);
            break;

        case FType:
            deleteNode(node->fType.arguments);
            break;

        case VType:
        case BooType:
        case NType:
            break;
    }

    deleteNode(node->next);

    free(node);
}

int getArgumentCount(struct node *arguments)
{
    int count = 0;
    struct node *current = arguments;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

static void printIndent(int indentLevel)
{
    fprintf(stderr, "STP: ");
    for (int i = 0; i < indentLevel; i++)
        fputc('.', stderr);
}

static void printNodeList(struct node *node, int indentLevel)
{
    struct node *current = node;
    while (current != NULL)
    {
        printNode(current, indentLevel);
        current = current->next;
    }
}

void printNode(struct node *node, int indentLevel)
{
    if (node == NULL)
    {
        printIndent(indentLevel);
        fprintf(stderr, "NULL\n");
        return;
    }

    printIndent(indentLevel);

    switch (node->nodeType)
    {
        case QType:
            fprintf(stderr, "%s\n",
                    node->qType.quantorOperator == FORALL ? "ALL" : "EXIST");
            printNode(node->qType.quantorVariable, indentLevel + 1);
            printNode(node->qType.quantorBody, indentLevel + 1);
            break;

        case BType:
            switch (node->bType.binaryOperator)
            {
                case B_AND:
                    fprintf(stderr, "AND\n");
                    break;
                case B_OR:
                    fprintf(stderr, "OR\n");
                    break;
                case B_IMPLIES:
                    fprintf(stderr, "IMPLIES\n");
                    break;
                case B_EQUIV:
                    fprintf(stderr, "EQUIV\n");
                    break;
                default:
                    fprintf(stderr, "BINARY\n");
                    break;
            }
            printNode(node->bType.leftnode, indentLevel + 1);
            printNode(node->bType.rightnode, indentLevel + 1);
            break;

        case UType:
            fprintf(stderr, "%s\n", node->uType.unaryOperator == U_NOT ? "NOT" : "UNARY");
            printNode(node->uType.operand, indentLevel + 1);
            break;

        case PType:
            fprintf(stderr, "Predicate: %s\n",
                    node->pType.symbolTableEntry ? node->pType.symbolTableEntry->identifier : "<unknown>");
            printNodeList(node->pType.arguments, indentLevel + 1);
            break;

        case FType:
            fprintf(stderr, "Function: %s\n",
                    node->fType.symbolTableEntry ? node->fType.symbolTableEntry->identifier : "<unknown>");
            printNodeList(node->fType.arguments, indentLevel + 1);
            break;

        case VType:
            fprintf(stderr, "Variable: %s\n",
                    node->vType.symbolTableEntry ? node->vType.symbolTableEntry->identifier : "<unknown>");
            break;

        case BooType:
            fprintf(stderr, "Boolean: %s\n", node->booType.value ? "TRUE" : "FALSE");
            break;

        case NType:
            fprintf(stderr, "Number: %d\n", node->nType.value);
            break;

        default:
            fprintf(stderr, "Unknown node type\n");
            break;
    }
}