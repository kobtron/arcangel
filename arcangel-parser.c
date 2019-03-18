/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 1 "arcangel-parser.y"

    #include <assert.h>
    #include <stdlib.h>
    #include "arcangel-vm.h"
    #include "arcangel-compiler.h"

    #define opMacro(op) \
        progPopB(ip);\
        progPop(ip);\
        progOp(op, ip);\
        progPush(ip);

    #define uOpMacro(op) \
        progPop(ip);\
        progOp(op, ip);\
        progPush(ip);

    #define entryMacro(A) \
        int addr = findEntry(A.str);\
        int found = 1;\
        int isGlobal = 0;\
        if (addr == -1) {\
            addr = findGlobalEntry(A.str);\
            if (addr == -1){\
                printf("Unrecognized variable %s\n", A.str);\
                gError = 1;\
                found = 0;\
            } else {\
                isGlobal = 1;\
            }\
        } else {\
            isGlobal = 0;\
        }\

#line 63 "arcangel-parser.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 84
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE             120
#define YYNRULE              82
#define YYNTOKEN             55
#define YY_MAX_SHIFT         119
#define YY_MIN_SHIFTREDUCE   170
#define YY_MAX_SHIFTREDUCE   251
#define YY_ERROR_ACTION      252
#define YY_ACCEPT_ACTION     253
#define YY_NO_ACTION         254
#define YY_MIN_REDUCE        255
#define YY_MAX_REDUCE        336
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (918)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     5,   46,   44,   43,   42,   41,   40,   39,   38,   37,
 /*    10 */    36,   35,   34,   33,   32,   31,   30,   28,   29,   46,
 /*    20 */    44,   43,   42,   41,   40,   39,   38,   37,   36,   35,
 /*    30 */    34,   33,   32,   31,   30,   28,   29,  253,  119,  183,
 /*    40 */    42,   41,   40,   39,   38,   37,   36,   35,   34,   33,
 /*    50 */    32,   31,   30,   28,   29,  179,  171,   46,   44,   43,
 /*    60 */    42,   41,   40,   39,   38,   37,   36,   35,   34,   33,
 /*    70 */    32,   31,   30,   28,   29,   46,   44,   43,   42,   41,
 /*    80 */    40,   39,   38,   37,   36,   35,   34,   33,   32,   31,
 /*    90 */    30,   28,   29,   39,   38,   37,   36,   35,   34,   33,
 /*   100 */    32,   31,   30,   28,   29,  100,   38,   37,   36,   35,
 /*   110 */    34,   33,   32,   31,   30,   28,   29,   33,   32,   31,
 /*   120 */    30,   28,   29,  227,  174,   46,   44,   43,   42,   41,
 /*   130 */    40,   39,   38,   37,   36,   35,   34,   33,   32,   31,
 /*   140 */    30,   28,   29,   35,   34,   33,   32,   31,   30,   28,
 /*   150 */    29,  224,  180,   46,   44,   43,   42,   41,   40,   39,
 /*   160 */    38,   37,   36,   35,   34,   33,   32,   31,   30,   28,
 /*   170 */    29,   34,   33,   32,   31,   30,   28,   29,    7,  249,
 /*   180 */     3,   46,   44,   43,   42,   41,   40,   39,   38,   37,
 /*   190 */    36,   35,   34,   33,   32,   31,   30,   28,   29,   32,
 /*   200 */    31,   30,   28,   29,   19,  103,    6,  223,    2,   46,
 /*   210 */    44,   43,   42,   41,   40,   39,   38,   37,   36,   35,
 /*   220 */    34,   33,   32,   31,   30,   28,   29,   31,   30,   28,
 /*   230 */    29,  231,  233,   13,   12,  222,  226,   46,   44,   43,
 /*   240 */    42,   41,   40,   39,   38,   37,   36,   35,   34,   33,
 /*   250 */    32,   31,   30,   28,   29,   30,   28,   29,   18,   28,
 /*   260 */    29,  333,  331,  221,   57,   46,   44,   43,   42,   41,
 /*   270 */    40,   39,   38,   37,   36,   35,   34,   33,   32,   31,
 /*   280 */    30,   28,   29,  319,   56,   59,  258,   58,    8,   50,
 /*   290 */   329,  176,  327,   46,   44,   43,   42,   41,   40,   39,
 /*   300 */    38,   37,   36,   35,   34,   33,   32,   31,   30,   28,
 /*   310 */    29,   10,  190,  196,    1,   98,   11,   15,  181,  175,
 /*   320 */   177,   46,   44,   43,   42,   41,   40,   39,   38,   37,
 /*   330 */    36,   35,   34,   33,   32,   31,   30,   28,   29,   44,
 /*   340 */    43,   42,   41,   40,   39,   38,   37,   36,   35,   34,
 /*   350 */    33,   32,   31,   30,   28,   29,   43,   42,   41,   40,
 /*   360 */    39,   38,   37,   36,   35,   34,   33,   32,   31,   30,
 /*   370 */    28,   29,   92,  178,  104,   73,  105,   27,   26,   25,
 /*   380 */    24,  225,  110,  109,   20,   73,  114,  276,   21,  274,
 /*   390 */    53,   22,   73,   23,   29,   45,   94,  115,  112,  106,
 /*   400 */    17,   61,  182,  192,  193,  185,  186,  187,   14,  108,
 /*   410 */   229,  230,  232,   60,  197,   41,   40,   39,   38,   37,
 /*   420 */    36,   35,   34,   33,   32,   31,   30,   28,   29,   40,
 /*   430 */    39,   38,   37,   36,   35,   34,   33,   32,   31,   30,
 /*   440 */    28,   29,   37,   36,   35,   34,   33,   32,   31,   30,
 /*   450 */    28,   29,   36,   35,   34,   33,   32,   31,   30,   28,
 /*   460 */    29,   77,   48,   47,  255,  254,  118,  116,   63,   77,
 /*   470 */   254,   77,  114,  276,  254,   97,   55,   77,   77,   77,
 /*   480 */   114,  276,  254,  107,   55,  254,   77,   77,  114,  276,
 /*   490 */   254,  254,   54,  101,   77,  254,    9,    9,  274,  254,
 /*   500 */   254,  254,   16,  102,    4,  254,    9,    9,  254,  113,
 /*   510 */    62,   99,   16,  324,    4,  254,    9,    9,  254,  254,
 /*   520 */   254,   99,   16,  254,    4,  254,  254,  254,  254,   49,
 /*   530 */   254,   99,    9,    9,  254,  254,  254,  254,   16,   96,
 /*   540 */     4,  254,    9,    9,  254,  254,  254,   99,   16,  321,
 /*   550 */     4,  254,    9,    9,  254,  254,  254,   99,   16,  254,
 /*   560 */     4,  254,  117,  254,  254,    9,    9,   99,  254,  254,
 /*   570 */   254,   16,   64,    4,  254,  254,   74,  254,  254,  254,
 /*   580 */    99,  254,   64,  114,  276,  254,   74,  114,  276,   64,
 /*   590 */   254,  254,  254,   74,   75,  254,  254,   76,  254,  254,
 /*   600 */   254,  254,  254,  254,   75,  114,  276,   76,  114,  276,
 /*   610 */   254,   75,   51,  254,   76,   52,  254,  254,  254,  254,
 /*   620 */   254,  254,   51,  114,  276,   52,  114,  276,  254,   51,
 /*   630 */    65,  254,   52,  254,  254,  254,  254,  254,   66,  254,
 /*   640 */    65,  114,  276,  254,  254,  254,  254,   65,   66,  114,
 /*   650 */   276,   67,  254,  254,   68,   66,  254,  254,  254,  254,
 /*   660 */   254,   67,  114,  276,   68,  114,  276,  254,   67,   69,
 /*   670 */   254,   68,   70,  254,  254,  254,  254,  254,  254,   69,
 /*   680 */   114,  276,   70,  114,  276,  254,   69,  305,  254,   70,
 /*   690 */   304,  254,  254,  254,  254,  254,  254,  305,  114,  276,
 /*   700 */   304,  114,  276,  303,  305,  254,  302,  304,  254,  254,
 /*   710 */   254,  254,  254,  303,  114,  276,  302,  114,  276,  111,
 /*   720 */   303,  254,  254,  302,  254,  254,  254,  300,  254,  111,
 /*   730 */   114,  276,  254,  254,  254,  254,  111,  300,  114,  276,
 /*   740 */   254,  254,  254,  254,  300,   95,  254,  254,   93,  254,
 /*   750 */   254,  254,  254,  254,  254,   95,  114,  276,   93,  114,
 /*   760 */   276,  254,   95,   91,  254,   93,   90,  254,  254,  254,
 /*   770 */   254,  254,  254,   91,  114,  276,   90,  114,  276,   89,
 /*   780 */    91,  254,   88,   90,  254,  254,  254,  254,  254,   89,
 /*   790 */   114,  276,   88,  114,  276,   87,   89,  254,  254,   88,
 /*   800 */   254,  254,  254,   86,  254,   87,  114,  276,  254,  254,
 /*   810 */   254,  254,   87,   86,  114,  276,  254,  254,  254,  254,
 /*   820 */    86,   85,  254,  254,   84,  254,  254,  254,  254,  254,
 /*   830 */   254,   85,  114,  276,   84,  114,  276,  254,   85,   83,
 /*   840 */   254,   84,   82,  254,  254,  254,  254,  254,  254,   83,
 /*   850 */   114,  276,   82,  114,  276,   81,   83,  254,   80,   82,
 /*   860 */   254,  254,  254,  254,  254,   81,  114,  276,   80,  114,
 /*   870 */   276,   79,   81,  254,  254,   80,  254,  254,  254,   71,
 /*   880 */   254,   79,  114,  276,  254,  254,  254,  254,   79,   71,
 /*   890 */   114,  276,  254,  254,  254,  254,   71,   78,  254,  254,
 /*   900 */    72,  254,  254,  254,  254,  254,  254,   78,  114,  276,
 /*   910 */    72,  114,  276,  254,   78,  254,  254,   72,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    69,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    1,
 /*    20 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*    30 */    12,   13,   14,   15,   16,   17,   18,   56,   57,   39,
 /*    40 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*    50 */    14,   15,   16,   17,   18,   37,   29,    1,    2,    3,
 /*    60 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*    70 */    14,   15,   16,   17,   18,    1,    2,    3,    4,    5,
 /*    80 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*    90 */    16,   17,   18,    7,    8,    9,   10,   11,   12,   13,
 /*   100 */    14,   15,   16,   17,   18,   49,    8,    9,   10,   11,
 /*   110 */    12,   13,   14,   15,   16,   17,   18,   13,   14,   15,
 /*   120 */    16,   17,   18,   49,   32,    1,    2,    3,    4,    5,
 /*   130 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   140 */    16,   17,   18,   11,   12,   13,   14,   15,   16,   17,
 /*   150 */    18,   27,   23,    1,    2,    3,    4,    5,    6,    7,
 /*   160 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   170 */    18,   12,   13,   14,   15,   16,   17,   18,   65,   27,
 /*   180 */    31,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   190 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   14,
 /*   200 */    15,   16,   17,   18,   26,   66,   67,   27,   82,    1,
 /*   210 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   220 */    12,   13,   14,   15,   16,   17,   18,   15,   16,   17,
 /*   230 */    18,   53,   54,   47,   48,   27,   26,    1,    2,    3,
 /*   240 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*   250 */    14,   15,   16,   17,   18,   16,   17,   18,   48,   17,
 /*   260 */    18,   80,   80,   27,   31,    1,    2,    3,    4,    5,
 /*   270 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   280 */    16,   17,   18,   61,   60,   30,   61,   60,   58,   31,
 /*   290 */    73,   27,   73,    1,    2,    3,    4,    5,    6,    7,
 /*   300 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   310 */    18,   58,   28,   27,   26,   32,   47,   40,   28,   27,
 /*   320 */    27,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   330 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    2,
 /*   340 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   350 */    13,   14,   15,   16,   17,   18,    3,    4,    5,    6,
 /*   360 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   370 */    17,   18,   14,   28,   32,   64,   26,   19,   20,   21,
 /*   380 */    22,   27,   24,   25,   26,   74,   75,   76,   26,   60,
 /*   390 */    32,   26,   81,   26,   18,   26,   32,   33,   34,   35,
 /*   400 */    36,   72,   38,   45,   46,   41,   42,   43,   44,   51,
 /*   410 */    52,   53,   54,   27,   50,    5,    6,    7,    8,    9,
 /*   420 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    6,
 /*   430 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   440 */    17,   18,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   450 */    17,   18,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   460 */    18,   64,   26,   26,    0,   83,   26,   28,   27,   64,
 /*   470 */    83,   74,   75,   76,   83,   78,   79,   64,   81,   74,
 /*   480 */    75,   76,   83,   78,   79,   83,   81,   74,   75,   76,
 /*   490 */    83,   83,   79,   59,   81,   83,   62,   63,   60,   83,
 /*   500 */    83,   83,   68,   59,   70,   83,   62,   63,   83,   71,
 /*   510 */    72,   77,   68,   59,   70,   83,   62,   63,   83,   83,
 /*   520 */    83,   77,   68,   83,   70,   83,   83,   83,   83,   59,
 /*   530 */    83,   77,   62,   63,   83,   83,   83,   83,   68,   59,
 /*   540 */    70,   83,   62,   63,   83,   83,   83,   77,   68,   59,
 /*   550 */    70,   83,   62,   63,   83,   83,   83,   77,   68,   83,
 /*   560 */    70,   83,   59,   83,   83,   62,   63,   77,   83,   83,
 /*   570 */    83,   68,   64,   70,   83,   83,   64,   83,   83,   83,
 /*   580 */    77,   83,   74,   75,   76,   83,   74,   75,   76,   81,
 /*   590 */    83,   83,   83,   81,   64,   83,   83,   64,   83,   83,
 /*   600 */    83,   83,   83,   83,   74,   75,   76,   74,   75,   76,
 /*   610 */    83,   81,   64,   83,   81,   64,   83,   83,   83,   83,
 /*   620 */    83,   83,   74,   75,   76,   74,   75,   76,   83,   81,
 /*   630 */    64,   83,   81,   83,   83,   83,   83,   83,   64,   83,
 /*   640 */    74,   75,   76,   83,   83,   83,   83,   81,   74,   75,
 /*   650 */    76,   64,   83,   83,   64,   81,   83,   83,   83,   83,
 /*   660 */    83,   74,   75,   76,   74,   75,   76,   83,   81,   64,
 /*   670 */    83,   81,   64,   83,   83,   83,   83,   83,   83,   74,
 /*   680 */    75,   76,   74,   75,   76,   83,   81,   64,   83,   81,
 /*   690 */    64,   83,   83,   83,   83,   83,   83,   74,   75,   76,
 /*   700 */    74,   75,   76,   64,   81,   83,   64,   81,   83,   83,
 /*   710 */    83,   83,   83,   74,   75,   76,   74,   75,   76,   64,
 /*   720 */    81,   83,   83,   81,   83,   83,   83,   64,   83,   74,
 /*   730 */    75,   76,   83,   83,   83,   83,   81,   74,   75,   76,
 /*   740 */    83,   83,   83,   83,   81,   64,   83,   83,   64,   83,
 /*   750 */    83,   83,   83,   83,   83,   74,   75,   76,   74,   75,
 /*   760 */    76,   83,   81,   64,   83,   81,   64,   83,   83,   83,
 /*   770 */    83,   83,   83,   74,   75,   76,   74,   75,   76,   64,
 /*   780 */    81,   83,   64,   81,   83,   83,   83,   83,   83,   74,
 /*   790 */    75,   76,   74,   75,   76,   64,   81,   83,   83,   81,
 /*   800 */    83,   83,   83,   64,   83,   74,   75,   76,   83,   83,
 /*   810 */    83,   83,   81,   74,   75,   76,   83,   83,   83,   83,
 /*   820 */    81,   64,   83,   83,   64,   83,   83,   83,   83,   83,
 /*   830 */    83,   74,   75,   76,   74,   75,   76,   83,   81,   64,
 /*   840 */    83,   81,   64,   83,   83,   83,   83,   83,   83,   74,
 /*   850 */    75,   76,   74,   75,   76,   64,   81,   83,   64,   81,
 /*   860 */    83,   83,   83,   83,   83,   74,   75,   76,   74,   75,
 /*   870 */    76,   64,   81,   83,   83,   81,   83,   83,   83,   64,
 /*   880 */    83,   74,   75,   76,   83,   83,   83,   83,   81,   74,
 /*   890 */    75,   76,   83,   83,   83,   83,   81,   64,   83,   83,
 /*   900 */    64,   83,   83,   83,   83,   83,   83,   74,   75,   76,
 /*   910 */    74,   75,   76,   83,   81,   83,   83,   81,
};
#define YY_SHIFT_COUNT    (119)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (464)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */    27,  358,  358,  358,  364,  364,  364,  364,  364,  364,
 /*    10 */   364,  358,  358,  358,  358,  358,  358,  358,  358,  358,
 /*    20 */   358,  358,  358,  358,  358,  358,  358,  358,  358,  358,
 /*    30 */   358,  358,  358,  358,  358,  358,  358,  358,  358,  358,
 /*    40 */   358,  358,  358,  358,  358,  358,  358,  358,   92,  129,
 /*    50 */    92,    0,   18,  210,  149,  149,  233,   92,  233,   92,
 /*    60 */   255,  258,  258,  255,   56,   74,  124,  152,  180,  208,
 /*    70 */   236,  264,  292,  320,  320,  320,  320,  320,  337,  353,
 /*    80 */    36,  410,  423,   86,   98,  433,  442,  132,  159,  104,
 /*    90 */   185,  212,  178,  239,  186,  242,  284,  286,  288,  283,
 /*   100 */   269,  277,  290,  345,  293,  342,  350,  354,  362,  365,
 /*   110 */   367,  376,  369,  386,  436,  437,  464,  439,  441,  440,
};
#define YY_REDUCE_COUNT (63)
#define YY_REDUCE_MIN   (-69)
#define YY_REDUCE_MAX   (836)
static const short yy_reduce_ofst[] = {
 /*     0 */   -19,  397,  405,  413,  434,  444,  454,  470,  480,  490,
 /*    10 */   503,  311,  508,  512,  530,  533,  548,  551,  566,  574,
 /*    20 */   587,  590,  605,  608,  623,  626,  639,  642,  655,  663,
 /*    30 */   681,  684,  699,  702,  715,  718,  731,  739,  757,  760,
 /*    40 */   775,  778,  791,  794,  807,  815,  833,  836,  438,  139,
 /*    50 */   329,  -69,  113,  126,  181,  182,  222,  224,  225,  227,
 /*    60 */   230,  217,  219,  253,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   252,  330,  330,  252,  322,  322,  322,  322,  322,  322,
 /*    10 */   322,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*    20 */   252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*    30 */   252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*    40 */   252,  252,  252,  252,  252,  252,  252,  252,  326,  325,
 /*    50 */   252,  252,  252,  313,  332,  332,  320,  252,  320,  252,
 /*    60 */   257,  328,  328,  257,  252,  252,  252,  252,  252,  252,
 /*    70 */   252,  252,  252,  280,  279,  273,  269,  283,  284,  285,
 /*    80 */   286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
 /*    90 */   296,  297,  252,  298,  252,  299,  252,  252,  252,  252,
 /*   100 */   252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*   110 */   252,  301,  252,  252,  252,  252,  252,  252,  252,  252,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "OR",
  /*    2 */ "AND",
  /*    3 */ "BOR",
  /*    4 */ "BXOR",
  /*    5 */ "BAND",
  /*    6 */ "NEQ",
  /*    7 */ "EQ",
  /*    8 */ "GT",
  /*    9 */ "GTE",
  /*   10 */ "LT",
  /*   11 */ "LTE",
  /*   12 */ "SR",
  /*   13 */ "SL",
  /*   14 */ "SUB",
  /*   15 */ "SUM",
  /*   16 */ "MOD",
  /*   17 */ "DIV",
  /*   18 */ "MULT",
  /*   19 */ "INC",
  /*   20 */ "DEC",
  /*   21 */ "NOT",
  /*   22 */ "BNOT",
  /*   23 */ "ELSE",
  /*   24 */ "INTCAST",
  /*   25 */ "FLOATCAST",
  /*   26 */ "LPAR",
  /*   27 */ "RPAR",
  /*   28 */ "END",
  /*   29 */ "PROGRAM",
  /*   30 */ "VAR",
  /*   31 */ "COMMA",
  /*   32 */ "ID",
  /*   33 */ "PRINT",
  /*   34 */ "PRINTS",
  /*   35 */ "FREE",
  /*   36 */ "IF",
  /*   37 */ "THEN",
  /*   38 */ "WHILE",
  /*   39 */ "DO",
  /*   40 */ "UNTIL",
  /*   41 */ "REPEAT",
  /*   42 */ "BREAK",
  /*   43 */ "CONTINUE",
  /*   44 */ "RETURN",
  /*   45 */ "FUNCTION",
  /*   46 */ "CALLBACK",
  /*   47 */ "ASGN",
  /*   48 */ "LBRK",
  /*   49 */ "RBRK",
  /*   50 */ "CALL",
  /*   51 */ "NEW",
  /*   52 */ "STR",
  /*   53 */ "INTEGER",
  /*   54 */ "FLOAT",
  /*   55 */ "error",
  /*   56 */ "program",
  /*   57 */ "programStart",
  /*   58 */ "decls",
  /*   59 */ "stmts",
  /*   60 */ "idDecl",
  /*   61 */ "declTail",
  /*   62 */ "stmt",
  /*   63 */ "asgn",
  /*   64 */ "expr",
  /*   65 */ "ifStart",
  /*   66 */ "else",
  /*   67 */ "elseStart",
  /*   68 */ "whileStart",
  /*   69 */ "whileCond",
  /*   70 */ "repeatStart",
  /*   71 */ "parsList",
  /*   72 */ "par",
  /*   73 */ "parsListTail",
  /*   74 */ "fn",
  /*   75 */ "fnStart",
  /*   76 */ "fnStartKw",
  /*   77 */ "callStart",
  /*   78 */ "argsList",
  /*   79 */ "arg",
  /*   80 */ "argsListTail",
  /*   81 */ "number",
  /*   82 */ "lParStart",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program ::= programStart LPAR RPAR decls stmts END",
 /*   1 */ "programStart ::= PROGRAM",
 /*   2 */ "decls ::=",
 /*   3 */ "decls ::= VAR idDecl declTail",
 /*   4 */ "idDecl ::= ID",
 /*   5 */ "stmt ::= PRINT LPAR expr RPAR",
 /*   6 */ "stmt ::= PRINTS LPAR expr RPAR",
 /*   7 */ "stmt ::= FREE LPAR ID RPAR",
 /*   8 */ "stmt ::= IF expr ifStart stmts else END",
 /*   9 */ "ifStart ::= THEN",
 /*  10 */ "elseStart ::= ELSE",
 /*  11 */ "stmt ::= whileStart expr whileCond stmts END",
 /*  12 */ "whileStart ::= WHILE",
 /*  13 */ "whileCond ::= DO",
 /*  14 */ "stmt ::= repeatStart stmts UNTIL expr",
 /*  15 */ "repeatStart ::= REPEAT",
 /*  16 */ "stmt ::= BREAK",
 /*  17 */ "stmt ::= CONTINUE",
 /*  18 */ "stmt ::= RETURN expr",
 /*  19 */ "par ::= idDecl",
 /*  20 */ "fn ::= fnStart LPAR parsList RPAR decls stmts END",
 /*  21 */ "fnStart ::= fnStartKw",
 /*  22 */ "fnStartKw ::= FUNCTION",
 /*  23 */ "fnStartKw ::= CALLBACK",
 /*  24 */ "asgn ::= ID ASGN expr",
 /*  25 */ "asgn ::= ID LBRK expr RBRK ASGN expr",
 /*  26 */ "stmt ::= callStart ID LPAR argsList RPAR",
 /*  27 */ "callStart ::= CALL",
 /*  28 */ "arg ::= expr",
 /*  29 */ "expr ::= expr OR expr",
 /*  30 */ "expr ::= expr AND expr",
 /*  31 */ "expr ::= expr BOR expr",
 /*  32 */ "expr ::= expr BXOR expr",
 /*  33 */ "expr ::= expr BAND expr",
 /*  34 */ "expr ::= expr NEQ expr",
 /*  35 */ "expr ::= expr EQ expr",
 /*  36 */ "expr ::= expr GT expr",
 /*  37 */ "expr ::= expr GTE expr",
 /*  38 */ "expr ::= expr LT expr",
 /*  39 */ "expr ::= expr LTE expr",
 /*  40 */ "expr ::= expr SR expr",
 /*  41 */ "expr ::= expr SL expr",
 /*  42 */ "expr ::= expr SUB expr",
 /*  43 */ "expr ::= expr SUM expr",
 /*  44 */ "expr ::= expr MOD expr",
 /*  45 */ "expr ::= expr MULT expr",
 /*  46 */ "expr ::= expr DIV expr",
 /*  47 */ "expr ::= INC expr",
 /*  48 */ "expr ::= DEC expr",
 /*  49 */ "expr ::= NOT expr",
 /*  50 */ "expr ::= BNOT expr",
 /*  51 */ "expr ::= INTCAST LPAR expr RPAR",
 /*  52 */ "expr ::= FLOATCAST LPAR expr RPAR",
 /*  53 */ "expr ::= NEW LPAR expr RPAR",
 /*  54 */ "expr ::= SUB LPAR expr RPAR",
 /*  55 */ "expr ::= ID lParStart argsList RPAR",
 /*  56 */ "lParStart ::= LPAR",
 /*  57 */ "expr ::= ID LBRK expr RBRK",
 /*  58 */ "expr ::= ID",
 /*  59 */ "expr ::= STR",
 /*  60 */ "number ::= INTEGER",
 /*  61 */ "number ::= SUB INTEGER",
 /*  62 */ "number ::= FLOAT",
 /*  63 */ "number ::= SUB FLOAT",
 /*  64 */ "declTail ::= COMMA idDecl declTail",
 /*  65 */ "declTail ::=",
 /*  66 */ "stmts ::= stmt stmts",
 /*  67 */ "stmts ::=",
 /*  68 */ "stmt ::= asgn",
 /*  69 */ "else ::= elseStart stmts",
 /*  70 */ "else ::=",
 /*  71 */ "parsList ::=",
 /*  72 */ "parsList ::= par parsListTail",
 /*  73 */ "parsListTail ::=",
 /*  74 */ "parsListTail ::= COMMA par parsListTail",
 /*  75 */ "argsList ::=",
 /*  76 */ "argsList ::= arg argsListTail",
 /*  77 */ "argsListTail ::=",
 /*  78 */ "argsListTail ::= COMMA arg argsListTail",
 /*  79 */ "expr ::= LPAR expr RPAR",
 /*  80 */ "expr ::= number",
 /*  81 */ "expr ::= fn",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ParseInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) ParseInit(pParser);
  return pParser;
}
#endif /* Parse_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
      /* TERMINAL Destructor */
    case 1: /* OR */
    case 2: /* AND */
    case 3: /* BOR */
    case 4: /* BXOR */
    case 5: /* BAND */
    case 6: /* NEQ */
    case 7: /* EQ */
    case 8: /* GT */
    case 9: /* GTE */
    case 10: /* LT */
    case 11: /* LTE */
    case 12: /* SR */
    case 13: /* SL */
    case 14: /* SUB */
    case 15: /* SUM */
    case 16: /* MOD */
    case 17: /* DIV */
    case 18: /* MULT */
    case 19: /* INC */
    case 20: /* DEC */
    case 21: /* NOT */
    case 22: /* BNOT */
    case 23: /* ELSE */
    case 24: /* INTCAST */
    case 25: /* FLOATCAST */
    case 26: /* LPAR */
    case 27: /* RPAR */
    case 28: /* END */
    case 29: /* PROGRAM */
    case 30: /* VAR */
    case 31: /* COMMA */
    case 32: /* ID */
    case 33: /* PRINT */
    case 34: /* PRINTS */
    case 35: /* FREE */
    case 36: /* IF */
    case 37: /* THEN */
    case 38: /* WHILE */
    case 39: /* DO */
    case 40: /* UNTIL */
    case 41: /* REPEAT */
    case 42: /* BREAK */
    case 43: /* CONTINUE */
    case 44: /* RETURN */
    case 45: /* FUNCTION */
    case 46: /* CALLBACK */
    case 47: /* ASGN */
    case 48: /* LBRK */
    case 49: /* RBRK */
    case 50: /* CALL */
    case 51: /* NEW */
    case 52: /* STR */
    case 53: /* INTEGER */
    case 54: /* FLOAT */
{
#line 39 "arcangel-parser.y"

    token_destructor((yypminor->yy0));

#line 903 "arcangel-parser.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ParseFinalize(p);
  (*freeProc)(p);
}
#endif /* Parse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int ParseCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 && i+YYNTOKEN<=sizeof(yy_lookahead)/sizeof(yy_lookahead[0]) );
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   56,   -6 }, /* (0) program ::= programStart LPAR RPAR decls stmts END */
  {   57,   -1 }, /* (1) programStart ::= PROGRAM */
  {   58,    0 }, /* (2) decls ::= */
  {   58,   -3 }, /* (3) decls ::= VAR idDecl declTail */
  {   60,   -1 }, /* (4) idDecl ::= ID */
  {   62,   -4 }, /* (5) stmt ::= PRINT LPAR expr RPAR */
  {   62,   -4 }, /* (6) stmt ::= PRINTS LPAR expr RPAR */
  {   62,   -4 }, /* (7) stmt ::= FREE LPAR ID RPAR */
  {   62,   -6 }, /* (8) stmt ::= IF expr ifStart stmts else END */
  {   65,   -1 }, /* (9) ifStart ::= THEN */
  {   67,   -1 }, /* (10) elseStart ::= ELSE */
  {   62,   -5 }, /* (11) stmt ::= whileStart expr whileCond stmts END */
  {   68,   -1 }, /* (12) whileStart ::= WHILE */
  {   69,   -1 }, /* (13) whileCond ::= DO */
  {   62,   -4 }, /* (14) stmt ::= repeatStart stmts UNTIL expr */
  {   70,   -1 }, /* (15) repeatStart ::= REPEAT */
  {   62,   -1 }, /* (16) stmt ::= BREAK */
  {   62,   -1 }, /* (17) stmt ::= CONTINUE */
  {   62,   -2 }, /* (18) stmt ::= RETURN expr */
  {   72,   -1 }, /* (19) par ::= idDecl */
  {   74,   -7 }, /* (20) fn ::= fnStart LPAR parsList RPAR decls stmts END */
  {   75,   -1 }, /* (21) fnStart ::= fnStartKw */
  {   76,   -1 }, /* (22) fnStartKw ::= FUNCTION */
  {   76,   -1 }, /* (23) fnStartKw ::= CALLBACK */
  {   63,   -3 }, /* (24) asgn ::= ID ASGN expr */
  {   63,   -6 }, /* (25) asgn ::= ID LBRK expr RBRK ASGN expr */
  {   62,   -5 }, /* (26) stmt ::= callStart ID LPAR argsList RPAR */
  {   77,   -1 }, /* (27) callStart ::= CALL */
  {   79,   -1 }, /* (28) arg ::= expr */
  {   64,   -3 }, /* (29) expr ::= expr OR expr */
  {   64,   -3 }, /* (30) expr ::= expr AND expr */
  {   64,   -3 }, /* (31) expr ::= expr BOR expr */
  {   64,   -3 }, /* (32) expr ::= expr BXOR expr */
  {   64,   -3 }, /* (33) expr ::= expr BAND expr */
  {   64,   -3 }, /* (34) expr ::= expr NEQ expr */
  {   64,   -3 }, /* (35) expr ::= expr EQ expr */
  {   64,   -3 }, /* (36) expr ::= expr GT expr */
  {   64,   -3 }, /* (37) expr ::= expr GTE expr */
  {   64,   -3 }, /* (38) expr ::= expr LT expr */
  {   64,   -3 }, /* (39) expr ::= expr LTE expr */
  {   64,   -3 }, /* (40) expr ::= expr SR expr */
  {   64,   -3 }, /* (41) expr ::= expr SL expr */
  {   64,   -3 }, /* (42) expr ::= expr SUB expr */
  {   64,   -3 }, /* (43) expr ::= expr SUM expr */
  {   64,   -3 }, /* (44) expr ::= expr MOD expr */
  {   64,   -3 }, /* (45) expr ::= expr MULT expr */
  {   64,   -3 }, /* (46) expr ::= expr DIV expr */
  {   64,   -2 }, /* (47) expr ::= INC expr */
  {   64,   -2 }, /* (48) expr ::= DEC expr */
  {   64,   -2 }, /* (49) expr ::= NOT expr */
  {   64,   -2 }, /* (50) expr ::= BNOT expr */
  {   64,   -4 }, /* (51) expr ::= INTCAST LPAR expr RPAR */
  {   64,   -4 }, /* (52) expr ::= FLOATCAST LPAR expr RPAR */
  {   64,   -4 }, /* (53) expr ::= NEW LPAR expr RPAR */
  {   64,   -4 }, /* (54) expr ::= SUB LPAR expr RPAR */
  {   64,   -4 }, /* (55) expr ::= ID lParStart argsList RPAR */
  {   82,   -1 }, /* (56) lParStart ::= LPAR */
  {   64,   -4 }, /* (57) expr ::= ID LBRK expr RBRK */
  {   64,   -1 }, /* (58) expr ::= ID */
  {   64,   -1 }, /* (59) expr ::= STR */
  {   81,   -1 }, /* (60) number ::= INTEGER */
  {   81,   -2 }, /* (61) number ::= SUB INTEGER */
  {   81,   -1 }, /* (62) number ::= FLOAT */
  {   81,   -2 }, /* (63) number ::= SUB FLOAT */
  {   61,   -3 }, /* (64) declTail ::= COMMA idDecl declTail */
  {   61,    0 }, /* (65) declTail ::= */
  {   59,   -2 }, /* (66) stmts ::= stmt stmts */
  {   59,    0 }, /* (67) stmts ::= */
  {   62,   -1 }, /* (68) stmt ::= asgn */
  {   66,   -2 }, /* (69) else ::= elseStart stmts */
  {   66,    0 }, /* (70) else ::= */
  {   71,    0 }, /* (71) parsList ::= */
  {   71,   -2 }, /* (72) parsList ::= par parsListTail */
  {   73,    0 }, /* (73) parsListTail ::= */
  {   73,   -3 }, /* (74) parsListTail ::= COMMA par parsListTail */
  {   78,    0 }, /* (75) argsList ::= */
  {   78,   -2 }, /* (76) argsList ::= arg argsListTail */
  {   80,    0 }, /* (77) argsListTail ::= */
  {   80,   -3 }, /* (78) argsListTail ::= COMMA arg argsListTail */
  {   64,   -3 }, /* (79) expr ::= LPAR expr RPAR */
  {   64,   -1 }, /* (80) expr ::= number */
  {   64,   -1 }, /* (81) expr ::= fn */
};

static void yy_acceptt(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  ParseTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* program ::= programStart LPAR RPAR decls stmts END */
#line 80 "arcangel-parser.y"
{
    progEnd(ip);
    Entries* entries = arcangel_Stack_pop(entriesStack);
    free(entries);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
#line 1366 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-4].minor);
  yy_destructor(yypParser,27,&yymsp[-3].minor);
  yy_destructor(yypParser,28,&yymsp[0].minor);
        break;
      case 1: /* programStart ::= PROGRAM */
{  yy_destructor(yypParser,29,&yymsp[0].minor);
#line 87 "arcangel-parser.y"
{
    Entries* entries = calloc(1, sizeof(Entries));
    arcangel_Stack_push(entriesStack, entries);
    arcangel_Stack_push(loopStartStack, (void*)-1);
    arcangel_Stack_push(loopEndStack, (void*)-1);
    parsSize = 0;
}
#line 1381 "arcangel-parser.c"
}
        break;
      case 2: /* decls ::= */
#line 95 "arcangel-parser.y"
{
    Entries* entries = arcangel_Stack_top(entriesStack);
    progHeap(entries->entriesSize, ip);
    int i;
    for (i = 0; i < parsSize; ++i) {
        progSGet((parsSize - 1) - i, ip);
        progSet(i, ip);
    }
}
#line 1395 "arcangel-parser.c"
        break;
      case 3: /* decls ::= VAR idDecl declTail */
{  yy_destructor(yypParser,30,&yymsp[-2].minor);
#line 104 "arcangel-parser.y"
{
    Entries* entries = arcangel_Stack_top(entriesStack);
    progHeap(entries->entriesSize, ip);
    int i;
    for (i = 0; i < parsSize; ++i) {
        progSGet((parsSize - 1) - i, ip);
        progSet(i, ip);
    }
}
#line 1409 "arcangel-parser.c"
}
        break;
      case 4: /* idDecl ::= ID */
#line 116 "arcangel-parser.y"
{
    int found = addEntry(yymsp[0].minor.yy0.str);
    if (found) {
        printf("Repeated variable %s\n", yymsp[0].minor.yy0.str);
        gError = 1;
    }
}
#line 1421 "arcangel-parser.c"
        break;
      case 5: /* stmt ::= PRINT LPAR expr RPAR */
{  yy_destructor(yypParser,33,&yymsp[-3].minor);
#line 129 "arcangel-parser.y"
{
    progPop(ip);
    progPrint(ip);
}
#line 1430 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 6: /* stmt ::= PRINTS LPAR expr RPAR */
{  yy_destructor(yypParser,34,&yymsp[-3].minor);
#line 134 "arcangel-parser.y"
{
    progPop(ip);
    progPrintStr(ip);
}
#line 1442 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 7: /* stmt ::= FREE LPAR ID RPAR */
{  yy_destructor(yypParser,35,&yymsp[-3].minor);
#line 139 "arcangel-parser.y"
{
    entryMacro(yymsp[-1].minor.yy0)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progOp(Operation_FREE, ip);
    }
}
#line 1461 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 8: /* stmt ::= IF expr ifStart stmts else END */
{  yy_destructor(yypParser,36,&yymsp[-5].minor);
#line 151 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progEndIf(lbl, ip);
}
#line 1473 "arcangel-parser.c"
  yy_destructor(yypParser,28,&yymsp[0].minor);
}
        break;
      case 9: /* ifStart ::= THEN */
{  yy_destructor(yypParser,37,&yymsp[0].minor);
#line 155 "arcangel-parser.y"
{
    int lbl = labelCount;
    progIfStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
}
#line 1486 "arcangel-parser.c"
}
        break;
      case 10: /* elseStart ::= ELSE */
{  yy_destructor(yypParser,23,&yymsp[0].minor);
#line 163 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progElse(lbl, lbl + 1, ip);
    arcangel_Stack_push(labelStack, (void*)(lbl + 1));
}
#line 1497 "arcangel-parser.c"
}
        break;
      case 11: /* stmt ::= whileStart expr whileCond stmts END */
#line 170 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progWhileEnd(lbl, lbl + 1, ip);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
#line 1508 "arcangel-parser.c"
  yy_destructor(yypParser,28,&yymsp[0].minor);
        break;
      case 12: /* whileStart ::= WHILE */
{  yy_destructor(yypParser,38,&yymsp[0].minor);
#line 176 "arcangel-parser.y"
{
    int lbl = labelCount;
    progWhileStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    arcangel_Stack_push(loopStartStack, (void*)(lbl));
    arcangel_Stack_push(loopEndStack, (void*)(lbl + 1));
}
#line 1522 "arcangel-parser.c"
}
        break;
      case 13: /* whileCond ::= DO */
{  yy_destructor(yypParser,39,&yymsp[0].minor);
#line 184 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_top(labelStack);
    progWhileCond(lbl + 1, ip);
}
#line 1532 "arcangel-parser.c"
}
        break;
      case 14: /* stmt ::= repeatStart stmts UNTIL expr */
#line 189 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progRepeatEnd(lbl, lbl + 1, ip);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
#line 1543 "arcangel-parser.c"
  yy_destructor(yypParser,40,&yymsp[-1].minor);
        break;
      case 15: /* repeatStart ::= REPEAT */
{  yy_destructor(yypParser,41,&yymsp[0].minor);
#line 195 "arcangel-parser.y"
{
    int lbl = labelCount;
    progRepeatStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    arcangel_Stack_push(loopStartStack, (void*)(lbl));
    arcangel_Stack_push(loopEndStack, (void*)(lbl + 1));
}
#line 1557 "arcangel-parser.c"
}
        break;
      case 16: /* stmt ::= BREAK */
{  yy_destructor(yypParser,42,&yymsp[0].minor);
#line 204 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_top(loopEndStack);

    if (lbl == -1) {
        printf("break must be inside a loop\n");
        gError = 1;
    } else {
        progGoto(lbl, ip);
    }
}
#line 1573 "arcangel-parser.c"
}
        break;
      case 17: /* stmt ::= CONTINUE */
{  yy_destructor(yypParser,43,&yymsp[0].minor);
#line 215 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_top(loopStartStack);

    if (lbl == -1) {
        printf("continue must be inside a loop\n");
        gError = 1;
    } else {
        progGoto(lbl, ip);
    }
}
#line 1589 "arcangel-parser.c"
}
        break;
      case 18: /* stmt ::= RETURN expr */
{  yy_destructor(yypParser,44,&yymsp[-1].minor);
#line 226 "arcangel-parser.y"
{
    progRet(ip);
}
#line 1598 "arcangel-parser.c"
}
        break;
      case 19: /* par ::= idDecl */
#line 234 "arcangel-parser.y"
{
    parsSize++;
}
#line 1606 "arcangel-parser.c"
        break;
      case 20: /* fn ::= fnStart LPAR parsList RPAR decls stmts END */
#line 238 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progFnEnd(lbl + 1, ip);
    progConstInt(lbl, ip);
    progPush(ip);
    Entries* entries = arcangel_Stack_pop(entriesStack);
    free(entries);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
#line 1620 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-5].minor);
  yy_destructor(yypParser,27,&yymsp[-3].minor);
  yy_destructor(yypParser,28,&yymsp[0].minor);
        break;
      case 21: /* fnStart ::= fnStartKw */
#line 248 "arcangel-parser.y"
{
    int lbl = labelCount;
    progFnStart(lbl, lbl + 1, 0, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    if (isCallback) {
        progOp(Operation_Load_CALLBACK, ip);
    }
    Entries* entries = calloc(1, sizeof(Entries));
    arcangel_Stack_push(entriesStack, entries);
    arcangel_Stack_push(loopStartStack, (void*)-1);
    arcangel_Stack_push(loopEndStack, (void*)-1);
    parsSize = 0;
}
#line 1641 "arcangel-parser.c"
        break;
      case 22: /* fnStartKw ::= FUNCTION */
{  yy_destructor(yypParser,45,&yymsp[0].minor);
#line 263 "arcangel-parser.y"
{
    isCallback = 0;
}
#line 1649 "arcangel-parser.c"
}
        break;
      case 23: /* fnStartKw ::= CALLBACK */
{  yy_destructor(yypParser,46,&yymsp[0].minor);
#line 266 "arcangel-parser.y"
{
    isCallback = 1;
}
#line 1658 "arcangel-parser.c"
}
        break;
      case 24: /* asgn ::= ID ASGN expr */
#line 270 "arcangel-parser.y"
{
    entryMacro(yymsp[-2].minor.yy0)
    if (found) {
        progPop(ip);
        if (isGlobal) {
            progGSet(addr, ip);
        } else {
            progSet(addr, ip);
        }
    }
}
#line 1674 "arcangel-parser.c"
  yy_destructor(yypParser,47,&yymsp[-1].minor);
        break;
      case 25: /* asgn ::= ID LBRK expr RBRK ASGN expr */
#line 282 "arcangel-parser.y"
{
    entryMacro(yymsp[-5].minor.yy0)
    if (found) {
        progPop(ip);
        progPopB(ip);
        progPush(ip);
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progOp(Operation_POINT, ip);
        progPopB(ip);
        progRSet(ip);
    }
}
#line 1695 "arcangel-parser.c"
  yy_destructor(yypParser,48,&yymsp[-4].minor);
  yy_destructor(yypParser,49,&yymsp[-2].minor);
  yy_destructor(yypParser,47,&yymsp[-1].minor);
        break;
      case 26: /* stmt ::= callStart ID LPAR argsList RPAR */
#line 299 "arcangel-parser.y"
{
    entryMacro(yymsp[-3].minor.yy0)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }

        progCall(ip);
        progPop(ip);
        int i = 0;
        for (i = 0; i < argsSize; ++i) {
            progPopB(ip);
        }
    }
}
#line 1719 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
        break;
      case 27: /* callStart ::= CALL */
{  yy_destructor(yypParser,50,&yymsp[0].minor);
#line 316 "arcangel-parser.y"
{
    argsSize = 0;
}
#line 1729 "arcangel-parser.c"
}
        break;
      case 28: /* arg ::= expr */
#line 324 "arcangel-parser.y"
{
    ++argsSize;
}
#line 1737 "arcangel-parser.c"
        break;
      case 29: /* expr ::= expr OR expr */
#line 328 "arcangel-parser.y"
{ opMacro(Operation_OR) }
#line 1742 "arcangel-parser.c"
  yy_destructor(yypParser,1,&yymsp[-1].minor);
        break;
      case 30: /* expr ::= expr AND expr */
#line 329 "arcangel-parser.y"
{ opMacro(Operation_AND) }
#line 1748 "arcangel-parser.c"
  yy_destructor(yypParser,2,&yymsp[-1].minor);
        break;
      case 31: /* expr ::= expr BOR expr */
#line 330 "arcangel-parser.y"
{ opMacro(Operation_BOR) }
#line 1754 "arcangel-parser.c"
  yy_destructor(yypParser,3,&yymsp[-1].minor);
        break;
      case 32: /* expr ::= expr BXOR expr */
#line 331 "arcangel-parser.y"
{ opMacro(Operation_BXOR) }
#line 1760 "arcangel-parser.c"
  yy_destructor(yypParser,4,&yymsp[-1].minor);
        break;
      case 33: /* expr ::= expr BAND expr */
#line 332 "arcangel-parser.y"
{ opMacro(Operation_BAND) }
#line 1766 "arcangel-parser.c"
  yy_destructor(yypParser,5,&yymsp[-1].minor);
        break;
      case 34: /* expr ::= expr NEQ expr */
#line 333 "arcangel-parser.y"
{ opMacro(Operation_NEQ) }
#line 1772 "arcangel-parser.c"
  yy_destructor(yypParser,6,&yymsp[-1].minor);
        break;
      case 35: /* expr ::= expr EQ expr */
#line 334 "arcangel-parser.y"
{ opMacro(Operation_EQ) }
#line 1778 "arcangel-parser.c"
  yy_destructor(yypParser,7,&yymsp[-1].minor);
        break;
      case 36: /* expr ::= expr GT expr */
#line 335 "arcangel-parser.y"
{ opMacro(Operation_GT) }
#line 1784 "arcangel-parser.c"
  yy_destructor(yypParser,8,&yymsp[-1].minor);
        break;
      case 37: /* expr ::= expr GTE expr */
#line 336 "arcangel-parser.y"
{ opMacro(Operation_GTE) }
#line 1790 "arcangel-parser.c"
  yy_destructor(yypParser,9,&yymsp[-1].minor);
        break;
      case 38: /* expr ::= expr LT expr */
#line 337 "arcangel-parser.y"
{ opMacro(Operation_LT) }
#line 1796 "arcangel-parser.c"
  yy_destructor(yypParser,10,&yymsp[-1].minor);
        break;
      case 39: /* expr ::= expr LTE expr */
#line 338 "arcangel-parser.y"
{ opMacro(Operation_LTE) }
#line 1802 "arcangel-parser.c"
  yy_destructor(yypParser,11,&yymsp[-1].minor);
        break;
      case 40: /* expr ::= expr SR expr */
#line 339 "arcangel-parser.y"
{ opMacro(Operation_SR) }
#line 1808 "arcangel-parser.c"
  yy_destructor(yypParser,12,&yymsp[-1].minor);
        break;
      case 41: /* expr ::= expr SL expr */
#line 340 "arcangel-parser.y"
{ opMacro(Operation_SL) }
#line 1814 "arcangel-parser.c"
  yy_destructor(yypParser,13,&yymsp[-1].minor);
        break;
      case 42: /* expr ::= expr SUB expr */
#line 341 "arcangel-parser.y"
{ opMacro(Operation_SUB) }
#line 1820 "arcangel-parser.c"
  yy_destructor(yypParser,14,&yymsp[-1].minor);
        break;
      case 43: /* expr ::= expr SUM expr */
#line 342 "arcangel-parser.y"
{ opMacro(Operation_SUM) }
#line 1826 "arcangel-parser.c"
  yy_destructor(yypParser,15,&yymsp[-1].minor);
        break;
      case 44: /* expr ::= expr MOD expr */
#line 343 "arcangel-parser.y"
{ opMacro(Operation_MOD) }
#line 1832 "arcangel-parser.c"
  yy_destructor(yypParser,16,&yymsp[-1].minor);
        break;
      case 45: /* expr ::= expr MULT expr */
#line 344 "arcangel-parser.y"
{ opMacro(Operation_MULT) }
#line 1838 "arcangel-parser.c"
  yy_destructor(yypParser,18,&yymsp[-1].minor);
        break;
      case 46: /* expr ::= expr DIV expr */
#line 345 "arcangel-parser.y"
{ opMacro(Operation_DIV) }
#line 1844 "arcangel-parser.c"
  yy_destructor(yypParser,17,&yymsp[-1].minor);
        break;
      case 47: /* expr ::= INC expr */
{  yy_destructor(yypParser,19,&yymsp[-1].minor);
#line 346 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_INC) }
#line 1851 "arcangel-parser.c"
}
        break;
      case 48: /* expr ::= DEC expr */
{  yy_destructor(yypParser,20,&yymsp[-1].minor);
#line 347 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_DEC) }
#line 1858 "arcangel-parser.c"
}
        break;
      case 49: /* expr ::= NOT expr */
{  yy_destructor(yypParser,21,&yymsp[-1].minor);
#line 348 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_NOT) }
#line 1865 "arcangel-parser.c"
}
        break;
      case 50: /* expr ::= BNOT expr */
{  yy_destructor(yypParser,22,&yymsp[-1].minor);
#line 349 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_BNOT) }
#line 1872 "arcangel-parser.c"
}
        break;
      case 51: /* expr ::= INTCAST LPAR expr RPAR */
{  yy_destructor(yypParser,24,&yymsp[-3].minor);
#line 350 "arcangel-parser.y"
{ uOpMacro(Operation_INT) }
#line 1879 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 52: /* expr ::= FLOATCAST LPAR expr RPAR */
{  yy_destructor(yypParser,25,&yymsp[-3].minor);
#line 351 "arcangel-parser.y"
{ uOpMacro(Operation_FLOAT) }
#line 1888 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 53: /* expr ::= NEW LPAR expr RPAR */
{  yy_destructor(yypParser,51,&yymsp[-3].minor);
#line 352 "arcangel-parser.y"
{
    progPop(ip);
    progOp(Operation_ALLOC, ip);
    progPush(ip);
}
#line 1901 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 54: /* expr ::= SUB LPAR expr RPAR */
{  yy_destructor(yypParser,14,&yymsp[-3].minor);
#line 359 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_NEG) }
#line 1910 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 55: /* expr ::= ID lParStart argsList RPAR */
#line 362 "arcangel-parser.y"
{
    entryMacro(yymsp[-3].minor.yy0)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progCall(ip);
        progPop(ip);
        int i = 0;
        for (i = 0; i < argsSize; ++i) {
            progPopB(ip);
        }
        progPush(ip);
    }
}
#line 1934 "arcangel-parser.c"
  yy_destructor(yypParser,27,&yymsp[0].minor);
        break;
      case 56: /* lParStart ::= LPAR */
{  yy_destructor(yypParser,26,&yymsp[0].minor);
#line 379 "arcangel-parser.y"
{
    argsSize = 0;
}
#line 1943 "arcangel-parser.c"
}
        break;
      case 57: /* expr ::= ID LBRK expr RBRK */
#line 383 "arcangel-parser.y"
{
    entryMacro(yymsp[-3].minor.yy0)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progPopB(ip);
        progOp(Operation_POINT, ip);
        progRGet(ip);
        progPush(ip);
    }
}
#line 1962 "arcangel-parser.c"
  yy_destructor(yypParser,48,&yymsp[-2].minor);
  yy_destructor(yypParser,49,&yymsp[0].minor);
        break;
      case 58: /* expr ::= ID */
#line 397 "arcangel-parser.y"
{
    entryMacro(yymsp[0].minor.yy0)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progPush(ip);
    }
}
#line 1979 "arcangel-parser.c"
        break;
      case 59: /* expr ::= STR */
{  yy_destructor(yypParser,52,&yymsp[0].minor);
#line 409 "arcangel-parser.y"
{
    progStr(currStr, ip);
    progPush(ip);
    free(currStr);
}
#line 1989 "arcangel-parser.c"
}
        break;
      case 60: /* number ::= INTEGER */
{  yy_destructor(yypParser,53,&yymsp[0].minor);
#line 415 "arcangel-parser.y"
{
    progConstInt(tkn.value, ip);
    progPush(ip);
}
#line 1999 "arcangel-parser.c"
}
        break;
      case 61: /* number ::= SUB INTEGER */
{  yy_destructor(yypParser,14,&yymsp[-1].minor);
#line 420 "arcangel-parser.y"
{
    progConstInt(-tkn.value, ip);
    progPush(ip);
}
#line 2009 "arcangel-parser.c"
  yy_destructor(yypParser,53,&yymsp[0].minor);
}
        break;
      case 62: /* number ::= FLOAT */
{  yy_destructor(yypParser,54,&yymsp[0].minor);
#line 425 "arcangel-parser.y"
{
    progConstFloat(tkn.fValue, ip);
    progPush(ip);
}
#line 2020 "arcangel-parser.c"
}
        break;
      case 63: /* number ::= SUB FLOAT */
{  yy_destructor(yypParser,14,&yymsp[-1].minor);
#line 430 "arcangel-parser.y"
{
    progConstFloat(-tkn.fValue, ip);
    progPush(ip);
}
#line 2030 "arcangel-parser.c"
  yy_destructor(yypParser,54,&yymsp[0].minor);
}
        break;
      case 64: /* declTail ::= COMMA idDecl declTail */
      case 74: /* parsListTail ::= COMMA par parsListTail */ yytestcase(yyruleno==74);
      case 78: /* argsListTail ::= COMMA arg argsListTail */ yytestcase(yyruleno==78);
{  yy_destructor(yypParser,31,&yymsp[-2].minor);
#line 113 "arcangel-parser.y"
{
}
#line 2041 "arcangel-parser.c"
}
        break;
      case 79: /* expr ::= LPAR expr RPAR */
{  yy_destructor(yypParser,26,&yymsp[-2].minor);
#line 358 "arcangel-parser.y"
{
}
#line 2049 "arcangel-parser.c"
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      default:
      /* (65) declTail ::= */ yytestcase(yyruleno==65);
      /* (66) stmts ::= stmt stmts */ yytestcase(yyruleno==66);
      /* (67) stmts ::= */ yytestcase(yyruleno==67);
      /* (68) stmt ::= asgn (OPTIMIZED OUT) */ assert(yyruleno!=68);
      /* (69) else ::= elseStart stmts */ yytestcase(yyruleno==69);
      /* (70) else ::= */ yytestcase(yyruleno==70);
      /* (71) parsList ::= */ yytestcase(yyruleno==71);
      /* (72) parsList ::= par parsListTail */ yytestcase(yyruleno==72);
      /* (73) parsListTail ::= */ yytestcase(yyruleno==73);
      /* (75) argsList ::= */ yytestcase(yyruleno==75);
      /* (76) argsList ::= arg argsListTail */ yytestcase(yyruleno==76);
      /* (77) argsListTail ::= */ yytestcase(yyruleno==77);
      /* (80) expr ::= number (OPTIMIZED OUT) */ assert(yyruleno!=80);
      /* (81) expr ::= fn (OPTIMIZED OUT) */ assert(yyruleno!=81);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 75 "arcangel-parser.y"

    printf("Syntax error!\n\n\n");
    gError = 1;
#line 2127 "arcangel-parser.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_acceptt(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 72 "arcangel-parser.y"

#line 2153 "arcangel-parser.c"
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    int stateno = yypParser->yytos->stateno;
    if( stateno < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact >= YY_MIN_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,yyminor);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_acceptt(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
