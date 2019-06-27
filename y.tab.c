/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y" /* yacc.c:337  */

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

bool isArithOperand(int token){
    return ( token==ADD_OP || token==SUB_OP || token==MUL_OP || token==DIV_OP || token==MOD_OP);
}

bool isRelationalOpernd(int token){
    return (token==LE_OP || token==NE_OP || token==GE_OP || token==EQ_OP || token==LT_OP || token==GT_OP);
}

bool isNumber(int opernd){
    if(opernd==intType || opernd==floatType || opernd==doubleType) return true;
    return false;
}

int coercible(int LHS, int OPerator, int RHS){
    if(OPerator == -1){
        if(LHS==doubleType && (RHS==doubleType || RHS==floatType || RHS==intType)) return doubleType;
        else if(LHS==floatType && (RHS==floatType||RHS==intType)) return floatType;
        else if(LHS==intType && RHS==intType) return intType;
        return -1;
    }
    else if( isArithOperand(OPerator)==true ){
        if(LHS==doubleType && (RHS==doubleType || RHS==floatType || RHS==intType)) return doubleType;
        else if(LHS==floatType && (RHS==floatType||RHS==intType)) return floatType;
        else if(LHS==intType && RHS==intType) return intType;
    }
    else if( isRelationalOpernd(OPerator)==true ){
        if( isNumber(LHS) && isNumber(RHS) ){
            return boolType;
        }
        else return -1;
    }
    
    return -1;
}

bool largerPrecedence(int operator1, int operator2){
    if(operator1==NOT_OP) return true;
    else if(operator1==MUL_OP || operator1==DIV_OP || operator1==MOD_OP){
        if(operator2==NOT_OP)return false;
        return true;
    }
    else if(operator1==ADD_OP || operator1==SUB_OP){
        if(operator2==MUL_OP || operator2==DIV_OP || operator2==MOD_OP || operator2==NOT_OP) return false;
        return true;
    }
    else if(operator1==LT_OP || operator1==LE_OP || operator1==GT_OP || operator1==GE_OP){
        if(operator2==ADD_OP || operator2==SUB_OP || operator2==MUL_OP || operator2==DIV_OP || operator2==MOD_OP || operator2==NOT_OP) return false;
        return true;
    }
    else if(operator1==EQ_OP){
        if(operator2==EQ_OP || operator2==AND_OP || operator2==OR_OP) return true;
        return false;
    }
    else if(operator1==AND_OP){
        if(operator2==AND_OP || operator2==OR_OP) return true;
        return false;
    }
    else if(operator1==OR_OP){
        if(operator2==OR_OP) return true;
        return false;
    }
    return false;
}

bool isNumberOrBool(int opernd){
    if(isNumber(opernd) || opernd==boolType) return true;
    return false;
}

bool isScalar(int refBack){
    if(refBack==voidType || refBack==stringType) return false;
    return true;
}

void addParameter(){
    for(int i=0; i<parameterList.size(); i++){
        STV.back().addInstance(parameterList[i]);
    }
}

bool isOperator(int token){
    return (token==LE_OP || token==NE_OP || token==GE_OP || token==EQ_OP || token==AND_OP || token==OR_OP || token==L_PAREN || token==R_PAREN || token==ADD_OP || token==SUB_OP || token==MUL_OP || token==DIV_OP || token==MOD_OP || token==LT_OP || token==GT_OP || token==NOT_OP);
}



int evalVariableTypeVector(){
    //printf(" enter evaluation process\n ");
    //for(int i=0; i<variableTypeVector.size(); i++){
    //    printf("%d ", variableTypeVector[i]);
    //}
    //printf("\n\n");
    
    if(isArraySameDepth == false){
        printf("##########Error at Line #%d:", linenum);
        printf("logical expression value can only be scalar Type");
        printf(" ##########\n");
        isArraySameDepth = true;
    }
    
    
    vector<int> opertr;
    vector<int> opernd;
    std::reverse(variableTypeVector.begin(), variableTypeVector.end());
    
    while(variableTypeVector.empty()==false){
        if( isOperator(variableTypeVector.back()) == false ){//which is an operand
            //printf("1\n");
            opernd.push_back(variableTypeVector.back());
            variableTypeVector.pop_back();
        }
        
        if( variableTypeVector.back()==L_PAREN ){
            //printf("2\n");
            opertr.push_back(variableTypeVector.back());
            variableTypeVector.pop_back();
            while(variableTypeVector.back()!=R_PAREN){
                //printf("3\n");
                if( isOperator(variableTypeVector.back()) == false ){//which is an operand
                    opernd.push_back(variableTypeVector.back());
                    variableTypeVector.pop_back();
                }
                else{
                    opertr.push_back(variableTypeVector.back());
                    variableTypeVector.pop_back();
                }
            }
        }
        else if(opertr.size()==0 && isOperator(variableTypeVector.back())==true){
            //printf("4\n");
            opertr.push_back(variableTypeVector.back());
            variableTypeVector.pop_back();
            continue;
        }
        else if( isOperator(variableTypeVector.back()) == true && variableTypeVector.back()!=R_PAREN){
            //printf("5\n");
            while( largerPrecedence(opertr.back(), variableTypeVector.back()) ){
                int tmpOperator = opertr.back();
                opertr.pop_back();
                
                if( tmpOperator!=NOT_OP ){
                    int operand1 = opernd.back(); opernd.pop_back();
                    int operand2 = opernd.back(); opernd.pop_back();
                    int result = coercible(operand1, tmpOperator, operand2);
                    opernd.push_back(result);
                    if(result==-1){
                        printf("##########Error at Line #%d:", linenum);
                        printf("arithmetic operation issue");
                        printf(" ##########\n");
                    }
                    
                }
            }
            opertr.push_back(variableTypeVector.back());
            variableTypeVector.pop_back();
        }
        else if(variableTypeVector.back()==R_PAREN){
            //printf("6\n");
            while(opertr.back()!=L_PAREN){
                int tmpOperator = opertr.back();
                opertr.pop_back();
                
                if( tmpOperator!=NOT_OP ){
                    int operand1 = opernd.back(); opernd.pop_back();
                    int operand2 = opernd.back(); opernd.pop_back();
                    int result = coercible(operand1, tmpOperator, operand2);
                    opernd.push_back(result);
                    if(result==-1){
                        printf("##########Error at Line #%d:", linenum);
                        printf("arithmetic operation issue");
                        printf(" ##########\n");
                    }
                }
            }
            variableTypeVector.pop_back();
            opertr.pop_back();
        }
    }
    
    while( opertr.empty()==false ){
        int tmpOperator = opertr.back();
        opertr.pop_back();
        
        if( tmpOperator!=NOT_OP ){
            int operand1 = opernd.back(); opernd.pop_back();
            int operand2 = opernd.back(); opernd.pop_back();
            int result = coercible(operand1, tmpOperator, operand2);
            opernd.push_back(result);
            if(result==-1){
                printf("##########Error at Line #%d:", linenum);
                printf("arithmetic operation issue");
                printf(" ##########\n");
            }
        }
    }
    variableTypeVector.clear();
    variableTypeVector.push_back(opernd.back());
    return 0;
    
}

#line 321 "y.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    INT_CONST = 259,
    FLOAT_CONST = 260,
    SCIENTIFIC = 261,
    STR_CONST = 262,
    LE_OP = 263,
    NE_OP = 264,
    GE_OP = 265,
    EQ_OP = 266,
    AND_OP = 267,
    OR_OP = 268,
    READ = 269,
    BOOLEAN = 270,
    WHILE = 271,
    DO = 272,
    IF = 273,
    ELSE = 274,
    TRUE = 275,
    FALSE = 276,
    FOR = 277,
    INT = 278,
    PRINT = 279,
    BOOL = 280,
    VOID = 281,
    FLOAT = 282,
    DOUBLE = 283,
    STRING = 284,
    CONTINUE = 285,
    BREAK = 286,
    RETURN = 287,
    CONST = 288,
    L_PAREN = 289,
    R_PAREN = 290,
    COMMA = 291,
    SEMICOLON = 292,
    ML_BRACE = 293,
    MR_BRACE = 294,
    L_BRACE = 295,
    R_BRACE = 296,
    ADD_OP = 297,
    SUB_OP = 298,
    MUL_OP = 299,
    DIV_OP = 300,
    MOD_OP = 301,
    ASSIGN_OP = 302,
    LT_OP = 303,
    GT_OP = 304,
    NOT_OP = 305
  };
#endif
/* Tokens.  */
#define ID 258
#define INT_CONST 259
#define FLOAT_CONST 260
#define SCIENTIFIC 261
#define STR_CONST 262
#define LE_OP 263
#define NE_OP 264
#define GE_OP 265
#define EQ_OP 266
#define AND_OP 267
#define OR_OP 268
#define READ 269
#define BOOLEAN 270
#define WHILE 271
#define DO 272
#define IF 273
#define ELSE 274
#define TRUE 275
#define FALSE 276
#define FOR 277
#define INT 278
#define PRINT 279
#define BOOL 280
#define VOID 281
#define FLOAT 282
#define DOUBLE 283
#define STRING 284
#define CONTINUE 285
#define BREAK 286
#define RETURN 287
#define CONST 288
#define L_PAREN 289
#define R_PAREN 290
#define COMMA 291
#define SEMICOLON 292
#define ML_BRACE 293
#define MR_BRACE 294
#define L_BRACE 295
#define R_BRACE 296
#define ADD_OP 297
#define SUB_OP 298
#define MUL_OP 299
#define DIV_OP 300
#define MOD_OP 301
#define ASSIGN_OP 302
#define LT_OP 303
#define GT_OP 304
#define NOT_OP 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   420

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  192
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  318

#define YYUNDEFTOK  2
#define YYMAXUTOK   305

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   307,   307,   309,   307,   314,   315,   316,   317,   321,
     322,   323,   324,   325,   329,   328,   346,   345,   364,   366,
     367,   363,   379,   381,   382,   378,   396,   406,   417,   427,
     440,   446,   453,   460,   468,   471,   476,   488,   499,   506,
     517,   523,   530,   537,   537,   540,   549,   558,   561,   561,
     563,   575,   589,   589,   592,   602,   615,   614,   624,   625,
     626,   627,   631,   632,   633,   634,   635,   636,   637,   640,
     644,   640,   664,   664,   679,   679,   679,   698,   697,   709,
     709,   727,   738,   744,   746,   727,   751,   752,   754,   755,
     763,   750,   772,   771,   788,   789,   792,   793,   792,   808,
     809,   810,   811,   810,   826,   827,   830,   835,   830,   849,
     850,   852,   853,   852,   869,   870,   873,   874,   873,   887,
     888,   889,   890,   889,   907,   908,   906,   924,   924,   941,
     965,   968,   976,   987,  1010,  1042,  1044,  1044,  1049,  1056,
    1056,  1057,  1060,  1060,  1061,  1064,  1065,  1068,  1069,  1070,
    1071,  1072,  1073,  1076,  1076,  1081,  1081,  1086,  1089,  1089,
    1090,  1090,  1091,  1091,  1092,  1095,  1096,  1099,  1099,  1104,
    1105,  1105,  1114,  1118,  1126,  1128,  1131,  1131,  1134,  1145,
    1159,  1160,  1161,  1162,  1163,  1166,  1167,  1170,  1171,  1172,
    1173,  1174,  1175
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INT_CONST", "FLOAT_CONST",
  "SCIENTIFIC", "STR_CONST", "LE_OP", "NE_OP", "GE_OP", "EQ_OP", "AND_OP",
  "OR_OP", "READ", "BOOLEAN", "WHILE", "DO", "IF", "ELSE", "TRUE", "FALSE",
  "FOR", "INT", "PRINT", "BOOL", "VOID", "FLOAT", "DOUBLE", "STRING",
  "CONTINUE", "BREAK", "RETURN", "CONST", "L_PAREN", "R_PAREN", "COMMA",
  "SEMICOLON", "ML_BRACE", "MR_BRACE", "L_BRACE", "R_BRACE", "ADD_OP",
  "SUB_OP", "MUL_OP", "DIV_OP", "MOD_OP", "ASSIGN_OP", "LT_OP", "GT_OP",
  "NOT_OP", "$accept", "program", "$@1", "$@2", "decl_list",
  "decl_and_def_list", "funct_def", "$@3", "$@4", "$@5", "$@6", "$@7",
  "$@8", "$@9", "$@10", "funct_decl", "parameter_list", "var_decl",
  "identifier_list", "initial_array", "$@11", "literal_list", "const_decl",
  "$@12", "const_list", "array_decl", "$@13", "dim", "compound_statement",
  "$@14", "var_const_stmt_list", "statement", "simple_statement", "$@15",
  "$@16", "$@17", "$@18", "$@19", "conditional_statement", "$@20", "$@21",
  "while_statement", "$@22", "$@23", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "$@30", "for_statement", "$@31",
  "initial_expression_list", "initial_expression", "$@32", "$@33", "$@34",
  "$@35", "control_expression_list", "control_expression", "$@36", "$@37",
  "$@38", "$@39", "increment_expression_list", "increment_expression",
  "$@40", "$@41", "$@42", "$@43", "function_invoke_statement", "$@44",
  "$@45", "$@46", "jump_statement", "jump_loop_statement",
  "variable_reference", "Logical_expression", "logical_expression", "$@47",
  "logical_term", "$@48", "logical_factor", "$@49", "relation_expression",
  "relation_operator", "arithmetic_expression", "$@50", "$@51", "term",
  "$@52", "$@53", "$@54", "factor", "element", "$@55", "$@56",
  "logical_expression_list", "array_list", "$@57", "dimension",
  "scalar_type", "sign_literal_const", "literal_const", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
# endif

#define YYPACT_NINF -220

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-220)))

#define YYTABLE_NINF -177

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -220,    22,  -220,  -220,   198,  -220,  -220,    25,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,    27,    20,    46,  -220,
      19,     7,   -32,    80,    62,   198,   385,   162,    45,    82,
    -220,  -220,    55,    63,   103,    47,    83,  -220,  -220,  -220,
    -220,    15,    91,    38,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,    57,  -220,  -220,  -220,    89,    92,  -220,    10,    88,
      66,  -220,  -220,  -220,  -220,  -220,   117,    94,   -24,   100,
    -220,   106,  -220,   108,   114,    46,   116,  -220,    57,   149,
    -220,  -220,  -220,  -220,    50,   136,   120,   162,    57,     6,
    -220,   162,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
     194,  -220,  -220,  -220,  -220,  -220,   121,   151,   162,  -220,
     162,  -220,  -220,   108,   156,  -220,   125,  -220,  -220,  -220,
    -220,  -220,    89,    99,   162,   123,    12,  -220,  -220,  -220,
     162,   162,    88,   194,   194,   194,   194,   194,  -220,   134,
    -220,  -220,   -12,  -220,  -220,  -220,   116,  -220,    57,   216,
    -220,  -220,   162,   137,   162,  -220,    92,  -220,    66,    66,
    -220,  -220,  -220,  -220,   162,  -220,  -220,  -220,  -220,    48,
     161,   141,   144,   147,   153,   162,   148,   152,   162,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,   185,    89,  -220,   154,  -220,  -220,   155,   164,
    -220,   162,  -220,   162,   162,   166,  -220,  -220,   167,   145,
       4,  -220,   170,   162,   171,  -220,  -220,   174,   173,   175,
     169,  -220,  -220,  -220,  -220,  -220,   101,  -220,   178,  -220,
    -220,   182,   162,   162,   188,  -220,   162,  -220,   181,  -220,
     180,   371,   189,   196,   205,   186,   204,  -220,   206,  -220,
    -220,   215,  -220,  -220,   150,  -220,  -220,   162,   162,   207,
     208,   162,  -220,  -220,  -220,   242,   247,   278,   224,   226,
     213,  -220,   219,  -220,  -220,  -220,  -220,   371,   233,  -220,
     249,  -220,   162,   235,   236,   162,   162,   232,   162,   244,
     245,   239,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,   243,   162,   162,  -220,   254,   309,   340,  -220,  -220,
    -220,   256,  -220,  -220,   162,  -220,  -220,  -220
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     8,     1,     0,   180,   183,     0,   184,   181,
     182,    48,     3,     7,     5,     6,     0,     0,     0,    13,
      42,     0,    40,     0,     0,     4,     0,     0,     0,     0,
      34,    43,    18,     0,     0,     0,     0,    12,    11,     9,
      10,     0,     0,   134,   187,   188,   189,   190,   191,   192,
     170,   167,   142,   169,    41,   135,   138,   141,   144,   146,
     157,   164,   166,   133,   165,   186,     0,    53,    35,    38,
      39,     0,    28,     0,    22,     0,    33,    32,     0,     0,
      49,    26,    56,    14,     0,     0,     0,     0,     0,     0,
     185,     0,   136,   139,   148,   152,   150,   149,   147,   151,
       0,   153,   155,   158,   160,   162,     0,     0,     0,    43,
      47,    19,    29,     0,     0,    51,     0,    61,    15,    27,
      16,   173,   175,     0,     0,   177,     0,   167,   168,   143,
       0,     0,   145,     0,     0,     0,     0,     0,    55,     0,
      36,    37,     0,    46,    20,    23,    30,    31,     0,     0,
      17,   172,     0,     0,     0,   171,   137,   140,   154,   156,
     159,   161,   163,    54,     0,    44,    21,    24,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      59,    60,    62,    58,    63,    64,    65,    66,    67,    68,
     130,    69,     0,   174,   179,     0,    45,    25,   124,   134,
      74,     0,    86,     0,    95,     0,   131,   132,     0,     0,
      42,   178,     0,     0,     0,    81,    87,     0,     0,    94,
     169,   100,    72,   129,    70,   127,     0,    75,     0,    61,
      77,     0,   105,     0,     0,    73,     0,   128,     0,    76,
       0,    88,     0,     0,     0,   104,   169,   110,   169,    99,
     102,     0,   125,    82,     0,    61,    61,   115,     0,     0,
       0,     0,    71,   126,    61,     0,     0,     0,     0,   114,
     169,   120,   169,   109,   112,    97,   103,    83,     0,    78,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,   119,   122,   107,   113,    98,    84,    89,    61,
      61,     0,     0,     0,    85,     0,     0,     0,   117,   123,
     108,     0,    80,    93,     0,    90,   118,    91
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -220,  -220,  -220,  -220,  -220,  -220,   266,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,   272,   273,     8,  -220,   195,
    -220,  -220,    13,  -220,  -220,   -23,  -220,  -220,   -34,  -220,
    -219,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -144,  -108,   -84,
    -220,   168,  -220,   -83,  -220,  -220,  -220,   214,  -220,  -220,
      11,  -220,  -220,  -220,   -19,   227,  -220,  -220,   102,  -220,
    -220,  -220,     9,   -47,  -220
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    19,     4,    25,    12,   118,   150,    73,
     144,   166,   113,   167,   197,    13,    33,   180,    21,    70,
      71,   142,   181,    18,    36,    22,    28,    67,   182,   117,
     149,   183,   184,   209,   236,   235,   214,   239,   185,   242,
     231,   186,   228,   264,   287,   304,   216,   229,   254,   305,
     317,   187,   290,   218,   219,   260,   286,   234,   261,   244,
     245,   284,   303,   259,   285,   268,   269,   301,   314,   283,
     302,   188,   213,   263,   237,   189,   190,    53,    54,    55,
     130,    56,   131,    57,    91,    58,   100,    59,   133,   134,
      60,   135,   136,   137,    61,    62,    89,    87,   123,    63,
      86,   125,   192,    64,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     140,   122,   143,   126,    90,   191,    69,    83,   129,    43,
     241,    77,    14,    16,   -52,    31,   153,    15,    94,    95,
      96,    97,     3,   108,   164,    92,   200,    24,    17,   165,
      20,   115,    34,    39,    16,    34,   266,   267,    40,   111,
      50,    90,   -52,    29,    30,   277,   195,   155,   157,   127,
     120,    27,    81,    26,    23,    82,   196,   -52,    98,    99,
     220,    44,    45,    46,    47,    35,    27,   205,   193,     5,
     208,     6,    85,     8,     9,    10,  -176,    48,    49,   145,
     306,   307,   198,    66,   114,    68,  -176,   119,   246,   248,
      82,   147,    72,   215,    78,   217,   221,   191,    74,    75,
      88,   168,    92,     5,    93,     6,    76,     8,     9,    10,
     103,   104,   105,   270,   272,    32,   160,   161,   162,    79,
      80,   106,   191,   191,   247,   249,    84,    75,   251,   122,
     101,   102,   107,   191,   151,   152,   238,   152,   291,    43,
      44,    45,    46,    47,   158,   159,   110,   109,    82,   271,
     273,   112,   116,   276,   -52,   139,    48,    49,   124,   146,
     138,   154,   191,   191,   199,    43,    44,    45,    46,    47,
      50,   121,   148,   163,   292,   201,   194,   295,   296,    51,
     298,   203,    48,    49,   202,   206,    52,   204,   210,   207,
     212,   265,   224,   211,   309,   310,    50,    43,    44,    45,
      46,    47,  -176,   222,   223,    51,   316,   225,   227,   230,
     232,   233,    52,   240,    48,    49,  -101,   243,   252,   169,
     253,     5,   258,     6,     7,     8,     9,    10,    50,   255,
     170,    11,   171,   172,   173,   250,   256,    51,   174,     5,
     175,     6,   257,     8,     9,    10,   176,   177,   178,    11,
     169,  -111,   262,   -96,   274,   275,    82,   179,   278,   281,
    -121,   170,   282,   171,   172,   173,  -106,   288,   289,   174,
       5,   175,     6,   297,     8,     9,    10,   176,   177,   178,
      11,   169,   293,   294,   299,   300,  -116,    82,   279,   311,
     308,    37,   170,   315,   171,   172,   173,    38,   156,    42,
     174,     5,   175,     6,   141,     8,     9,    10,   176,   177,
     178,    11,   169,     0,   132,   226,   128,     0,    82,   280,
       0,     0,     0,   170,     0,   171,   172,   173,     0,     0,
       0,   174,     5,   175,     6,     0,     8,     9,    10,   176,
     177,   178,    11,   169,     0,     0,     0,     0,     0,    82,
     312,     0,     0,     0,   170,     0,   171,   172,   173,     0,
       0,     0,   174,     5,   175,     6,     0,     8,     9,    10,
     176,   177,   178,    11,   169,     0,     0,     0,     0,     0,
      82,   313,     0,     0,     0,   170,     0,   171,   172,   173,
       0,     0,     0,   174,     5,   175,     6,     0,     8,     9,
      10,   176,   177,   178,    11,     0,     0,     0,     5,     0,
       6,    82,     8,     9,    10,     0,     0,     0,     0,     0,
      41
};

static const yytype_int16 yycheck[] =
{
     108,    85,   110,    87,    51,   149,    29,    41,    91,     3,
     229,    34,     4,     4,    38,    47,   124,     4,     8,     9,
      10,    11,     0,    47,    36,    13,   170,    18,     3,    41,
       3,    78,    23,    25,    25,    26,   255,   256,    25,    73,
      34,    88,    38,    36,    37,   264,   154,    35,   131,    43,
      84,    47,    37,    34,    34,    40,   164,    38,    48,    49,
     204,     4,     5,     6,     7,     3,    47,   175,   152,    23,
     178,    25,    34,    27,    28,    29,    38,    20,    21,   113,
     299,   300,    34,    38,    75,     3,    38,    37,   232,   233,
      40,   114,    37,   201,    47,   203,   204,   241,    35,    36,
      43,   148,    13,    23,    12,    25,     3,    27,    28,    29,
      44,    45,    46,   257,   258,    35,   135,   136,   137,    36,
      37,     4,   266,   267,   232,   233,    35,    36,   236,   213,
      42,    43,    38,   277,    35,    36,    35,    36,   282,     3,
       4,     5,     6,     7,   133,   134,    40,    47,    40,   257,
     258,    37,     3,   261,    38,     4,    20,    21,    38,     3,
      39,    38,   306,   307,     3,     3,     4,     5,     6,     7,
      34,    35,    47,    39,   282,    34,    39,   285,   286,    43,
     288,    34,    20,    21,    40,    37,    50,    34,     3,    37,
      35,    41,    47,    39,   302,   303,    34,     3,     4,     5,
       6,     7,    38,    37,    37,    43,   314,    37,    37,    35,
      37,    36,    50,    35,    20,    21,    47,    35,    37,     3,
      40,    23,    36,    25,    26,    27,    28,    29,    34,    40,
      14,    33,    16,    17,    18,    47,    40,    43,    22,    23,
      24,    25,    37,    27,    28,    29,    30,    31,    32,    33,
       3,    47,    37,    47,    47,    47,    40,    41,    16,    35,
      47,    14,    36,    16,    17,    18,    47,    34,    19,    22,
      23,    24,    25,    41,    27,    28,    29,    30,    31,    32,
      33,     3,    47,    47,    40,    40,    47,    40,    41,    35,
      47,    25,    14,    37,    16,    17,    18,    25,   130,    26,
      22,    23,    24,    25,   109,    27,    28,    29,    30,    31,
      32,    33,     3,    -1,   100,   213,    89,    -1,    40,    41,
      -1,    -1,    -1,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,     3,    -1,    -1,    -1,    -1,    -1,    40,
      41,    -1,    -1,    -1,    14,    -1,    16,    17,    18,    -1,
      -1,    -1,    22,    23,    24,    25,    -1,    27,    28,    29,
      30,    31,    32,    33,     3,    -1,    -1,    -1,    -1,    -1,
      40,    41,    -1,    -1,    -1,    14,    -1,    16,    17,    18,
      -1,    -1,    -1,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    23,    -1,
      25,    40,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,
      35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,    53,     0,    55,    23,    25,    26,    27,    28,
      29,    33,    57,    66,    68,    73,   153,     3,    74,    54,
       3,    69,    76,    34,   153,    56,    34,    47,    77,    36,
      37,    47,    35,    67,   153,     3,    75,    57,    66,    68,
      73,    35,    67,     3,     4,     5,     6,     7,    20,    21,
      34,    43,    50,   128,   129,   130,   132,   134,   136,   138,
     141,   145,   146,   150,   154,   155,    38,    78,     3,    76,
      70,    71,    37,    60,    35,    36,     3,    76,    47,    36,
      37,    37,    40,    79,    35,    34,   151,   148,    43,   147,
     154,   135,    13,    12,     8,     9,    10,    11,    48,    49,
     137,    42,    43,    44,    45,    46,     4,    38,    47,    47,
      40,    79,    37,    63,   153,   154,     3,    80,    58,    37,
      79,    35,   130,   149,    38,   152,   130,    43,   146,   134,
     131,   133,   138,   139,   140,   142,   143,   144,    39,     4,
     129,    70,    72,   129,    61,    79,     3,    76,    47,    81,
      59,    35,    36,   129,    38,    35,   132,   134,   141,   141,
     145,   145,   145,    39,    36,    41,    62,    64,   154,     3,
      14,    16,    17,    18,    22,    24,    30,    31,    32,    41,
      68,    73,    79,    82,    83,    89,    92,   102,   122,   126,
     127,   128,   153,   130,    39,   129,   129,    65,    34,     3,
     128,    34,    40,    34,    34,   129,    37,    37,   129,    84,
       3,    39,    35,   123,    87,   129,    97,   129,   104,   105,
     128,   129,    37,    37,    47,    37,   149,    37,    93,    98,
      35,    91,    37,    36,   108,    86,    85,   125,    35,    88,
      35,    81,    90,    35,   110,   111,   128,   129,   128,   129,
      47,   129,    37,    40,    99,    40,    40,    37,    36,   114,
     106,   109,    37,   124,    94,    41,    81,    81,   116,   117,
     128,   129,   128,   129,    47,    47,   129,    81,    16,    41,
      41,    35,    36,   120,   112,   115,   107,    95,    34,    19,
     103,   128,   129,    47,    47,   129,   129,    41,   129,    40,
      40,   118,   121,   113,    96,   100,    81,    81,    47,   129,
     129,    35,    41,    41,   119,    37,   129,   101
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    53,    54,    52,    55,    55,    55,    55,    56,
      56,    56,    56,    56,    58,    57,    59,    57,    60,    61,
      62,    57,    63,    64,    65,    57,    66,    66,    66,    66,
      67,    67,    67,    67,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    71,    70,    72,    72,    72,    74,    73,
      75,    75,    77,    76,    78,    78,    80,    79,    81,    81,
      81,    81,    82,    82,    82,    82,    82,    82,    82,    84,
      85,    83,    86,    83,    87,    88,    83,    90,    89,    91,
      89,    93,    94,    95,    96,    92,    97,    98,    99,   100,
     101,    92,   103,   102,   104,   104,   106,   107,   105,   105,
     105,   108,   109,   105,   110,   110,   112,   113,   111,   111,
     111,   114,   115,   111,   116,   116,   118,   119,   117,   117,
     117,   120,   121,   117,   123,   124,   122,   125,   122,   126,
     126,   127,   127,   128,   128,   129,   131,   130,   130,   133,
     132,   132,   135,   134,   134,   136,   136,   137,   137,   137,
     137,   137,   137,   139,   138,   140,   138,   138,   142,   141,
     143,   141,   144,   141,   141,   145,   145,   147,   146,   146,
     148,   146,   146,   146,   149,   149,   151,   150,   152,   152,
     153,   153,   153,   153,   153,   154,   154,   155,   155,   155,
     155,   155,   155
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     5,     2,     2,     2,     0,     2,
       2,     2,     2,     0,     0,     6,     0,     7,     0,     0,
       0,     8,     0,     0,     0,     9,     5,     6,     5,     6,
       4,     4,     2,     2,     3,     3,     5,     5,     3,     3,
       1,     3,     1,     0,     4,     3,     1,     0,     0,     5,
       5,     3,     0,     3,     4,     3,     0,     4,     2,     2,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     6,     0,     4,     0,     0,     5,     0,     8,     0,
      12,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,    14,     0,    12,     1,     0,     0,     0,     7,     3,
       1,     0,     0,     5,     1,     0,     0,     0,     7,     3,
       1,     0,     0,     5,     1,     0,     0,     0,     7,     3,
       1,     0,     0,     5,     0,     0,     7,     0,     5,     3,
       1,     2,     2,     1,     1,     1,     0,     4,     1,     0,
       4,     1,     0,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     0,     4,     0,     4,     1,     0,     4,
       0,     4,     0,     4,     1,     1,     1,     0,     3,     1,
       0,     4,     4,     3,     3,     1,     0,     3,     4,     3,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 307 "parser.y" /* yacc.c:1652  */
    {STV.push_back(symbolTable(globalLevelCnt++));}
#line 1827 "y.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 309 "parser.y" /* yacc.c:1652  */
    {kindVector.pop_back();}
#line 1833 "y.tab.c" /* yacc.c:1652  */
    break;

  case 4:
#line 311 "parser.y" /* yacc.c:1652  */
    {STV.back().print(); STV.pop_back();}
#line 1839 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 315 "parser.y" /* yacc.c:1652  */
    {kindVector.pop_back();}
#line 1845 "y.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 322 "parser.y" /* yacc.c:1652  */
    {kindVector.pop_back();}
#line 1851 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 324 "parser.y" /* yacc.c:1652  */
    {kindVector.pop_back();}
#line 1857 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 329 "parser.y" /* yacc.c:1652  */
    {STV.back().print(); STV.pop_back();}
#line 1863 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 330 "parser.y" /* yacc.c:1652  */
    {
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("every function should only be defined once");
                    printf(" ##########\n");
                }
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back()));
                name.pop_back();
                typeVector.pop_back();
                if(isStatementReturn==false){
                    printf("##########Error at Line #%d:", linenum);
                    printf("last statement of this function should be return");
                    printf(" ##########\n");
                }
            }
#line 1883 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 346 "parser.y" /* yacc.c:1652  */
    {STV.back().print(); STV.pop_back();}
#line 1889 "y.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 347 "parser.y" /* yacc.c:1652  */
    {
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("every function should only be defined once");
                    printf(" ##########\n");
                }
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back(), parameterList));
                name.pop_back();
                typeVector.pop_back();
                parameterList.clear();
                if(isStatementReturn==false){
                    printf("##########Error at Line #%d:", linenum);
                    printf("last statement of this function should be return");
                    printf(" ##########\n");
                }
            }
#line 1910 "y.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 364 "parser.y" /* yacc.c:1652  */
    {isVoidFunc = true;}
#line 1916 "y.tab.c" /* yacc.c:1652  */
    break;

  case 19:
#line 366 "parser.y" /* yacc.c:1652  */
    {isVoidFunc = false;}
#line 1922 "y.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 367 "parser.y" /* yacc.c:1652  */
    {STV.back().print(); STV.pop_back();}
#line 1928 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 368 "parser.y" /* yacc.c:1652  */
    {
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("every function should only be defined once");
                    printf(" ##########\n");
                }
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back()));
                name.pop_back();
                typeVector.pop_back();
            }
#line 1943 "y.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 379 "parser.y" /* yacc.c:1652  */
    {isVoidFunc = true;}
#line 1949 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 381 "parser.y" /* yacc.c:1652  */
    {isVoidFunc = false;}
#line 1955 "y.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 382 "parser.y" /* yacc.c:1652  */
    {STV.back().print(); STV.pop_back();}
#line 1961 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 383 "parser.y" /* yacc.c:1652  */
    {
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("every function should only be defined once");
                    printf(" ##########\n");
                }
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back(), parameterList));
                name.pop_back();
                typeVector.pop_back();
                parameterList.clear();
            }
#line 1977 "y.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 397 "parser.y" /* yacc.c:1652  */
    {
                typeVector.pop_back();
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("funct_decl should appear before definition");
                    printf(" ##########\n");
                }
                name.pop_back();
            }
#line 1991 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 407 "parser.y" /* yacc.c:1652  */
    {
                typeVector.pop_back();
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("funct_decl should appear before definition");
                    printf(" ##########\n");
                }
                name.pop_back();
                parameterList.clear();
            }
#line 2006 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 418 "parser.y" /* yacc.c:1652  */
    {
                typeVector.pop_back();
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("funct_decl should appear before definition");
                    printf(" ##########\n");
                }
                name.pop_back();
            }
#line 2020 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 428 "parser.y" /* yacc.c:1652  */
    {
                typeVector.pop_back();
                if(STV.back().doesExist(name.back())==true){
                    printf("##########Error at Line #%d:", linenum);
                    printf("funct_decl should appear before definition");
                    printf(" ##########\n");
                }
                name.pop_back();
                parameterList.clear();
            }
#line 2035 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 441 "parser.y" /* yacc.c:1652  */
    {
                    parameterList.push_back(instance(name.back(), parameterKind, typeVector.back()));
                    name.pop_back();
                    typeVector.pop_back();
                }
#line 2045 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 447 "parser.y" /* yacc.c:1652  */
    {
                    parameterList.push_back(instance(name.back(), parameterKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    typeVector.pop_back();
                    arrayDim.clear();
                }
#line 2056 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 454 "parser.y" /* yacc.c:1652  */
    {
                    parameterList.push_back(instance(name.back(), parameterKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    typeVector.pop_back();
                    arrayDim.clear();
                }
#line 2067 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 461 "parser.y" /* yacc.c:1652  */
    {
                    parameterList.push_back(instance(name.back(), parameterKind, typeVector.back()));
                    name.pop_back();
                    typeVector.pop_back();
                }
#line 2077 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 468 "parser.y" /* yacc.c:1652  */
    {typeVector.pop_back();}
#line 2083 "y.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 472 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back()));
                    name.pop_back();
                }
#line 2092 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 477 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back()));
                    name.pop_back();
                    
                    if(coercible(variableOrConstantDeclaredType, -1, variableTypeVector.back())==-1){
                        printf("##########Error at Line #%d:", linenum);
                        printf("initial type of LHS should be the same with that of RHS");
                        printf(" ##########\n");
                    }
                    variableTypeVector.pop_back();
                }
#line 2108 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 489 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    arrayDim.clear();
                    if(arrayDeclarationItemCnt<arrayInitItemCnt){
                        printf("##########Error at Line #%d:", linenum);
                        printf("Array init item more than declared");
                        printf(" ##########\n");
                    }
                }
#line 2123 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 500 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    arrayDim.clear();
                    
                }
#line 2134 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 507 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    arrayDim.clear();
                    if(arrayDeclarationItemCnt<arrayInitItemCnt){
                        printf("##########Error at Line #%d:", linenum);
                        printf("Array init item more than declared");
                        printf(" ##########\n");
                    }
                }
#line 2149 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 518 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back(), arrayDim));
                    name.pop_back();
                    arrayDim.clear();
                }
#line 2159 "y.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 524 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back()));
                    name.pop_back();
                    
                    variableTypeVector.pop_back();
                }
#line 2170 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 531 "parser.y" /* yacc.c:1652  */
    {
                    STV.back().addInstance(instance(name.back(), variableKind, typeVector.back()));
                    name.pop_back();
                }
#line 2179 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 537 "parser.y" /* yacc.c:1652  */
    {arrayInitItemCnt = 0;}
#line 2185 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 540 "parser.y" /* yacc.c:1652  */
    {
                if(coercible( arrayType, -1, variableTypeVector.back() )==-1){
                    printf("##########Error at Line #%d:", linenum);
                    printf("array declaration different from init item");
                    printf(" ##########\n");
                }
                arrayInitItemCnt++;
                variableTypeVector.pop_back();
            }
#line 2199 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 549 "parser.y" /* yacc.c:1652  */
    {
                if(coercible( arrayType, -1, variableTypeVector.back() )==-1){
                    printf("##########Error at Line #%d:", linenum);
                    printf("array declaration different from init item");
                    printf(" ##########\n");
                }
                arrayInitItemCnt++;
                variableTypeVector.pop_back();
            }
#line 2213 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 561 "parser.y" /* yacc.c:1652  */
    {kindVector.push_back(constantKind);}
#line 2219 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 561 "parser.y" /* yacc.c:1652  */
    {typeVector.pop_back();}
#line 2225 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 564 "parser.y" /* yacc.c:1652  */
    {
                //printf("nameSize:%d kindVec:%d typeVec:%d\n", name.size(), kindVector.size(), typeVector.size());
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back()));
                name.pop_back();
                if(coercible(variableOrConstantDeclaredType, -1, variableTypeVector.back())==-1){
                    printf("##########Error at Line #%d:", linenum);
                    printf("initial type of LHS should be the same with that of RHS");
                    printf(" ##########\n");
                }
                variableTypeVector.pop_back();
            }
#line 2241 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 576 "parser.y" /* yacc.c:1652  */
    {
                //printf("nameSize:%d kindVec:%d typeVec:%d\n", name.size(), kindVector.size(), typeVector.size());
                STV.back().addInstance(instance(name.back(), kindVector.back(), typeVector.back()));
                name.pop_back();
                if(coercible(variableOrConstantDeclaredType, -1, variableTypeVector.back())==-1){
                    printf("##########Error at Line #%d:", linenum);
                    printf("initial type of LHS should be the same with that of RHS");
                    printf(" ##########\n");
                }
                variableTypeVector.pop_back();
            }
#line 2257 "y.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 589 "parser.y" /* yacc.c:1652  */
    {arrayDeclarationItemCnt=1;}
#line 2263 "y.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 593 "parser.y" /* yacc.c:1652  */
    {
          if(externalIntLex<=0){
              printf("##########Error at Line #%d:", linenum);
              printf("declaration index must be greater than zero.");
              printf(" ##########\n");
          }
          arrayDim.push_back(externalIntLex);
          arrayDeclarationItemCnt *= externalIntLex;
      }
#line 2277 "y.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 603 "parser.y" /* yacc.c:1652  */
    {
          if(externalIntLex<=0){
              printf("##########Error at Line #%d:", linenum);
              printf("declaration index must be greater than zero.");
              printf(" ##########\n");
          }
          arrayDim.push_back(externalIntLex);
          arrayDeclarationItemCnt *= externalIntLex;
      }
#line 2291 "y.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 615 "parser.y" /* yacc.c:1652  */
    {
                        kindVector.push_back(functionKind);
                        STV.push_back(symbolTable(globalLevelCnt++));
                        addParameter();
                    }
#line 2301 "y.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 621 "parser.y" /* yacc.c:1652  */
    {globalLevelCnt--;}
#line 2307 "y.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 626 "parser.y" /* yacc.c:1652  */
    {kindVector.pop_back();}
#line 2313 "y.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 640 "parser.y" /* yacc.c:1652  */
    {
                        LHStype=variableTypeVector.back();
                        variableTypeVector.pop_back();
                    }
#line 2322 "y.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 644 "parser.y" /* yacc.c:1652  */
    {
                       if(variableRefType.back()==constantKind){
                           printf("##########Error at Line #%d:", linenum);
                           printf("no reassignment for constantKind");
                           printf(" ##########\n");
                       }
                       
                   }
#line 2335 "y.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 653 "parser.y" /* yacc.c:1652  */
    {
                        isStatementReturn=false;
                        if(coercible( LHStype, -1, variableTypeVector.back() ) == -1){
                            printf("##########Error at Line #%d:", linenum);
                            printf("right hand side not coercible to left hand side");
                            printf(" ##########\n");
                        }
                        variableRefType.pop_back();
                        variableTypeVector.pop_back();
                        
                    }
#line 2351 "y.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 664 "parser.y" /* yacc.c:1652  */
    {
                        if(isScalar(variableTypeVector.back())==false && variableTypeVector.back()!=stringType){
                            printf("##########Error at Line #%d:", linenum);
                            printf("PRINT can only print scalarTYpe or stringType");
                            printf(" ##########\n");
                        }
                        variableTypeVector.pop_back();
                        if(isArraySameDepth == false){
                            printf("##########Error at Line #%d:", linenum);
                            printf("PRINT value can only be scalar Type");
                            printf(" ##########\n");
                            isArraySameDepth = true;
                        }
                    }
#line 2370 "y.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 678 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2376 "y.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 679 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2382 "y.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 679 "parser.y" /* yacc.c:1652  */
    {
                     variableRefType.pop_back();
                     if(isScalar(variableRefType.back())==false){
                         printf("##########Error at Line #%d:", linenum);
                         printf("READ can only print scalarTYpe");
                         printf(" ##########\n");
                     }
                     variableRefType.pop_back();
                     if(isArraySameDepth == false){
                         printf("##########Error at Line #%d:", linenum);
                         printf("READ value can only be scalar Type");
                         printf(" ##########\n");
                         isArraySameDepth = true;
                     }
                 }
#line 2402 "y.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 694 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2408 "y.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 698 "parser.y" /* yacc.c:1652  */
    {
                            expressionStart = expressionCalEnd;
                            if(variableTypeVector.back()!=boolType){
                                printf("##########Error at Line #%d:", linenum);
                                printf("must be boolType in conditional statement");
                                printf(" ##########\n");
                            }
                            variableTypeVector.pop_back();
                        }
#line 2422 "y.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 708 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2428 "y.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 709 "parser.y" /* yacc.c:1652  */
    {
                          expressionStart = expressionCalEnd;
                          if(variableTypeVector.back()!=boolType){
                              printf("##########Error at Line #%d:", linenum);
                              printf("must be boolType in if parantheses");
                              printf(" ##########\n");
                          }
                          variableTypeVector.pop_back();
                      }
#line 2442 "y.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 722 "parser.y" /* yacc.c:1652  */
    {
                            isStatementReturn=false;
                            variableTypeVector.pop_back();
                        }
#line 2451 "y.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 727 "parser.y" /* yacc.c:1652  */
    {
                    expressionStart = expressionCalEnd;
                    if(variableTypeVector.back()!=boolType){
                        printf("##########Error at Line #%d:", linenum);
                        printf("must be boolType in if parantheses");
                        printf(" ##########\n");
                    }
                    variableTypeVector.pop_back();
                }
#line 2465 "y.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 738 "parser.y" /* yacc.c:1652  */
    {
                    loopLevelCnt++;
                    STV.push_back(symbolTable(globalLevelCnt++));
                    
                }
#line 2475 "y.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 744 "parser.y" /* yacc.c:1652  */
    {loopLevelCnt--;}
#line 2481 "y.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 746 "parser.y" /* yacc.c:1652  */
    {globalLevelCnt--;
                    STV.back().print();
                    STV.pop_back();}
#line 2489 "y.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 749 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2495 "y.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 751 "parser.y" /* yacc.c:1652  */
    {STV.push_back(symbolTable(globalLevelCnt++));}
#line 2501 "y.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 752 "parser.y" /* yacc.c:1652  */
    {loopLevelCnt++;}
#line 2507 "y.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 754 "parser.y" /* yacc.c:1652  */
    {loopLevelCnt--;}
#line 2513 "y.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 755 "parser.y" /* yacc.c:1652  */
    {
                      expressionStart = expressionCalEnd;
                      if(variableTypeVector.back()!=boolType){
                          printf("##########Error at Line #%d:", linenum);
                          printf("must be boolType in if parantheses");
                          printf(" ##########\n");
                      }
                      variableTypeVector.pop_back();
                  }
#line 2527 "y.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 763 "parser.y" /* yacc.c:1652  */
    {
                    globalLevelCnt--;
                    STV.back().print();
                    STV.pop_back();
                  }
#line 2537 "y.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 768 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2543 "y.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 772 "parser.y" /* yacc.c:1652  */
    {
                    //printf("incrementExpression done variableTypeVector size:%d\n", variableTypeVector.size());
                    loopLevelCnt++;
                    STV.push_back(symbolTable(globalLevelCnt++));
                }
#line 2553 "y.tab.c" /* yacc.c:1652  */
    break;

  case 93:
#line 778 "parser.y" /* yacc.c:1652  */
    {
                    loopLevelCnt--;
                    isStatementReturn=false;
                    globalLevelCnt--;
                    STV.back().print();
                    STV.pop_back();
                    
                }
#line 2566 "y.tab.c" /* yacc.c:1652  */
    break;

  case 96:
#line 792 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2572 "y.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 793 "parser.y" /* yacc.c:1652  */
    {
                        //printf("initialExpressionRule1\n");
                        if(variableRefType.size()==0){
                            ;
                        }
                        else if(variableRefType.back()==constantKind){
                            printf("##########Error at Line #%d:", linenum);
                            printf("no reassignment for constantKind");
                            printf(" ##########\n");
                            variableRefType.pop_back();
                        }
                        
                    }
#line 2590 "y.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 807 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2596 "y.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 808 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2602 "y.tab.c" /* yacc.c:1652  */
    break;

  case 100:
#line 809 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2608 "y.tab.c" /* yacc.c:1652  */
    break;

  case 101:
#line 810 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2614 "y.tab.c" /* yacc.c:1652  */
    break;

  case 102:
#line 811 "parser.y" /* yacc.c:1652  */
    {
                        //printf("initialExpressionRule4\n");
                        if(variableRefType.size()==0){
                            ;
                        }
                        else if(variableRefType.back()==constantKind){
                            printf("##########Error at Line #%d:", linenum);
                            printf("no reassignment for constantKind");
                            printf(" ##########\n");
                            variableRefType.pop_back();
                        }
                    }
#line 2631 "y.tab.c" /* yacc.c:1652  */
    break;

  case 103:
#line 824 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2637 "y.tab.c" /* yacc.c:1652  */
    break;

  case 106:
#line 830 "parser.y" /* yacc.c:1652  */
    {
                    LHStype=variableTypeVector.back();
                    variableTypeVector.pop_back();
                    }
#line 2646 "y.tab.c" /* yacc.c:1652  */
    break;

  case 107:
#line 835 "parser.y" /* yacc.c:1652  */
    {
                        //printf("controlExpression rule1\n");
                        if(variableRefType.size()==0){
                            ;
                        }
                        else if(variableRefType.back()==constantKind){
                            printf("##########Error at Line #%d:", linenum);
                            printf("no reassignment for constantKind");
                            printf(" ##########\n");
                            variableRefType.pop_back();
                        }
                    }
#line 2663 "y.tab.c" /* yacc.c:1652  */
    break;

  case 108:
#line 848 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2669 "y.tab.c" /* yacc.c:1652  */
    break;

  case 109:
#line 849 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2675 "y.tab.c" /* yacc.c:1652  */
    break;

  case 110:
#line 850 "parser.y" /* yacc.c:1652  */
    {/*printf("controlExpression rule3\n");*/ variableTypeVector.pop_back();}
#line 2681 "y.tab.c" /* yacc.c:1652  */
    break;

  case 111:
#line 852 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2687 "y.tab.c" /* yacc.c:1652  */
    break;

  case 112:
#line 853 "parser.y" /* yacc.c:1652  */
    {
                        //printf("controlExpression rule4\n");
                        if(variableRefType.size()==0){
                            ;
                        }
                        else if(variableRefType.back()==constantKind){
                            printf("##########Error at Line #%d:", linenum);
                            printf("no reassignment for constantKind");
                            printf(" ##########\n");
                            variableRefType.pop_back();
                        }
                    }
#line 2704 "y.tab.c" /* yacc.c:1652  */
    break;

  case 113:
#line 866 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2710 "y.tab.c" /* yacc.c:1652  */
    break;

  case 116:
#line 873 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2716 "y.tab.c" /* yacc.c:1652  */
    break;

  case 117:
#line 874 "parser.y" /* yacc.c:1652  */
    {
                            if(variableRefType.size()==0){
                                ;
                            }
                            else if(variableRefType.back()==constantKind){
                                printf("##########Error at Line #%d:", linenum);
                                printf("no reassignment for constantKind");
                                printf(" ##########\n");
                                variableRefType.pop_back();
                            }
                        }
#line 2732 "y.tab.c" /* yacc.c:1652  */
    break;

  case 118:
#line 886 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2738 "y.tab.c" /* yacc.c:1652  */
    break;

  case 119:
#line 887 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2744 "y.tab.c" /* yacc.c:1652  */
    break;

  case 120:
#line 888 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2750 "y.tab.c" /* yacc.c:1652  */
    break;

  case 121:
#line 889 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2756 "y.tab.c" /* yacc.c:1652  */
    break;

  case 122:
#line 890 "parser.y" /* yacc.c:1652  */
    {
                            if(variableRefType.size()==0){
                                ;
                            }
                            else if(variableRefType.back()==constantKind){
                                printf("##########Error at Line #%d:", linenum);
                                printf("no reassignment for constantKind");
                                printf(" ##########\n");
                                variableRefType.pop_back();
                            }
                        }
#line 2772 "y.tab.c" /* yacc.c:1652  */
    break;

  case 123:
#line 902 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.pop_back();}
#line 2778 "y.tab.c" /* yacc.c:1652  */
    break;

  case 124:
#line 907 "parser.y" /* yacc.c:1652  */
    {functionParameterAtInvocation=0;}
#line 2784 "y.tab.c" /* yacc.c:1652  */
    break;

  case 125:
#line 908 "parser.y" /* yacc.c:1652  */
    {
                                if(STV[0].doesExist(name.back())==false){
                                    printf("##########Error at Line #%d:", linenum);
                                    printf("function must be declared/defined before invokation\n");
                                    printf(" ##########\n");
                                }
                                if( STV[0].paraAttrCntForFunction(name.back()) != functionParameterAtInvocation ){
                                    printf("##########Error at Line #%d:", linenum);
                                    printf("parameter count should be the same as declared\n");
                                    printf(" ##########\n");
                                }
                                name.pop_back();
                            
                            }
#line 2803 "y.tab.c" /* yacc.c:1652  */
    break;

  case 126:
#line 922 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2809 "y.tab.c" /* yacc.c:1652  */
    break;

  case 127:
#line 924 "parser.y" /* yacc.c:1652  */
    {
                                  functionParameterAtInvocation = 0;
                                  if(STV[0].doesExist(name.back())==false){
                                      printf("##########Error at Line #%d:", linenum);
                                      printf("function must be declared/defined before invokation");
                                      printf(" ##########\n");
                                  }
                                  if( STV[0].paraAttrCntForFunction(name.back()) != functionParameterAtInvocation ){
                                      printf("##########Error at Line #%d:", linenum);
                                      printf("parameter count should be the same as declared");
                                      printf(" ##########\n");
                                  }
                                  name.pop_back();
                          }
#line 2828 "y.tab.c" /* yacc.c:1652  */
    break;

  case 128:
#line 938 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2834 "y.tab.c" /* yacc.c:1652  */
    break;

  case 129:
#line 942 "parser.y" /* yacc.c:1652  */
    {
                    if(isVoidFunc==true){
                        printf("##########Error at Line #%d:", linenum);
                        printf("no return in void func");
                        printf(" ##########\n");
                    }
                    else{
                        if(coercible(functionDeclaredType, -1, variableTypeVector.back() == -1)){
                            printf("##########Error at Line #%d:", linenum);
                            printf("return type does not match declared type");
                            printf(" ##########\n");
                        }
                    }
                    isStatementReturn=true;
                    variableTypeVector.pop_back();
                    
                    if(isArraySameDepth == false){
                        printf("##########Error at Line #%d:", linenum);
                        printf("return value can only be scalar Type");
                        printf(" ##########\n");
                        isArraySameDepth = true;
                    }
                }
#line 2862 "y.tab.c" /* yacc.c:1652  */
    break;

  case 130:
#line 965 "parser.y" /* yacc.c:1652  */
    {isStatementReturn=false;}
#line 2868 "y.tab.c" /* yacc.c:1652  */
    break;

  case 131:
#line 969 "parser.y" /* yacc.c:1652  */
    {
                        if(loopLevelCnt<=0){
                            printf("##########Error at Line #%d:", linenum);
                            printf("continue only in loop");
                            printf(" ##########\n");
                        }
                    }
#line 2880 "y.tab.c" /* yacc.c:1652  */
    break;

  case 132:
#line 976 "parser.y" /* yacc.c:1652  */
    {
                        if(loopLevelCnt<=0){
                            printf("##########Error at Line #%d:", linenum);
                            printf("break only in loop");
                            printf(" ##########\n");
                        }
                    }
#line 2892 "y.tab.c" /* yacc.c:1652  */
    break;

  case 133:
#line 987 "parser.y" /* yacc.c:1652  */
    {
                        for(int i=0; i<STV.size(); i++){
                            if(STV[i].dataType(name.back())!=-1){
                                int tmptype = STV[i].dataType(name.back());
                                variableTypeVector.push_back(tmptype);
                                
                                int tmpkind = STV[i].dataKind(name.back());
                                variableRefType.push_back(tmpkind);
                                
                                if( STV[i].dimension(name.back())== arrayDepthCnt && isArraySameDepth == true){
                                    //printf("linenum true:%d\n", linenum);
                                    isArraySameDepth = true;
                                }
                                else{
                                    //printf("linenum false:%d\n", linenum);
                                    isArraySameDepth = false;
                                }
                                break;
                            }
                        }
                        
                        name.pop_back();
                    }
#line 2920 "y.tab.c" /* yacc.c:1652  */
    break;

  case 134:
#line 1010 "parser.y" /* yacc.c:1652  */
    {
                       //printf("varaibleReferenceRule2Start\n");
                       //printf("STV size: %d", STV.size());
                       //printf("varRef rule2\n");
                       bool foundName = false;
                       for(int i=0; i<STV.size(); i++){
                           
                           //STV[i].print();
                           
                           if(STV[i].dataType(name.back())!=-1){
                               int tmptype = STV[i].dataType(name.back());
                               variableTypeVector.push_back(tmptype);
                               
                               int tmpkind = STV[i].dataKind(name.back());
                               variableRefType.push_back(tmpkind);
                               
                               foundName = true;
                               break;
                           }
                       }
                       if(foundName==false){
                           variableTypeVector.push_back(intType); // most general, promotable
                           variableRefType.push_back(variableKind);
                           printf("##########Error at Line #%d:", linenum);
                           printf("name must be declared before use");
                           printf(" ##########\n");
                       }
                       name.pop_back();
                       //printf("varaibleReferenceRule2End\n");
                   }
#line 2955 "y.tab.c" /* yacc.c:1652  */
    break;

  case 135:
#line 1042 "parser.y" /* yacc.c:1652  */
    {evalVariableTypeVector();}
#line 2961 "y.tab.c" /* yacc.c:1652  */
    break;

  case 136:
#line 1044 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(OR_OP);}
#line 2967 "y.tab.c" /* yacc.c:1652  */
    break;

  case 137:
#line 1044 "parser.y" /* yacc.c:1652  */
    {
                        //printf("logical Expression rule1\n");
                        
                        //evalVariableTypeVector();
                        }
#line 2977 "y.tab.c" /* yacc.c:1652  */
    break;

  case 138:
#line 1049 "parser.y" /* yacc.c:1652  */
    {
                         //printf("logical Expression rule2\n");
                         //evalVariableTypeVector();
                         
                     }
#line 2987 "y.tab.c" /* yacc.c:1652  */
    break;

  case 139:
#line 1056 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(AND_OP);}
#line 2993 "y.tab.c" /* yacc.c:1652  */
    break;

  case 142:
#line 1060 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(NOT_OP);}
#line 2999 "y.tab.c" /* yacc.c:1652  */
    break;

  case 147:
#line 1068 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(LT_OP);}
#line 3005 "y.tab.c" /* yacc.c:1652  */
    break;

  case 148:
#line 1069 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(LE_OP);}
#line 3011 "y.tab.c" /* yacc.c:1652  */
    break;

  case 149:
#line 1070 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(EQ_OP);}
#line 3017 "y.tab.c" /* yacc.c:1652  */
    break;

  case 150:
#line 1071 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(GE_OP);}
#line 3023 "y.tab.c" /* yacc.c:1652  */
    break;

  case 151:
#line 1072 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(GT_OP);}
#line 3029 "y.tab.c" /* yacc.c:1652  */
    break;

  case 152:
#line 1073 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(NE_OP);}
#line 3035 "y.tab.c" /* yacc.c:1652  */
    break;

  case 153:
#line 1076 "parser.y" /* yacc.c:1652  */
    {
                            variableTypeVector.push_back(ADD_OP);
                            //printf("addOP\n");
                        }
#line 3044 "y.tab.c" /* yacc.c:1652  */
    break;

  case 155:
#line 1081 "parser.y" /* yacc.c:1652  */
    {
                            variableTypeVector.push_back(SUB_OP);
                            //printf("subOP\n");
                        }
#line 3053 "y.tab.c" /* yacc.c:1652  */
    break;

  case 158:
#line 1089 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(MUL_OP);}
#line 3059 "y.tab.c" /* yacc.c:1652  */
    break;

  case 160:
#line 1090 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(DIV_OP);}
#line 3065 "y.tab.c" /* yacc.c:1652  */
    break;

  case 162:
#line 1091 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(MOD_OP);}
#line 3071 "y.tab.c" /* yacc.c:1652  */
    break;

  case 167:
#line 1099 "parser.y" /* yacc.c:1652  */
    {
                variableTypeVector.push_back(SUB_OP);
                //printf("subOP\n");
            }
#line 3080 "y.tab.c" /* yacc.c:1652  */
    break;

  case 170:
#line 1105 "parser.y" /* yacc.c:1652  */
    {
            variableTypeVector.push_back(L_PAREN);
            //printf("L_PAREN\n");
        }
#line 3089 "y.tab.c" /* yacc.c:1652  */
    break;

  case 171:
#line 1110 "parser.y" /* yacc.c:1652  */
    {
            variableTypeVector.push_back(R_PAREN);
            //printf("R_PAREN\n");
        }
#line 3098 "y.tab.c" /* yacc.c:1652  */
    break;

  case 172:
#line 1114 "parser.y" /* yacc.c:1652  */
    {
            variableTypeVector.push_back(STV[0].dataType(name.back()));
            name.pop_back();
        }
#line 3107 "y.tab.c" /* yacc.c:1652  */
    break;

  case 173:
#line 1118 "parser.y" /* yacc.c:1652  */
    {
            variableTypeVector.push_back(STV[0].dataType(name.back()));
            name.pop_back();
        }
#line 3116 "y.tab.c" /* yacc.c:1652  */
    break;

  case 174:
#line 1127 "parser.y" /* yacc.c:1652  */
    {functionParameterAtInvocation++;}
#line 3122 "y.tab.c" /* yacc.c:1652  */
    break;

  case 175:
#line 1128 "parser.y" /* yacc.c:1652  */
    {functionParameterAtInvocation++;}
#line 3128 "y.tab.c" /* yacc.c:1652  */
    break;

  case 176:
#line 1131 "parser.y" /* yacc.c:1652  */
    {arrayDepthCnt = 0;}
#line 3134 "y.tab.c" /* yacc.c:1652  */
    break;

  case 178:
#line 1135 "parser.y" /* yacc.c:1652  */
    {
    
    if(variableTypeVector.back()!=intType){
        printf("##########Error at Line #%d:", linenum);
        printf("array dimension should be integer type");
        printf(" ##########\n");
    }
    variableTypeVector.pop_back();
    arrayDepthCnt++;
}
#line 3149 "y.tab.c" /* yacc.c:1652  */
    break;

  case 179:
#line 1146 "parser.y" /* yacc.c:1652  */
    {
    if(variableTypeVector.back()!=intType){
        printf("##########Error at Line #%d:", linenum);
        printf("array dimension should be integer type");
        printf(" ##########\n");
    }
    variableTypeVector.pop_back();
    arrayDepthCnt++;
}
#line 3163 "y.tab.c" /* yacc.c:1652  */
    break;

  case 180:
#line 1159 "parser.y" /* yacc.c:1652  */
    {functionDeclaredType = intType;    arrayType = intType;    variableOrConstantDeclaredType=intType;}
#line 3169 "y.tab.c" /* yacc.c:1652  */
    break;

  case 181:
#line 1160 "parser.y" /* yacc.c:1652  */
    {functionDeclaredType = doubleType; arrayType = doubleType; variableOrConstantDeclaredType=doubleType;}
#line 3175 "y.tab.c" /* yacc.c:1652  */
    break;

  case 182:
#line 1161 "parser.y" /* yacc.c:1652  */
    {functionDeclaredType = stringType; arrayType = stringType; variableOrConstantDeclaredType=stringType;}
#line 3181 "y.tab.c" /* yacc.c:1652  */
    break;

  case 183:
#line 1162 "parser.y" /* yacc.c:1652  */
    {functionDeclaredType = boolType;   arrayType = boolType;   variableOrConstantDeclaredType=boolType;}
#line 3187 "y.tab.c" /* yacc.c:1652  */
    break;

  case 184:
#line 1163 "parser.y" /* yacc.c:1652  */
    {functionDeclaredType = floatType;  arrayType = floatType;  variableOrConstantDeclaredType=floatType;}
#line 3193 "y.tab.c" /* yacc.c:1652  */
    break;

  case 187:
#line 1170 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(intType); /*printf("%d\n", externalIntLex);*/}
#line 3199 "y.tab.c" /* yacc.c:1652  */
    break;

  case 188:
#line 1171 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(floatType);}
#line 3205 "y.tab.c" /* yacc.c:1652  */
    break;

  case 189:
#line 1172 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(floatType);}
#line 3211 "y.tab.c" /* yacc.c:1652  */
    break;

  case 190:
#line 1173 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(stringType);}
#line 3217 "y.tab.c" /* yacc.c:1652  */
    break;

  case 191:
#line 1174 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(boolType);}
#line 3223 "y.tab.c" /* yacc.c:1652  */
    break;

  case 192:
#line 1175 "parser.y" /* yacc.c:1652  */
    {variableTypeVector.push_back(boolType);}
#line 3229 "y.tab.c" /* yacc.c:1652  */
    break;


#line 3233 "y.tab.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1179 "parser.y" /* yacc.c:1918  */


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


