%{
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.c"
#include "y.tab.h"
#include <vector>
#include <algorithm>

using namespace std;



extern int linenum;
extern FILE *yyin;
extern char *yytext;
extern char buf[256];
extern vector<int>   kindVector;
extern vector<int>   typeVector;
extern vector<char*> name;
extern vector<int> variableTypeVector; //logical expression eval
extern int externalIntLex;


int yylex();
int yyerror( char *msg );


vector<symbolTable> STV;
vector<instance> parameterList;
vector<int> arrayDim;
vector<int> variableRefType;//variable reference type
int globalLevelCnt = 0;
int loopLevelCnt = 0;
int functionDeclaredType = -1;
int arrayType = -1;
int arrayDeclarationItemCnt = 1;
int arrayInitItemCnt = 0;
int functionParameterAtInvocation = 0;
int arrayDepthCnt = 0;
int LHStype = -1;
int variableOrConstantDeclaredType = -1;
bool isVoidFunc = false;
bool isStatementReturn = false;
bool isArraySameDepth = true;

int const expressionCalStart = -1;
int const expressionCalEnd =   -2;
int const evalError = -3;
int expressionStart = expressionCalEnd;

%}

%token  ID
%token  INT_CONST
%token  FLOAT_CONST
%token  SCIENTIFIC
%token  STR_CONST

%token  LE_OP
%token  NE_OP
%token  GE_OP
%token  EQ_OP
%token  AND_OP
%token  OR_OP

%token  READ
%token  BOOLEAN
%token  WHILE
%token  DO
%token  IF
%token  ELSE
%token  TRUE
%token  FALSE
%token  FOR
%token  INT
%token  PRINT
%token  BOOL
%token  VOID
%token  FLOAT
%token  DOUBLE
%token  STRING
%token  CONTINUE
%token  BREAK
%token  RETURN
%token  CONST

%token  L_PAREN
%token  R_PAREN
%token  COMMA
%token  SEMICOLON
%token  ML_BRACE
%token  MR_BRACE
%token  L_BRACE
%token  R_BRACE
%token  ADD_OP
%token  SUB_OP
%token  MUL_OP
%token  DIV_OP
%token  MOD_OP
%token  ASSIGN_OP
%token  LT_OP
%token  GT_OP
%token  NOT_OP

%token HEY
%token KEV
%token CAN
%token YOU
%token PLEASE
%token WHAT
%token TIME
%token IS
%token IT
%token QUESTIONMARK
%token SHOW
%token NORMALSTRING
%token ANALYZE
%token ME
%token CALCULATE
%token BMI
%token IP
%token MY
%token STOCK

%start program
%%

program : HEY KEV COMMA {printf("r1, s1\n");}command | HEY KEV COMMA polite command;

polite : CAN YOU | CAN YOU PLEASE;

command : timeFunc | stockFunc | bmiFunc | ipFunc;


//time
timeFunc : WHAT TIME IS IT questionmark | TIME;

questionmark : QUESTIONMARK | ;

//stock
stockFunc : show me stock NORMALSTRING stock | ANALYZE stock NORMALSTRING stock;

me : ME | ;
stock : STOCK | ;

//bmi
bmiFunc : CALCULATE my BMI ;

my : MY | ;

//ip
ipFunc : show my IP {system("./ipFunc");} ;
show : SHOW | ;


%%

int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
    fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
    fprintf( stderr, "|\n" );
    fprintf( stderr, "| Unmatched token: %s\n", yytext );
    fprintf( stderr, "|--------------------------------------------------------------------------\n" );
    exit(-1);
    //  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}


