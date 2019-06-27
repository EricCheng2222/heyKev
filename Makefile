all: parser

parser: main.c lex.yy.c y.tab.c symbolTable.c
	g++ -o parser main.c lex.yy.c y.tab.c symbolTable.c -ly -ll -lfl

y.tab.c: parser.y symbolTable.c
	yacc -d -v parser.y 

lex.yy.c: lex.l 
	flex lex.l



