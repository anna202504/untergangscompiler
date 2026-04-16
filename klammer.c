/*
Usage:
./pl1c Input_folder/complex_in.pl1 > symboltable.txt
  ./klammer symboltable.txt

Dieses Tool liest die Symboltabelle aus der angegebenen Datei und gibt sie erneut (zur Kontrolle) auf stderr aus.
*/
#include <stdio.h>
#include "symboltable.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s symboltable.txt\n", argv[0]);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror(argv[1]);
        return 1;
    }
    struct tableEntry *table = readSymbolTableFromFile(in);
    fclose(in);
    printSymbolTable(stderr, table); // Ausgabe auf stderr zur Kontrolle
    clearSymbolTable(&table);
    return 0;
}