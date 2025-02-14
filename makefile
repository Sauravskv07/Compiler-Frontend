stage1exe : adt.o lexer.o hash_table.o parseTable.o parseRules.o parser.o utils.o ast.o semCheck.o symbol_table.o ir.o codeGen.o
	gcc -o compiler driver.c adt.o ast.o lexer.o hash_table.o parseTable.o parseRules.o parser.o utils.o ir.o semCheck.o codeGen.o symbol_table.o -lm -fno-stack-protector

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
ast.o : ast.c ast.h
	gcc -c ast.c
ir.o : ir.c ir.h
	gcc -c ir.c
codeGen.o: codeGen.c codeGen.h
	gcc -c codeGen.c
semCheck.o : semCheck.c semCheck.h
	gcc -c semCheck.c
symbol_table.o : symbol_table.c symbol_table.h
	gcc -c symbol_table.c
clean :
	rm compiler adt.o hash_table.o parser.o parseRules.o utils.o lexer.o parseTable.o ast.o ir.o semCheck.o symbol_table.o codeGen.o

