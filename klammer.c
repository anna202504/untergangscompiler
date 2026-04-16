#include <string.h>
#include "klammer.h"

enum {
	PREC_IFF = 1,
	PREC_IMPLIES = 2,
	PREC_OR = 3,
	PREC_AND = 4,
	PREC_NOT = 5,
	PREC_ATOM = 6
};

static int getNodePrecedence(const struct treeNode *node) {
	if (node == NULL) {
		return 0;
	}

	switch (node->nodeType) {
		case NODE_BINARY_OPERATOR:
			switch (node->treeTypes.binaryType.operatorType) {
				case BINOP_OR:
					return PREC_OR;
				case BINOP_AND:
					return PREC_AND;
				case BINOP_IMPLIES:
					return PREC_IMPLIES;
				case BINOP_IFF:
					return PREC_IFF;
				default:
					return PREC_ATOM;
			}
		case NODE_UNARY_OPERATOR:
			return PREC_NOT;
		case NODE_QUANTOR:
		case NODE_PREDICATE:
		case NODE_FUNCTION:
		case NODE_VARIABLE:
		case NODE_NUMBER:
		case NODE_BOOL:
			return PREC_ATOM;
		default:
			return 0;
	}
}

static void printFormulaNode(const struct treeNode *node, int parentPrecedence, int isRightChild);

static void printArgumentList(const struct treeNode *node) {
	const struct treeNode *current = node;

	while (current != NULL) {
		if (current != node) {
			fprintf(stdout, ",");
		}

		printFormulaNode(current, 0, 0);
		current = current->next;
	}
}

static void printDeclarationRecursive(const struct tableEntry *entry) {
	if (entry == NULL) {
		return;
	}

	printDeclarationRecursive(entry->next);

	if (strcmp(entry->type, "predicate") == 0) {
		fprintf(stdout, "DECLARE PREDICATE %s : %d\n", entry->identifier, entry->arity);
	} else if (strcmp(entry->type, "function") == 0) {
		fprintf(stdout, "DECLARE FUNCTION %s : %d\n", entry->identifier, entry->arity);
	} else if (strcmp(entry->type, "variable") == 0) {
		fprintf(stdout, "DECLARE VARIABLE %s : int\n", entry->identifier);
	}
}


static void printFormulaNode(const struct treeNode *node, int parentPrecedence, int isRightChild) {
	if (node == NULL) {
		return;
	}

	int nodePrecedence = getNodePrecedence(node);
	int needsParentheses = 0;

	if (parentPrecedence > 0) {
		if (nodePrecedence < parentPrecedence) {
			needsParentheses = 1;
		} else if (nodePrecedence == parentPrecedence && isRightChild && node->nodeType == NODE_BINARY_OPERATOR) {
			needsParentheses = 1;
		}
	}

	if (needsParentheses) {
		fprintf(stdout, "(");
	}

	switch (node->nodeType) {
		case NODE_QUANTOR:
			if (node->treeTypes.quantorType.quantorType == FORALL) {
				fprintf(stdout, "ALL[%s]", node->treeTypes.quantorType.var->identifier);
			} else {
				fprintf(stdout, "EXIST[%s]", node->treeTypes.quantorType.var->identifier);
			}
			printFormulaNode(node->treeTypes.quantorType.formula, PREC_NOT, 1);
			break;
		case NODE_BINARY_OPERATOR:
			printFormulaNode(node->treeTypes.binaryType.left, nodePrecedence, 0);
			switch (node->treeTypes.binaryType.operatorType) {
				case BINOP_AND:
					fprintf(stdout, " & ");
					break;
				case BINOP_OR:
					fprintf(stdout, " | ");
					break;
				case BINOP_IMPLIES:
					fprintf(stdout, " -> ");
					break;
				case BINOP_IFF:
					fprintf(stdout, " <-> ");
					break;
				default:
					break;
			}
			printFormulaNode(node->treeTypes.binaryType.right, nodePrecedence, 1);
			break;
		case NODE_UNARY_OPERATOR:
			fprintf(stdout, " ~ ");
			printFormulaNode(node->treeTypes.unaryType.child, PREC_NOT, 1);
			break;
		case NODE_PREDICATE:
			fprintf(stdout, "%s(", node->treeTypes.predicatType.entry->identifier);
			printArgumentList(node->treeTypes.predicatType.arguments);
			fprintf(stdout, ")");
			break;
		case NODE_FUNCTION:
			if (node->treeTypes.functionType.entry->arity == 0) {
				fprintf(stdout, "%s", node->treeTypes.functionType.entry->identifier);
			} else {
				fprintf(stdout, "%s(", node->treeTypes.functionType.entry->identifier);
				printArgumentList(node->treeTypes.functionType.arguments);
				fprintf(stdout, ")");
			}
			break;
		case NODE_VARIABLE:
			fprintf(stdout, "%s", node->treeTypes.variableType.entry->identifier);
			break;
		case NODE_NUMBER:
			fprintf(stdout, "%d", node->treeTypes.numberType.value);
			break;
		case NODE_BOOL:
			fprintf(stdout, "%s", node->treeTypes.boolType.value ? "TRUE" : "FALSE");
			break;
		default:
			break;
	}

	if (needsParentheses) {
		fprintf(stdout, ")");
	}
}






void printDeclarationsFromSymbolTable(struct tableEntry *head) {
	printDeclarationRecursive(head);
}

void printFormulaFromSyntaxTree(struct treeNode *root) {
	printFormulaNode(root, 0, 0);
}
