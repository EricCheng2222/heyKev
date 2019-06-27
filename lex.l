%{
#include "symbolTable.c"
#include "y.tab.h"
#include <vector>
#include <string>
#define LIST                strcat(buf, yytext)
#define token(t)            {LIST; if (Opt_Token) printf("<%s>\n", #t);}
#define tokenInteger(t, i)  {LIST; if (Opt_Token) printf("<%s:%d>\n", t, i);}
#define tokenString(t, s)   {LIST; if (Opt_Token) printf("<%s:%s>\n", t, s);}
#define MAX_LINE_LENG 256
#define MAX_ID_LENG 256
using namespace std;


int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
/*
int const intType = -100;
int const floatType = -101;
int const doubleType = -102;

int const stringType = -103;
int const boolType = -104;
int const voidType = -105;
*/
int externalIntLex;

int kind, type;
char *nameToParser;
vector<int>   typeVector;
vector<int>   kindVector;
vector<int>   variableTypeVector;
vector<char*> name;

char buf[MAX_LINE_LENG];
char strContent[MAX_LINE_LENG];
typedef struct ids{
	char id_name[256];
	int freq;
	struct ids *next;
} id;
id *id_head;
void set(id **, char *);
id *new_id();
%}

%x PRAGMA
%x SOURCE
%x TOKEN
%x STATISTIC
%x OTHER
%x STR
%x STR_ESCAPE
%x COMMENT




%%

" " {
    printf("%s\n", strContent);
    strcpy(strContent, "");
}

. {
    strcat(strContent, yytext);
}

%%

int main( int argc, char **argv )
{
	if( argc != 2 ) {
	  	fprintf( stderr, "Usage: ./scanner [filename]\n" );
   		exit(0);
 	}
  
  	FILE *fp = fopen( argv[1], "r" );
  	if( fp == NULL ) {
    		fprintf( stderr, "Open file error\n" );
    		exit(-1);
  	}
	
  	yyin = fp;
  	id_head = NULL;
	yylex();
	
  	if(Opt_Statistic){
		id *offset = id_head;
  		while(offset){
    			printf("%s\t%d\n",offset->id_name, offset->freq);
    			offset = offset->next;
  		}
	}

  	exit(0);
}

