flex lex.l
sleep 2s
yacc -d -v parser.y 
sleep 2s 
g++ main.c lex.yy.c y.tab.c symbolTable.c -ly -ll -lfl
