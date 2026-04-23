#include <stdio.h>
#include <stdlib.h>
#include "optimierung.h"

/* ------------------------------------------------------------------ */
/* Internal helpers for NNF transformation                             */
/* ------------------------------------------------------------------ */

static struct treeNode *nnf_recurse(struct treeNode *node);
static struct treeNode *nnf_apply_neg(struct treeNode *node);

/* Creates a NOT node wrapping 'child'. */
static struct treeNode *make_not_node(struct treeNode *child) {
    struct treeNode *n = makeNode(NODE_UNARY_OPERATOR);
    n->treeTypes.unaryType.operatorType = UOP_NOT;
    n->treeTypes.unaryType.child = child;
    return n;
}

/* Creates a binary operator node. */
static struct treeNode *make_binary_node(enum BinaryOperatorType op,
                                         struct treeNode *left,
                                         struct treeNode *right) {
    struct treeNode *n = makeNode(NODE_BINARY_OPERATOR);
    n->treeTypes.binaryType.operatorType = op;
    n->treeTypes.binaryType.left  = left;
    n->treeTypes.binaryType.right = right;
    return n;
}

/* Creates a quantor node with a new formula. */
static struct treeNode *make_quantor_node(enum QuantorType qt,
                                          struct tableEntry *var,
                                          struct treeNode *formula) {
    struct treeNode *n = makeNode(NODE_QUANTOR);
    n->treeTypes.quantorType.quantorType = qt;
    n->treeTypes.quantorType.var         = var;
    n->treeTypes.quantorType.formula     = formula;
    return n;
}

/*
 * nnf_apply_neg: applies one negation to 'node' and pushes it inward.
 * 'node' is consumed (freed or restructured); returns the new subtree.
 */
static struct treeNode *nnf_apply_neg(struct treeNode *node) {
    if (node == NULL) return NULL;

    switch (node->nodeType) {

        case NODE_UNARY_OPERATOR:
            /* ~~A => A  (double negation) */
            if (node->treeTypes.unaryType.operatorType == UOP_NOT) {
                struct treeNode *child = node->treeTypes.unaryType.child;
                node->treeTypes.unaryType.child = NULL;
                free(node);
                fprintf(stderr, "OPT: NNF: ~~A => A\n");
                return nnf_recurse(child);
            }
            break;

        case NODE_BINARY_OPERATOR: {
            enum BinaryOperatorType op = node->treeTypes.binaryType.operatorType;
            struct treeNode *left      = node->treeTypes.binaryType.left;
            struct treeNode *right     = node->treeTypes.binaryType.right;
            node->treeTypes.binaryType.left  = NULL;
            node->treeTypes.binaryType.right = NULL;
            free(node);

            switch (op) {
                case BINOP_AND:
                    /* ~(A & B) => ~A | ~B */
                    fprintf(stderr, "OPT: NNF: De Morgan ~(A & B) => ~A | ~B\n");
                    return make_binary_node(BINOP_OR,
                                           nnf_apply_neg(left),
                                           nnf_apply_neg(right));

                case BINOP_OR:
                    /* ~(A | B) => ~A & ~B */
                    fprintf(stderr, "OPT: NNF: De Morgan ~(A | B) => ~A & ~B\n");
                    return make_binary_node(BINOP_AND,
                                           nnf_apply_neg(left),
                                           nnf_apply_neg(right));

                case BINOP_IMPLIES:
                    /* ~(A -> B) => A & ~B */
                    fprintf(stderr, "OPT: NNF: ~(A -> B) => A & ~B\n");
                    return make_binary_node(BINOP_AND,
                                           nnf_recurse(left),
                                           nnf_apply_neg(right));

                case BINOP_IFF: {
                    /* ~(A <-> B) => (A & ~B) | (~A & B) */
                    fprintf(stderr, "OPT: NNF: ~(A <-> B) => (A & ~B) | (~A & B)\n");
                    struct treeNode *left2  = copyTree(left);
                    struct treeNode *right2 = copyTree(right);
                    return make_binary_node(BINOP_OR,
                               make_binary_node(BINOP_AND,
                                                nnf_recurse(left),
                                                nnf_apply_neg(right)),
                               make_binary_node(BINOP_AND,
                                                nnf_apply_neg(left2),
                                                nnf_recurse(right2)));
                }
            }
            break;
        }

        case NODE_QUANTOR: {
            /* ~ALL[x].A  => EXIST[x].~A
               ~EXIST[x].A => ALL[x].~A  */
            enum QuantorType old_qt    = node->treeTypes.quantorType.quantorType;
            enum QuantorType new_qt    = (old_qt == FORALL) ? EXISTS : FORALL;
            struct tableEntry *var     = node->treeTypes.quantorType.var;
            struct treeNode   *formula = node->treeTypes.quantorType.formula;
            node->treeTypes.quantorType.formula = NULL;
            free(node);
            fprintf(stderr, "OPT: NNF: ~%s => %s (flip quantifier)\n",
                    old_qt == FORALL ? "ALL" : "EXIST",
                    new_qt == FORALL ? "ALL" : "EXIST");
            return make_quantor_node(new_qt, var, nnf_apply_neg(formula));
        }

        case NODE_BOOL:
            /* ~TRUE => FALSE,  ~FALSE => TRUE */
            fprintf(stderr, "OPT: NNF: ~%s => %s\n",
                    node->treeTypes.boolType.value ? "TRUE" : "FALSE",
                    node->treeTypes.boolType.value ? "FALSE" : "TRUE");
            node->treeTypes.boolType.value = !node->treeTypes.boolType.value;
            return node;

        case NODE_PREDICATE:
        case NODE_FUNCTION:
        case NODE_VARIABLE:
        case NODE_NUMBER:
        default:
            /* Base case: negation reaches an atom — wrap in NOT */
            fprintf(stderr, "OPT: NNF: Negation placed before atom.\n");
            return make_not_node(node);
    }

    /* Unreachable for well-formed trees */
    return make_not_node(node);
}

/*
 * nnf_recurse: converts 'node' to NNF without adding a negation.
 * Restructures the tree in-place where possible.
 */
static struct treeNode *nnf_recurse(struct treeNode *node) {
    if (node == NULL) return NULL;

    switch (node->nodeType) {

        case NODE_UNARY_OPERATOR:
            /* Encountered a NOT: delegate the negation to nnf_apply_neg */
            if (node->treeTypes.unaryType.operatorType == UOP_NOT) {
                struct treeNode *child = node->treeTypes.unaryType.child;
                node->treeTypes.unaryType.child = NULL;
                free(node);
                return nnf_apply_neg(child);
            }
            break;

        case NODE_BINARY_OPERATOR:
            node->treeTypes.binaryType.left  =
                nnf_recurse(node->treeTypes.binaryType.left);
            node->treeTypes.binaryType.right =
                nnf_recurse(node->treeTypes.binaryType.right);
            break;

        case NODE_QUANTOR:
            node->treeTypes.quantorType.formula =
                nnf_recurse(node->treeTypes.quantorType.formula);
            break;

        default:
            break; /* leaf node — nothing to do */
    }

    return node;
}

/* ------------------------------------------------------------------ */
/* Public: eliminate implications and equivalences                     */
/* ------------------------------------------------------------------ */

struct treeNode *eliminate_implications_and_equivalences(struct treeNode *node) {
    if (node == NULL) return NULL;

    switch (node->nodeType) {

        case NODE_BINARY_OPERATOR: {
            /* Recurse into children first (bottom-up) */
            node->treeTypes.binaryType.left =
                eliminate_implications_and_equivalences(node->treeTypes.binaryType.left);
            node->treeTypes.binaryType.right =
                eliminate_implications_and_equivalences(node->treeTypes.binaryType.right);

            struct treeNode *left  = node->treeTypes.binaryType.left;
            struct treeNode *right = node->treeTypes.binaryType.right;

            switch (node->treeTypes.binaryType.operatorType) {

                case BINOP_IMPLIES: {
                    /* A -> B  =>  ~A | B */
                    fprintf(stderr, "OPT: IMPL: A -> B => ~A | B\n");
                    node->treeTypes.binaryType.left  = NULL;
                    node->treeTypes.binaryType.right = NULL;
                    free(node);
                    return make_binary_node(BINOP_OR,
                                           make_not_node(left),
                                           right);
                }

                case BINOP_IFF: {
                    /* A <-> B  =>  (~A | B) & (~B | A) */
                    fprintf(stderr, "OPT: IMPL: A <-> B => (~A | B) & (~B | A)\n");
                    struct treeNode *left2  = copyTree(left);
                    struct treeNode *right2 = copyTree(right);
                    node->treeTypes.binaryType.left  = NULL;
                    node->treeTypes.binaryType.right = NULL;
                    free(node);
                    return make_binary_node(BINOP_AND,
                               make_binary_node(BINOP_OR, make_not_node(left),  right),
                               make_binary_node(BINOP_OR, make_not_node(right2), left2));
                }

                default:
                    break; /* AND, OR: nothing to rewrite */
            }
            break;
        }

        case NODE_UNARY_OPERATOR:
            node->treeTypes.unaryType.child =
                eliminate_implications_and_equivalences(node->treeTypes.unaryType.child);
            break;

        case NODE_QUANTOR:
            node->treeTypes.quantorType.formula =
                eliminate_implications_and_equivalences(node->treeTypes.quantorType.formula);
            break;

        default:
            break; /* leaf nodes — nothing to do */
    }

    return node;
}

/* ------------------------------------------------------------------ */
/* Public NNF function                                                 */
/* ------------------------------------------------------------------ */

struct treeNode *push_negations_to_predicates(struct treeNode *node) {
    fprintf(stderr, "OPT: Starting negation push-down (NNF transformation).\n");
    struct treeNode *result = nnf_recurse(node);
    fprintf(stderr, "OPT: Negation push-down complete.\n");
    return result;
}

struct treeNode *eliminate_double_negation(struct treeNode *node) {
    if (node == NULL) {
        return NULL;
    }

    switch (node->nodeType) {
        case NODE_UNARY_OPERATOR:
            if (node->treeTypes.unaryType.operatorType == UOP_NOT) {
                struct treeNode *child = node->treeTypes.unaryType.child;

                /* Double negation: NOT( NOT( A ) ) => A */
                if (child != NULL
                        && child->nodeType == NODE_UNARY_OPERATOR
                        && child->treeTypes.unaryType.operatorType == UOP_NOT) {

                    struct treeNode *grandchild = child->treeTypes.unaryType.child;

                    fprintf(stderr, "OPT: Double negation eliminated.\n");

                    /* Detach grandchild so it is not freed with the NOT nodes */
                    child->treeTypes.unaryType.child = NULL;
                    node->treeTypes.unaryType.child  = NULL;

                    free(child);
                    free(node);

                    /* Recursively optimise the result (handles ~~~~A etc.) */
                    return eliminate_double_negation(grandchild);
                }

                /* Single negation — recurse into child */
                node->treeTypes.unaryType.child =
                    eliminate_double_negation(child);
            }
            break;

        case NODE_BINARY_OPERATOR:
            node->treeTypes.binaryType.left =
                eliminate_double_negation(node->treeTypes.binaryType.left);
            node->treeTypes.binaryType.right =
                eliminate_double_negation(node->treeTypes.binaryType.right);
            break;

        case NODE_QUANTOR:
            node->treeTypes.quantorType.formula =
                eliminate_double_negation(node->treeTypes.quantorType.formula);
            break;

        case NODE_PREDICATE:
        case NODE_FUNCTION:
        case NODE_VARIABLE:
        case NODE_NUMBER:
        case NODE_BOOL:
            /* Leaf nodes or term-level nodes — nothing to optimise */
            break;

        default:
            fprintf(stderr, "OPT: Warning: Unknown node type %d, skipping.\n",
                    node->nodeType);
            break;
    }

    return node;
}