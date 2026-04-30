# PL/1 Formula Compiler

This project is a small compiler/rewriter for a PL/1-inspired first-order logic input language. It reads declarations and formulas from a `.pl1` file, builds a syntax tree, performs simple logical optimizations, and prints the transformed formula again.

The executable is called `pl1c`.

## What the Compiler Does

For each input block, the compiler:

1. tokenizes the input with Flex (`scanner.l`);
2. parses declarations and formulas with Bison (`parser.y`);
3. stores declared predicates, functions, and variables in a symbol table;
4. builds an abstract syntax tree;
5. validates identifier usage and argument arity;
6. rewrites the formula with logical simplifications;
7. prints declarations and the optimized formula to `stdout`.

Debug information from the scanner, parser, symbol table, syntax tree, and optimizer is printed to `stderr`.

## Project Structure

| Path | Purpose |
| --- | --- |
| `scanner.l` | Flex lexer definition. Recognizes keywords, identifiers, numbers, operators, brackets, comments, and whitespace. |
| `parser.y` | Bison grammar and main program. Parses declarations/formulas, performs semantic checks, calls optimizer passes, and prints output. |
| `symbol_table.c/.h` | Linked-list symbol table for predicates, functions, and variables. |
| `tree.c/.h` | Syntax tree node definitions, creation, copying, deletion, argument counting, and debug tree printing. |
| `optimierung1.c/.h` | Formula optimization passes. |
| `klammer.c/.h` | Pretty-printer for declarations and formulas, including parentheses handling. |
| `Input_folder/` | Example input files. |
| `reference_output_folder/` | Expected/reference outputs for the example inputs. |
| `Makefile` | Build rules for generating and compiling `pl1c`. |

Generated build files such as `parser.c`, `parser.h`, `scanner.c`, `*.o`, and `pl1c` are produced by the build process.

## Requirements

Install these tools before building:

- `gcc`
- `make`
- `flex`
- `bison`

On macOS with Homebrew:

```sh
brew install flex bison
```

On Debian/Ubuntu:

```sh
sudo apt install build-essential flex bison
```

## Build

From the project root:

```sh
make
```

This generates the parser and scanner sources and builds the executable:

```text
./pl1c
```

To remove generated files:

```sh
make clean
```

## Run

Run the compiler with one input file:

```sh
./pl1c Input_folder/complex_in.pl1
```

The optimized PL/1-like formula is printed to `stdout`. To save it to a file:

```sh
./pl1c Input_folder/complex_in.pl1 > out.pl1
```

Because debug logs are printed to `stderr`, they will still appear in the terminal. To save only the optimized output and discard debug logs:

```sh
./pl1c Input_folder/complex_in.pl1 > out.pl1 2>/dev/null
```

To save both output and debug logs separately:

```sh
./pl1c Input_folder/complex_in.pl1 > out.pl1 2> debug.log
```

## Input Language

An input file contains declarations followed by a formula ending with `;`.

Example:

```pl1
DECLARE PREDICATE A : 0
DECLARE PREDICATE B : 0

A() -> B() ;
```

Supported declarations:

```pl1
DECLARE PREDICATE Name : arity
DECLARE FUNCTION Name : arity
DECLARE VARIABLE name : int
```

Supported formula elements:

| Syntax | Meaning |
| --- | --- |
| `TRUE`, `FALSE` | Boolean constants |
| `~A` | Negation |
| `A & B` | Conjunction |
| `A \| B` | Disjunction |
| `A -> B` | Implication |
| `A <-> B` | Equivalence |
| `ALL[x]A` | Universal quantifier |
| `EXIST[x]A` | Existential quantifier |
| `P(x,y)` | Predicate call |
| `f(x)` | Function term |
| `/* comment */` | Block comment |

Identifiers must start with a letter and may contain letters and digits. Variables must currently be declared with type `int`.

## Optimizations

The optimizer currently applies these rewrites:

| Input pattern | Output pattern |
| --- | --- |
| `A -> B` | `~A \| B` |
| `A <-> B` | `(A & B) \| (~A & ~B)` |
| `~~A` | `A` |
| `~(A & B)` | `~A \| ~B` |
| `~(A \| B)` | `~A & ~B` |
| `~ALL[x]A` | `EXIST[x]~A` |
| `~EXIST[x]A` | `ALL[x]~A` |
| `TRUE & A` / `A & TRUE` | `A` |
| `FALSE & A` / `A & FALSE` | `FALSE` |
| `TRUE \| A` / `A \| TRUE` | `TRUE` |
| `FALSE \| A` / `A \| FALSE` | `A` |
| `~TRUE` | `FALSE` |
| `~FALSE` | `TRUE` |

## Example

Command:

```sh
./pl1c Input_folder/simple-imply_in.pl1 > out.pl1 2>/dev/null
```

Input:

```pl1
DECLARE PREDICATE A : 0
DECLARE PREDICATE B : 0

A() -> B() ;
```

Output:

```pl1
DECLARE PREDICATE A : 0
DECLARE PREDICATE B : 0

~ A() | B() ;
```

## Troubleshooting

`Usage: ./pl1c <inputfile>`  
You ran `pl1c` without an input file. Pass a `.pl1` file path.

`fopen: No such file or directory`  
The input path does not exist or was typed incorrectly.

`ERROR: Identifier ... already declared with different type`  
The same name was declared with conflicting information.

`ERROR: ... not declared`  
The formula uses a predicate, function, or variable that was not declared.

`ERROR: Predicate/Function ... expects ... argument(s)`  
The number of arguments in a call does not match the declared arity.
