#include <stdio.h>
#include <stdlib.h>

#include "optimierung.h"


// Aufgabe 1: Doppelte Negationen eliminieren
struct treeNode *eliminateDoubleNegations(struct treeNode *root) {
	struct treeNode *innerNot;
	struct treeNode *replacement;

	if (root == NULL) {
		return NULL;
	}

	root->next = eliminateDoubleNegations(root->next);

	switch (root->nodeType) {
		case NODE_QUANTOR:
			root->treeTypes.quantorType.formula = eliminateDoubleNegations(root->treeTypes.quantorType.formula);
			return root;

		case NODE_BINARY_OPERATOR:
			root->treeTypes.binaryType.left = eliminateDoubleNegations(root->treeTypes.binaryType.left);
			root->treeTypes.binaryType.right = eliminateDoubleNegations(root->treeTypes.binaryType.right);
			return root;

		case NODE_UNARY_OPERATOR:
			root->treeTypes.unaryType.child = eliminateDoubleNegations(root->treeTypes.unaryType.child);

			if (root->treeTypes.unaryType.operatorType == UOP_NOT
				&& root->treeTypes.unaryType.child != NULL
				&& root->treeTypes.unaryType.child->nodeType == NODE_UNARY_OPERATOR
				&& root->treeTypes.unaryType.child->treeTypes.unaryType.operatorType == UOP_NOT) {

				innerNot = root->treeTypes.unaryType.child;
				replacement = innerNot->treeTypes.unaryType.child;

				free(innerNot);
				free(root);

				return replacement;
			}

			return root;

		case NODE_PREDICATE:
			root->treeTypes.predicatType.arguments = eliminateDoubleNegations(root->treeTypes.predicatType.arguments);
			return root;

		case NODE_FUNCTION:
			root->treeTypes.functionType.arguments = eliminateDoubleNegations(root->treeTypes.functionType.arguments);
			return root;

		case NODE_VARIABLE:
		case NODE_NUMBER:
		case NODE_BOOL:
		default:
			return root;
	}
}


// Aufgabe 2: Negationen zu Prädikaten pushen



static struct treeNode *makeUnaryNot(struct treeNode *child) {
	struct treeNode *node = makeNode(NODE_UNARY_OPERATOR);
	node->treeTypes.unaryType.operatorType = UOP_NOT;
	node->treeTypes.unaryType.child = child;
	return node;
}

static struct treeNode *makeBinaryNode(enum BinaryOperatorType operatorType,
								   struct treeNode *left,
								   struct treeNode *right) {
	struct treeNode *node = makeNode(NODE_BINARY_OPERATOR);
	node->treeTypes.binaryType.operatorType = operatorType;
	node->treeTypes.binaryType.left = left;
	node->treeTypes.binaryType.right = right;
	return node;
}

static struct treeNode *pushNegationsRec(struct treeNode *root, int negated) {
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *firstPart;
	struct treeNode *secondPart;

	if (root == NULL) {
		return NULL;
	}

	root->next = pushNegationsRec(root->next, 0);

	switch (root->nodeType) {
		case NODE_UNARY_OPERATOR:
			if (root->treeTypes.unaryType.operatorType == UOP_NOT) {
				fprintf(stderr, "OPT1: NOT erkannt, Negationskontext wird invertiert.\n");
				left = root->treeTypes.unaryType.child;
				free(root);
				return pushNegationsRec(left, !negated);
			}
			root->treeTypes.unaryType.child = pushNegationsRec(root->treeTypes.unaryType.child, negated);
			return root;

		case NODE_BINARY_OPERATOR:
			if (negated == 0) {
				root->treeTypes.binaryType.left = pushNegationsRec(root->treeTypes.binaryType.left, 0);
				root->treeTypes.binaryType.right = pushNegationsRec(root->treeTypes.binaryType.right, 0);
				return root;
			}

			switch (root->treeTypes.binaryType.operatorType) {
				case BINOP_AND:
					fprintf(stderr, "OPT1: De Morgan auf AND -> OR angewendet.\n");
					left = pushNegationsRec(root->treeTypes.binaryType.left, 1);
					right = pushNegationsRec(root->treeTypes.binaryType.right, 1);
					free(root);
					return makeBinaryNode(BINOP_OR, left, right);

				case BINOP_OR:
					fprintf(stderr, "OPT1: De Morgan auf OR -> AND angewendet.\n");
					left = pushNegationsRec(root->treeTypes.binaryType.left, 1);
					right = pushNegationsRec(root->treeTypes.binaryType.right, 1);
					free(root);
					return makeBinaryNode(BINOP_AND, left, right);

				case BINOP_IMPLIES:
					fprintf(stderr, "OPT1: Negation vor IMPLIES umgeformt.\n");
					left = pushNegationsRec(root->treeTypes.binaryType.left, 0);
					right = pushNegationsRec(root->treeTypes.binaryType.right, 1);
					free(root);
					return makeBinaryNode(BINOP_AND, left, right);

				case BINOP_IFF:
					fprintf(stderr, "OPT1: Negation vor EQUIV umgeformt.\n");
					firstPart = makeBinaryNode(
						BINOP_AND,
						pushNegationsRec(root->treeTypes.binaryType.left, 0),
						pushNegationsRec(root->treeTypes.binaryType.right, 1));
					secondPart = makeBinaryNode(
						BINOP_AND,
						pushNegationsRec(root->treeTypes.binaryType.left, 1),
						pushNegationsRec(root->treeTypes.binaryType.right, 0));
					free(root);
					return makeBinaryNode(BINOP_OR, firstPart, secondPart);

				default:
					return root;
			}

		case NODE_QUANTOR:
			root->treeTypes.quantorType.formula = pushNegationsRec(root->treeTypes.quantorType.formula, negated);
			if (negated) {
				if (root->treeTypes.quantorType.quantorType == FORALL) {
					fprintf(stderr, "OPT1: De Morgan auf ALL -> EXIST angewendet.\n");
					root->treeTypes.quantorType.quantorType = EXISTS;
				} else {
					fprintf(stderr, "OPT1: De Morgan auf EXIST -> ALL angewendet.\n");
					root->treeTypes.quantorType.quantorType = FORALL;
				}
			}
			return root;

		case NODE_PREDICATE:
			root->treeTypes.predicatType.arguments = pushNegationsRec(root->treeTypes.predicatType.arguments, 0);
			if (negated) {
				fprintf(stderr, "OPT1: Negation bleibt direkt vor dem Prädikat stehen.\n");
				return makeUnaryNot(root);
			}
			return root;

		case NODE_FUNCTION:
			root->treeTypes.functionType.arguments = pushNegationsRec(root->treeTypes.functionType.arguments, 0);
			if (negated) {
				fprintf(stderr, "OPT1: Negation bleibt direkt vor dem Funktionsterm stehen.\n");
				return makeUnaryNot(root);
			}
			return root;

		case NODE_BOOL:
			if (negated) {
				fprintf(stderr, "OPT1: Bool-Wert wird negiert.\n");
				root->treeTypes.boolType.value = !root->treeTypes.boolType.value;
			}
			return root;

		case NODE_VARIABLE:
		case NODE_NUMBER:
		default:
			if (negated) {
				fprintf(stderr, "OPT1: Negation bleibt vor atomarem Term stehen.\n");
				return makeUnaryNot(root);
			}
			return root;
	}
}

struct treeNode *pushNegationsToPredicates(struct treeNode *root) {
	return pushNegationsRec(root, 0);
}


// Aufgabe 3: Implikationen eliminieren
// Transformiert A -> B in NOT A OR B

static struct treeNode *eliminateImplicationsRec(struct treeNode *root) {
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *negatedLeft;

	if (root == NULL) {
		return NULL;
	}

	root->next = eliminateImplicationsRec(root->next);

	switch (root->nodeType) {
		case NODE_QUANTOR:
			root->treeTypes.quantorType.formula = eliminateImplicationsRec(root->treeTypes.quantorType.formula);
			return root;

		case NODE_BINARY_OPERATOR:
			if (root->treeTypes.binaryType.operatorType == BINOP_IMPLIES) {
				fprintf(stderr, "OPT3: Implikation eliminiert: A -> B wird zu (NOT A) OR B.\n");
				
				// Kopiere die linke und rechte Seite
				left = copyTree(root->treeTypes.binaryType.left);
				right = copyTree(root->treeTypes.binaryType.right);
				
				// Rekursiv Implikationen in den Teilen eliminieren
				left = eliminateImplicationsRec(left);
				right = eliminateImplicationsRec(right);
				
				// Erstelle NOT left
				negatedLeft = makeUnaryNot(left);
				
				// Ersetze die Implikation durch OR
				root->treeTypes.binaryType.operatorType = BINOP_OR;
				root->treeTypes.binaryType.left = negatedLeft;
				root->treeTypes.binaryType.right = right;
				
				return root;
			}
			
			root->treeTypes.binaryType.left = eliminateImplicationsRec(root->treeTypes.binaryType.left);
			root->treeTypes.binaryType.right = eliminateImplicationsRec(root->treeTypes.binaryType.right);
			return root;

		case NODE_UNARY_OPERATOR:
			root->treeTypes.unaryType.child = eliminateImplicationsRec(root->treeTypes.unaryType.child);
			return root;

		case NODE_PREDICATE:
			root->treeTypes.predicatType.arguments = eliminateImplicationsRec(root->treeTypes.predicatType.arguments);
			return root;

		case NODE_FUNCTION:
			root->treeTypes.functionType.arguments = eliminateImplicationsRec(root->treeTypes.functionType.arguments);
			return root;

		case NODE_VARIABLE:
		case NODE_NUMBER:
		case NODE_BOOL:
		default:
			return root;
	}
}

struct treeNode *eliminateImplications(struct treeNode *root) {
	return eliminateImplicationsRec(root);
}


// Aufgabe 3: Äquivalenzen eliminieren
// Transformiert A <-> B in (A AND B) OR (NOT A AND NOT B)

static struct treeNode *eliminateEquivalencesRec(struct treeNode *root) {
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *leftCopy;
	struct treeNode *rightCopy;
	struct treeNode *firstPart;
	struct treeNode *secondPart;
	struct treeNode *negatedLeft;
	struct treeNode *negatedRight;

	if (root == NULL) {
		return NULL;
	}

	root->next = eliminateEquivalencesRec(root->next);

	switch (root->nodeType) {
		case NODE_QUANTOR:
			root->treeTypes.quantorType.formula = eliminateEquivalencesRec(root->treeTypes.quantorType.formula);
			return root;

		case NODE_BINARY_OPERATOR:
			if (root->treeTypes.binaryType.operatorType == BINOP_IFF) {
				fprintf(stderr, "OPT3: Äquivalenz eliminiert: A <-> B wird zu (A AND B) OR ((NOT A) AND (NOT B)).\n");
				
				// Kopiere die linke und rechte Seite
				left = copyTree(root->treeTypes.binaryType.left);
				right = copyTree(root->treeTypes.binaryType.right);
				
				// Rekursiv Äquivalenzen in den Teilen eliminieren
				left = eliminateEquivalencesRec(left);
				right = eliminateEquivalencesRec(right);
				
				// Erstelle erste Teilformel: A AND B
				leftCopy = copyTree(left);
				rightCopy = copyTree(right);
				firstPart = makeBinaryNode(BINOP_AND, leftCopy, rightCopy);
				
				// Erstelle zweite Teilformel: (NOT A) AND (NOT B)
				negatedLeft = makeUnaryNot(left);
				negatedRight = makeUnaryNot(right);
				secondPart = makeBinaryNode(BINOP_AND, negatedLeft, negatedRight);
				
				// Ersetze die Äquivalenz durch OR
				root->treeTypes.binaryType.operatorType = BINOP_OR;
				root->treeTypes.binaryType.left = firstPart;
				root->treeTypes.binaryType.right = secondPart;
				
				return root;
			}
			
			root->treeTypes.binaryType.left = eliminateEquivalencesRec(root->treeTypes.binaryType.left);
			root->treeTypes.binaryType.right = eliminateEquivalencesRec(root->treeTypes.binaryType.right);
			return root;

		case NODE_UNARY_OPERATOR:
			root->treeTypes.unaryType.child = eliminateEquivalencesRec(root->treeTypes.unaryType.child);
			return root;

		case NODE_PREDICATE:
			root->treeTypes.predicatType.arguments = eliminateEquivalencesRec(root->treeTypes.predicatType.arguments);
			return root;

		case NODE_FUNCTION:
			root->treeTypes.functionType.arguments = eliminateEquivalencesRec(root->treeTypes.functionType.arguments);
			return root;

		case NODE_VARIABLE:
		case NODE_NUMBER:
		case NODE_BOOL:
		default:
			return root;
	}
}

struct treeNode *eliminateEquivalences(struct treeNode *root) {
	return eliminateEquivalencesRec(root);
}


// Aufgabe 4: Boolesche Konstanten auswerten
// Vereinfacht Operationen mit TRUE und FALSE Konstanten

static struct treeNode *eliminateBooleanConstantsRec(struct treeNode *root) {
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *result;

	if (root == NULL) {
		return NULL;
	}

	root->next = eliminateBooleanConstantsRec(root->next);

	switch (root->nodeType) {
		case NODE_QUANTOR:
			root->treeTypes.quantorType.formula = eliminateBooleanConstantsRec(root->treeTypes.quantorType.formula);
			return root;

		case NODE_BINARY_OPERATOR:
			root->treeTypes.binaryType.left = eliminateBooleanConstantsRec(root->treeTypes.binaryType.left);
			root->treeTypes.binaryType.right = eliminateBooleanConstantsRec(root->treeTypes.binaryType.right);

			left = root->treeTypes.binaryType.left;
			right = root->treeTypes.binaryType.right;

			// Prüfe AND-Operationen
			if (root->treeTypes.binaryType.operatorType == BINOP_AND) {
				// TRUE AND X -> X
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: TRUE AND X vereinfacht zu X.\n");
					result = right;
					free(left);
					free(root);
					return result;
				}
				// X AND TRUE -> X
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: X AND TRUE vereinfacht zu X.\n");
					result = left;
					deleteTree(right);
					free(root);
					return result;
				}
				// FALSE AND X -> FALSE
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: FALSE AND X vereinfacht zu FALSE.\n");
					deleteTree(right);
					free(root);
					return left;
				}
				// X AND FALSE -> FALSE
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: X AND FALSE vereinfacht zu FALSE.\n");
					deleteTree(left);
					free(root);
					return right;
				}
			}

			// Prüfe OR-Operationen
			if (root->treeTypes.binaryType.operatorType == BINOP_OR) {
				// TRUE OR X -> TRUE
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: TRUE OR X vereinfacht zu TRUE.\n");
					deleteTree(right);
					free(root);
					return left;
				}
				// X OR TRUE -> TRUE
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: X OR TRUE vereinfacht zu TRUE.\n");
					deleteTree(left);
					free(root);
					return right;
				}
				// FALSE OR X -> X
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: FALSE OR X vereinfacht zu X.\n");
					result = right;
					free(left);
					free(root);
					return result;
				}
				// X OR FALSE -> X
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: X OR FALSE vereinfacht zu X.\n");
					result = left;
					deleteTree(right);
					free(root);
					return result;
				}
			}

			// Prüfe IMPLIES-Operationen
			if (root->treeTypes.binaryType.operatorType == BINOP_IMPLIES) {
				// FALSE IMPLIES X -> TRUE
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: FALSE IMPLIES X vereinfacht zu TRUE.\n");
					deleteTree(right);
					free(root);
					result = makeNode(NODE_BOOL);
					result->treeTypes.boolType.value = 1;
					free(left);
					return result;
				}
				// X IMPLIES TRUE -> TRUE
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: X IMPLIES TRUE vereinfacht zu TRUE.\n");
					deleteTree(left);
					free(root);
					return right;
				}
				// TRUE IMPLIES X -> X
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: TRUE IMPLIES X vereinfacht zu X.\n");
					result = right;
					free(left);
					free(root);
					return result;
				}
				// X IMPLIES FALSE -> NOT X
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: X IMPLIES FALSE vereinfacht zu NOT X.\n");
					deleteTree(right);
					free(root);
					return makeUnaryNot(left);
				}
			}

			// Prüfe IFF-Operationen
			if (root->treeTypes.binaryType.operatorType == BINOP_IFF) {
				// TRUE IFF X -> X
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: TRUE IFF X vereinfacht zu X.\n");
					result = right;
					free(left);
					free(root);
					return result;
				}
				// X IFF TRUE -> X
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 1) {
					fprintf(stderr, "OPT4: X IFF TRUE vereinfacht zu X.\n");
					result = left;
					deleteTree(right);
					free(root);
					return result;
				}
				// FALSE IFF X -> NOT X
				if (left != NULL && left->nodeType == NODE_BOOL && left->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: FALSE IFF X vereinfacht zu NOT X.\n");
					deleteTree(left);
					free(root);
					return makeUnaryNot(right);
				}
				// X IFF FALSE -> NOT X
				if (right != NULL && right->nodeType == NODE_BOOL && right->treeTypes.boolType.value == 0) {
					fprintf(stderr, "OPT4: X IFF FALSE vereinfacht zu NOT X.\n");
					result = makeUnaryNot(left);
					deleteTree(right);
					free(root);
					return result;
				}
			}

			return root;

		case NODE_UNARY_OPERATOR:
			root->treeTypes.unaryType.child = eliminateBooleanConstantsRec(root->treeTypes.unaryType.child);

			// Prüfe NOT-Operationen
			if (root->treeTypes.unaryType.operatorType == UOP_NOT) {
				if (root->treeTypes.unaryType.child != NULL && 
					root->treeTypes.unaryType.child->nodeType == NODE_BOOL) {
					// NOT TRUE -> FALSE
					if (root->treeTypes.unaryType.child->treeTypes.boolType.value == 1) {
						fprintf(stderr, "OPT4: NOT TRUE vereinfacht zu FALSE.\n");
						result = makeNode(NODE_BOOL);
						result->treeTypes.boolType.value = 0;
						deleteTree(root->treeTypes.unaryType.child);
						free(root);
						return result;
					}
					// NOT FALSE -> TRUE
					if (root->treeTypes.unaryType.child->treeTypes.boolType.value == 0) {
						fprintf(stderr, "OPT4: NOT FALSE vereinfacht zu TRUE.\n");
						result = makeNode(NODE_BOOL);
						result->treeTypes.boolType.value = 1;
						deleteTree(root->treeTypes.unaryType.child);
						free(root);
						return result;
					}
				}
			}
			return root;

		case NODE_PREDICATE:
			root->treeTypes.predicatType.arguments = eliminateBooleanConstantsRec(root->treeTypes.predicatType.arguments);
			return root;

		case NODE_FUNCTION:
			root->treeTypes.functionType.arguments = eliminateBooleanConstantsRec(root->treeTypes.functionType.arguments);
			return root;

		case NODE_VARIABLE:
		case NODE_NUMBER:
		case NODE_BOOL:
		default:
			return root;
	}
}

struct treeNode *eliminateBooleanConstants(struct treeNode *root) {
	return eliminateBooleanConstantsRec(root);
}