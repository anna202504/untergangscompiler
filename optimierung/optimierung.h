#ifndef OPTIMIERUNG_H
#define OPTIMIERUNG_H

#include "../tree.h"

/*
 * Eliminates implications and equivalences by rewriting them with AND/OR/NOT:
 *   A -> B      =>  ~A | B
 *   A <-> B     =>  (~A | B) & (~B | A)
 * Recurses into all children. Returns the new root of the modified tree.
 */
struct treeNode *eliminate_implications_and_equivalences(struct treeNode *node);

/*
 * Eliminates double negations (~~A => A) recursively in the syntax tree.
 * Freed nodes are the two NOT wrappers; the inner formula is preserved.
 * Returns the new root of the (potentially modified) tree.
 */
struct treeNode *eliminate_double_negation(struct treeNode *node);

/*
 * Transforms the syntax tree into Negation Normal Form (NNF) by pushing
 * all negations inward until each sits directly before a predicate/atom.
 * Applies the following rewrite rules:
 *   ~~A           => A
 *   ~(A & B)      => ~A | ~B          (De Morgan)
 *   ~(A | B)      => ~A & ~B          (De Morgan)
 *   ~(A -> B)     => A & ~B
 *   ~(A <-> B)    => (A & ~B) | (~A & B)
 *   ~ALL[x].A     => EXIST[x].~A
 *   ~EXIST[x].A   => ALL[x].~A
 *   ~TRUE         => FALSE
 *   ~FALSE        => TRUE
 * Returns the new root of the transformed tree.
 */
struct treeNode *push_negations_to_predicates(struct treeNode *node);

/*
 * Evaluates boolean constants by simplifying operations that involve
 * TRUE or FALSE. Recurses into all children (bottom-up).
 * Rules applied:
 *   NOT TRUE       => FALSE          NOT FALSE      => TRUE
 *   FALSE & A      => FALSE          TRUE  & A      => A
 *   A     & FALSE  => FALSE          A     & TRUE   => A
 *   TRUE  | A      => TRUE           FALSE | A      => A
 *   A     | TRUE   => TRUE           A     | FALSE  => A
 *   FALSE -> A     => TRUE           TRUE  -> A     => A
 *   A     -> TRUE  => TRUE           A     -> FALSE => ~A
 *   TRUE  <-> A    => A              FALSE <-> A    => ~A
 *   A     <-> TRUE => A              A     <-> FALSE => ~A
 * Returns the new root of the simplified tree.
 */
struct treeNode *evaluate_boolean_constants(struct treeNode *node);

#endif /* OPTIMIERUNG_H */