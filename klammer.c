/*
Usage:
./pl1c Input_folder/complex_in.pl1 > symboltable.txt
  ./klammer symboltable.txt

Dieses Tool liest die Symboltabelle aus der angegebenen Datei und gibt sie erneut (zur Kontrolle) auf stderr aus.
*/
#include <stdio.h>
#include "symboltable.h"
#include <dirent.h>
#include <string.h>
#include "tree.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--trees") == 0) {
        check_all_trees();
        return 0;
    }
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


// Hilfsfunktion: Liest die nächste relevante Zeile (ohne Symboltabelle und Trennlinien)
static int read_stp_line(FILE *in, char *buf, size_t buflen) {
    while (fgets(buf, buflen, in)) {
        if (strstr(buf, "STP:")) return 1;
        if (strstr(buf, "----- End of Syntax Tree Printout.")) return 0;
    }
    return 0;
}

// Hilfsfunktion: Zählt die führenden Punkte (Einrückung)
static int count_indent(const char *line) {
    const char *p = strstr(line, "STP:");
    if (!p) return 0;
    int count = 0;
    p += 4; // nach "STP:"
    while (*p == '.') { count++; p++; }
    return count;
}

// Hilfsfunktion: Trimmed Pointer auf den eigentlichen Knotentext
static const char *node_text(const char *line) {
    const char *p = strstr(line, "STP:");
    if (!p) return line;
    p += 4;
    while (*p == '.') p++;
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

// Vorwärtsdeklaration
struct node* parse_stp_node(FILE *in, struct tableEntry *table, int depth, char *curline, size_t buflen);


// Hauptfunktion: Liest den Baum ab erster STP-Zeile, ignoriert alles davor
struct node* read_tree_from_file(FILE *in) {
    char line[256];
    // Suche erste STP-Zeile
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, "STP:")) break;
        if (strstr(line, "----- End of Syntax Tree Printout.")) return NULL;
    }
    if (!strstr(line, "STP:")) return NULL;
    // Symboltabelle einlesen (falls vorhanden, aber nicht zwingend)
    fseek(in, 0, SEEK_SET);
    struct tableEntry *table = readSymbolTableFromFile(in);
    // Positioniere Stream auf die gefundene STP-Zeile
    // (erneut suchen, da readSymbolTableFromFile bis zum Ende liest)
    rewind(in);
    long stp_pos = -1;
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, "STP:")) {
            stp_pos = ftell(in) - strlen(line);
            break;
        }
        if (strstr(line, "----- End of Syntax Tree Printout.")) return NULL;
    }
    if (stp_pos < 0) {
        clearSymbolTable(&table);
        return NULL;
    }
    fseek(in, stp_pos, SEEK_SET);
    fgets(line, sizeof(line), in); // hole die STP-Zeile nochmal
    struct node *tree = parse_stp_node(in, table, count_indent(line), line, sizeof(line));
    clearSymbolTable(&table);
    return tree;
}

// Rekursive Hilfsfunktion: Parst einen Knoten und seine Kinder
struct node* parse_stp_node(FILE *in, struct tableEntry *table, int depth, char *curline, size_t buflen) {
    // curline enthält die aktuelle Zeile
    const char *txt = node_text(curline);
    // Einfache Knoten
    if (strncmp(txt, "TRUE", 4) == 0) return create_bool_node(1);
    if (strncmp(txt, "FALSE", 5) == 0) return create_bool_node(0);
    if (strncmp(txt, "Number:", 7) == 0) {
        int val = atoi(txt+8);
        return create_number_node(val);
    }
    if (strncmp(txt, "Variable:", 9) == 0) {
        const char *id = txt+10;
        while (*id == ' ' || *id == '\t') id++;
        char varname[64];
        sscanf(id, "%63s", varname);
        struct tableEntry *entry = getSymbolEntry(table, varname);
        return create_variable_node(entry);
    }
    if (strncmp(txt, "Predicate:", 10) == 0) {
        const char *id = txt+11;
        while (*id == ' ' || *id == '\t') id++;
        char predname[64];
        sscanf(id, "%63s", predname);
        struct tableEntry *entry = getSymbolEntry(table, predname);
        // Argumente?
        // Peek nächste Zeile
        long pos = ftell(in);
        char next[256];
        if (read_stp_line(in, next, buflen) && count_indent(next) > depth) {
            struct node *args = parse_stp_node(in, table, count_indent(next), next, buflen);
            return create_predicate_node(entry, args);
        } else {
            fseek(in, pos, SEEK_SET);
            return create_predicate_node(entry, NULL);
        }
    }
    if (strncmp(txt, "Function:", 9) == 0) {
        const char *id = txt+10;
        while (*id == ' ' || *id == '\t') id++;
        char fname[64];
        sscanf(id, "%63s", fname);
        struct tableEntry *entry = getSymbolEntry(table, fname);
        // Argumente?
        long pos = ftell(in);
        char next[256];
        if (read_stp_line(in, next, buflen) && count_indent(next) > depth) {
            struct node *args = parse_stp_node(in, table, count_indent(next), next, buflen);
            return create_function_node(entry, args);
        } else {
            fseek(in, pos, SEEK_SET);
            return create_function_node(entry, NULL);
        }
    }
    // Operatoren
    if (strncmp(txt, "NOT", 3) == 0) {
        // Ein Kind
        char next[256];
        if (read_stp_line(in, next, buflen) && count_indent(next) > depth) {
            struct node *sub = parse_stp_node(in, table, count_indent(next), next, buflen);
            return create_unary_node(OP_NOT, sub);
        }
        return NULL;
    }
    if (strncmp(txt, "AND", 3) == 0 || strncmp(txt, "OR", 2) == 0 || strncmp(txt, "IMPLICATION", 11) == 0 || strncmp(txt, "EQUIVALENT", 11) == 0) {
        enum binaryOp op = OP_AND;
        if (strncmp(txt, "AND", 3) == 0) op = OP_AND;
        else if (strncmp(txt, "OR", 2) == 0) op = OP_OR;
        else if (strncmp(txt, "IMPLICATION", 11) == 0) op = OP_IMPLIES;
        else if (strncmp(txt, "EQUIVALENT", 11) == 0) op = OP_EQUIV;
        // Zwei Kinder
        char left[256], right[256];
        if (read_stp_line(in, left, buflen) && count_indent(left) > depth) {
            struct node *l = parse_stp_node(in, table, count_indent(left), left, buflen);
            if (read_stp_line(in, right, buflen) && count_indent(right) > depth) {
                struct node *r = parse_stp_node(in, table, count_indent(right), right, buflen);
                return create_binary_node(op, l, r);
            } else {
                // Fehlerfall
                free_tree(l);
            }
        }
        return NULL;
    }
    // Quantoren
    if (strncmp(txt, "ALL", 3) == 0 || strncmp(txt, "EXIST", 5) == 0) {
        enum quantorOp op = (txt[0] == 'A') ? OP_FORALL : OP_EXISTS;
        // Variable
        char vline[256];
        if (read_stp_line(in, vline, buflen) && count_indent(vline) > depth) {
            struct node *var = parse_stp_node(in, table, count_indent(vline), vline, buflen);
            char fline[256];
            if (read_stp_line(in, fline, buflen) && count_indent(fline) > depth) {
                struct node *form = parse_stp_node(in, table, count_indent(fline), fline, buflen);
                // var->types.variableType.entry extrahieren
                if (var->nodeType == NODE_VARIABLE)
                    return create_quantor_node(op, var->types.variableType.entry, form);
                free_tree(var); free_tree(form);
            } else {
                free_tree(var);
            }
        }
        return NULL;
    }
    // Unbekannt
    return NULL;
}

void check_all_trees() {
    DIR *dir = opendir("Input_folder");
    if (!dir) {
        perror("Input_folder");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".tree.txt")) {
            char path[256];
            snprintf(path, sizeof(path), "Input_folder/%s", entry->d_name);
            FILE *f = fopen(path, "r");
            if (!f) continue;
            fprintf(stderr, "\n==== %s ====\n", entry->d_name);
            struct node *tree = read_tree_from_file(f);
            fclose(f);
            if (tree) {
                print_tree(stderr, tree, 0);
                char *formel = tree_to_pl1_formula(tree);
                if (formel) {
                    fprintf(stderr, "PL1-Formel: %s\n", formel);
                    free(formel);
                }
                free_tree(tree);
            } else {
                fprintf(stderr, "Fehler beim Parsen von %s\n", entry->d_name);
            }
        }
    }
    closedir(dir);
}