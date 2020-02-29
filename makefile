stage1exe : adt.o lexer.o hash_table.o parseTable.o parseRules.o parser.o utils.o
	gcc -o stage1exe driver.c adt.o lexer.o hash_table.o parseTable.o parseRules.o parser.o utils.o -lm -fno-stack-protector

adt.o : adt.c adt.h
	gcc -c adt.c
lexer.o : lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
hash_table.o : hash_table.c hash_table.h
	gcc -c hash_table.c
parser.o : parser.c parser.h parserDef.h
	gcc -c parser.c -fno-stack-protector
parseTable.o : parseTable.c parseTable.h
	gcc -c parseTable.c
parseRules.o : parseRules.c parseRules.h
	gcc -c parseRules.c
utils.o : utils.c utils.h
	gcc -c utils.c
clean :
	rm stage1exe adt.o hash_table.o parser.o parseRules.o utils.o lexer.o parseTable.o

