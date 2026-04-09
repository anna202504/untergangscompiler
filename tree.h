#ifndef TREE_H
#define TREE_H

#include "symboltable.h"
#include <stdbool.h>

enum nodeType {PType, FType, VType, QType, BType, UType, BooType, NType};

enum quantorOp {FORALL, EXISTS};
enum binaryOp {B_AND, B_OR, B_IMPLIES, B_EQUIV};
enum unaryOp {U_NOT};


struct node 
{

    int nodeType; 
    union
    {

        struct qType
        {
            enum quantorOp quantorOperator;
            struct node *quantorVariable;
            struct node *quantorBody;
        } qType;


        struct bType
        {
            enum binaryOp binaryOperator;
            struct node *leftnode; 
            struct node *rightnode;

        } bType;   
         
        struct uType
        {
            enum unaryOp unaryOperator;
            struct node *operand;
        } uType;       
        
        struct pType
        {
            struct tableEntry *symbolTableEntry;
            struct node *arguments;
        } pType;


        struct fType
        {
          struct tableEntry *symbolTableEntry; 
                    struct node *arguments;
        } fType;

        struct vType
        {
            struct tableEntry *symbolTableEntry;
        } vType;

        struct booType
        {
            bool value;
        } booType;

        struct nType
        {
            int value;
        } nType;

 
    };

    struct node *next;
    


};
struct node *makeNode(int nodeType);
struct node *copyNode(struct node *originalNode);
void deleteNode(struct node *node);

void printNode(struct node *node, int indentLevel);



#endif