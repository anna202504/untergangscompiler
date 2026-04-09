#**********************************************
#*                                            *
#*     Project: pl1c	                      *
#*     Module:  Makefile                      *
#*                                            *
#*     (c) 2021 by Klaus Wieland              *
#*                                            *
#**********************************************

objects = parser.o scanner.o symboltable.o tree.o #error.o debug.o symboltable.o syntaxtree.o optimize.o generate.o process.o main.o
CC	= gcc
LEX	= flex
YACC	= bison

# Automatisch alle Tree-Dateien aus Input_folder generieren
TREE_FILES = $(patsubst Input_folder/%_in.pl1,Input_folder/%_in.pl1.tree.txt,$(wildcard Input_folder/*_in.pl1))

all: pl1c $(TREE_FILES)

pl1c: $(objects)
	$(CC) -o $@ $^

parser.c:	parser.y
	$(YACC) -d $< -o $@

scanner.c:	scanner.l 
	$(LEX) -t $< > $@

# Pattern Rule für die Generierung von Syntax-Tree-Dateien
Input_folder/%_in.pl1.tree.txt: Input_folder/%_in.pl1 pl1c
	@./pl1c $< > $@ 2>/dev/null


#error.o:	debug.h error.h
#debug.o:	debug.h error.h
#scanner.o:	debug.h error.h parser.h
scanner.o: parser.h
#symboltable.o:	debug.h error.h symboltable.h
#syntaxtree.o:	debug.h error.h syntaxtree.h symboltable.h
#optimize.o:	debug.h error.h optimize.h syntaxtree.h symboltable.h
#generate.o:	debug.h error.h generate.h syntaxtree.h symboltable.h
#parser.o:	debug.h error.h parser.h process.h symboltable.h syntaxtree.h 
parser.o:	parser.h symboltable.h 
#process.o:	debug.h error.h process.h optimize.h generate.h syntaxtree.h symboltable.h
#main.o:		debug.h error.h parser.h generate.h process.h syntaxtree.h symboltable.h
#parser.h:	parser.y


clean:
	rm pl1c parser.c scanner.c parser.h $(objects) $(TREE_FILES)