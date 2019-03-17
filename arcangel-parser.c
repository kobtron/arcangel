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
#define YYNOCODE 83
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
#define YYNSTATE             118
#define YYNRULE              81
#define YYNTOKEN             54
#define YY_MAX_SHIFT         117
#define YY_MIN_SHIFTREDUCE   168
#define YY_MAX_SHIFTREDUCE   248
#define YY_ERROR_ACTION      249
#define YY_ACCEPT_ACTION     250
#define YY_NO_ACTION         251
#define YY_MIN_REDUCE        252
#define YY_MAX_REDUCE        332
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
#define YY_ACTTAB_COUNT (906)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     5,   45,   43,   42,   41,   40,   39,   38,   37,   36,
 /*    10 */    35,   34,   33,   32,   31,   30,   29,   27,   28,   45,
 /*    20 */    43,   42,   41,   40,   39,   38,   37,   36,   35,   34,
 /*    30 */    33,   32,   31,   30,   29,   27,   28,  250,  117,  181,
 /*    40 */    41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
 /*    50 */    31,   30,   29,   27,   28,  177,  169,   45,   43,   42,
 /*    60 */    41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
 /*    70 */    31,   30,   29,   27,   28,   45,   43,   42,   41,   40,
 /*    80 */    39,   38,   37,   36,   35,   34,   33,   32,   31,   30,
 /*    90 */    29,   27,   28,   37,   36,   35,   34,   33,   32,   31,
 /*   100 */    30,   29,   27,   28,   98,   36,   35,   34,   33,   32,
 /*   110 */    31,   30,   29,   27,   28,   33,   32,   31,   30,   29,
 /*   120 */    27,   28,  224,  172,   45,   43,   42,   41,   40,   39,
 /*   130 */    38,   37,   36,   35,   34,   33,   32,   31,   30,   29,
 /*   140 */    27,   28,   34,   33,   32,   31,   30,   29,   27,   28,
 /*   150 */   221,  178,   45,   43,   42,   41,   40,   39,   38,   37,
 /*   160 */    36,   35,   34,   33,   32,   31,   30,   29,   27,   28,
 /*   170 */    32,   31,   30,   29,   27,   28,  101,    6,  246,    7,
 /*   180 */    45,   43,   42,   41,   40,   39,   38,   37,   36,   35,
 /*   190 */    34,   33,   32,   31,   30,   29,   27,   28,   31,   30,
 /*   200 */    29,   27,   28,   29,   27,   28,  220,    2,   45,   43,
 /*   210 */    42,   41,   40,   39,   38,   37,   36,   35,   34,   33,
 /*   220 */    32,   31,   30,   29,   27,   28,   30,   29,   27,   28,
 /*   230 */     3,   13,   12,  329,  219,  327,   45,   43,   42,   41,
 /*   240 */    40,   39,   38,   37,   36,   35,   34,   33,   32,   31,
 /*   250 */    30,   29,   27,   28,   56,  315,   18,   27,   28,   55,
 /*   260 */     8,  255,  218,  223,   45,   43,   42,   41,   40,   39,
 /*   270 */    38,   37,   36,   35,   34,   33,   32,   31,   30,   29,
 /*   280 */    27,   28,  228,  230,   17,   57,   58,  325,   49,  323,
 /*   290 */   174,   10,   45,   43,   42,   41,   40,   39,   38,   37,
 /*   300 */    36,   35,   34,   33,   32,   31,   30,   29,   27,   28,
 /*   310 */   187,  193,    1,   96,   11,   14,  179,  176,  173,  175,
 /*   320 */    45,   43,   42,   41,   40,   39,   38,   37,   36,   35,
 /*   330 */    34,   33,   32,   31,   30,   29,   27,   28,   43,   42,
 /*   340 */    41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
 /*   350 */    31,   30,   29,   27,   28,   42,   41,   40,   39,   38,
 /*   360 */    37,   36,   35,   34,   33,   32,   31,   30,   29,   27,
 /*   370 */    28,   90,  102,  103,   28,  222,   26,   25,   24,   23,
 /*   380 */   270,  108,  107,   19,   20,   21,   22,  270,   59,   52,
 /*   390 */    44,  111,   61,   47,   92,  113,  110,  104,   16,   60,
 /*   400 */   180,  189,  190,  183,  184,  185,   46,  106,  226,  227,
 /*   410 */   229,  194,  252,   40,   39,   38,   37,   36,   35,   34,
 /*   420 */    33,   32,   31,   30,   29,   27,   28,   39,   38,   37,
 /*   430 */    36,   35,   34,   33,   32,   31,   30,   29,   27,   28,
 /*   440 */    38,   37,   36,   35,   34,   33,   32,   31,   30,   29,
 /*   450 */    27,   28,   35,   34,   33,   32,   31,   30,   29,   27,
 /*   460 */    28,   75,  114,   62,  116,  251,  251,  251,  251,   75,
 /*   470 */   251,   75,  112,  272,  251,   95,   54,   75,   75,   75,
 /*   480 */   112,  272,  251,  105,   54,  251,   75,   75,  112,  272,
 /*   490 */   251,  251,   53,   99,   75,  251,    9,    9,  251,  251,
 /*   500 */   251,  251,   15,  100,    4,  251,    9,    9,  251,  251,
 /*   510 */   251,   97,   15,  320,    4,   72,    9,    9,  251,  251,
 /*   520 */   251,   97,   15,  251,    4,   72,  112,  272,  251,  251,
 /*   530 */    48,   97,   72,    9,    9,  251,  251,  251,  251,   15,
 /*   540 */    94,    4,  251,    9,    9,  251,  251,  251,   97,   15,
 /*   550 */   251,    4,  317,  251,  251,    9,    9,  251,   97,  251,
 /*   560 */   251,   15,  115,    4,  251,    9,    9,  251,  251,  251,
 /*   570 */    97,   15,  251,    4,  251,   63,  251,  251,   73,  251,
 /*   580 */    97,  251,  251,  251,  251,   63,  112,  272,   73,  112,
 /*   590 */   272,   74,   63,  251,   50,   73,  251,  251,  251,  251,
 /*   600 */   251,   74,  112,  272,   50,  112,  272,   51,   74,  251,
 /*   610 */   251,   50,  251,  251,  251,   64,  251,   51,  112,  272,
 /*   620 */   251,  251,  251,  251,   51,   64,  112,  272,  251,   65,
 /*   630 */   251,  251,   64,  251,  251,  251,  251,   66,  251,   65,
 /*   640 */   112,  272,  251,  251,  251,  251,   65,   66,  112,  272,
 /*   650 */    67,  251,  251,   68,   66,  251,  251,  251,  251,  251,
 /*   660 */    67,  112,  272,   68,  112,  272,   69,   67,  251,  301,
 /*   670 */    68,  251,  251,  251,  251,  251,   69,  112,  272,  301,
 /*   680 */   112,  272,  300,   69,  251,  251,  301,  251,  251,  251,
 /*   690 */   299,  251,  300,  112,  272,  251,  251,  251,  251,  300,
 /*   700 */   299,  112,  272,  298,  251,  251,  251,  299,  251,  251,
 /*   710 */   251,  109,  251,  298,  112,  272,  251,  251,  251,  251,
 /*   720 */   298,  109,  112,  272,  296,  251,  251,   93,  109,  251,
 /*   730 */   251,  251,  251,  251,  296,  112,  272,   93,  112,  272,
 /*   740 */    91,  296,  251,   89,   93,  251,  251,  251,  251,  251,
 /*   750 */    91,  112,  272,   89,  112,  272,   88,   91,  251,  251,
 /*   760 */    89,  251,  251,  251,   87,  251,   88,  112,  272,  251,
 /*   770 */   251,  251,  251,   88,   87,  112,  272,   86,  251,  251,
 /*   780 */   251,   87,  251,  251,  251,   85,  251,   86,  112,  272,
 /*   790 */   251,  251,  251,  251,   86,   85,  112,  272,   84,  251,
 /*   800 */   251,   83,   85,  251,  251,  251,  251,  251,   84,  112,
 /*   810 */   272,   83,  112,  272,   82,   84,  251,   81,   83,  251,
 /*   820 */   251,  251,  251,  251,   82,  112,  272,   81,  112,  272,
 /*   830 */    80,   82,  251,  251,   81,  251,  251,  251,   79,  251,
 /*   840 */    80,  112,  272,  251,  251,  251,  251,   80,   79,  112,
 /*   850 */   272,   78,  251,  251,  251,   79,  251,  251,  251,   77,
 /*   860 */   251,   78,  112,  272,  251,  251,  251,  251,   78,   77,
 /*   870 */   112,  272,   70,  251,  251,   76,   77,  251,  251,  251,
 /*   880 */   251,  251,   70,  112,  272,   76,  112,  272,   71,   70,
 /*   890 */   251,  251,   76,  251,  251,  251,  251,  251,   71,  112,
 /*   900 */   272,  251,  251,  251,  251,   71,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    68,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*    10 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    1,
 /*    20 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*    30 */    12,   13,   14,   15,   16,   17,   18,   55,   56,   39,
 /*    40 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*    50 */    14,   15,   16,   17,   18,   37,   29,    1,    2,    3,
 /*    60 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*    70 */    14,   15,   16,   17,   18,    1,    2,    3,    4,    5,
 /*    80 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*    90 */    16,   17,   18,    8,    9,   10,   11,   12,   13,   14,
 /*   100 */    15,   16,   17,   18,   48,    9,   10,   11,   12,   13,
 /*   110 */    14,   15,   16,   17,   18,   12,   13,   14,   15,   16,
 /*   120 */    17,   18,   48,   32,    1,    2,    3,    4,    5,    6,
 /*   130 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   140 */    17,   18,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   150 */    27,   23,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   160 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   170 */    13,   14,   15,   16,   17,   18,   65,   66,   27,   64,
 /*   180 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   190 */    11,   12,   13,   14,   15,   16,   17,   18,   14,   15,
 /*   200 */    16,   17,   18,   16,   17,   18,   27,   81,    1,    2,
 /*   210 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   220 */    13,   14,   15,   16,   17,   18,   15,   16,   17,   18,
 /*   230 */    31,   46,   47,   79,   27,   79,    1,    2,    3,    4,
 /*   240 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   250 */    15,   16,   17,   18,   31,   60,   26,   17,   18,   59,
 /*   260 */    57,   60,   27,   26,    1,    2,    3,    4,    5,    6,
 /*   270 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   280 */    17,   18,   52,   53,   47,   59,   30,   72,   31,   72,
 /*   290 */    27,   57,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   300 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   310 */    28,   27,   26,   32,   46,   40,   28,   28,   27,   27,
 /*   320 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   330 */    11,   12,   13,   14,   15,   16,   17,   18,    2,    3,
 /*   340 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*   350 */    14,   15,   16,   17,   18,    3,    4,    5,    6,    7,
 /*   360 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   370 */    18,   14,   32,   26,   18,   27,   19,   20,   21,   22,
 /*   380 */    59,   24,   25,   26,   26,   26,   26,   59,   27,   32,
 /*   390 */    26,   70,   71,   26,   32,   33,   34,   35,   36,   71,
 /*   400 */    38,   44,   45,   41,   42,   43,   26,   50,   51,   52,
 /*   410 */    53,   49,    0,    5,    6,    7,    8,    9,   10,   11,
 /*   420 */    12,   13,   14,   15,   16,   17,   18,    6,    7,    8,
 /*   430 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   440 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   450 */    17,   18,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   460 */    18,   63,   28,   27,   26,   82,   82,   82,   82,   63,
 /*   470 */    82,   73,   74,   75,   82,   77,   78,   63,   80,   73,
 /*   480 */    74,   75,   82,   77,   78,   82,   80,   73,   74,   75,
 /*   490 */    82,   82,   78,   58,   80,   82,   61,   62,   82,   82,
 /*   500 */    82,   82,   67,   58,   69,   82,   61,   62,   82,   82,
 /*   510 */    82,   76,   67,   58,   69,   63,   61,   62,   82,   82,
 /*   520 */    82,   76,   67,   82,   69,   73,   74,   75,   82,   82,
 /*   530 */    58,   76,   80,   61,   62,   82,   82,   82,   82,   67,
 /*   540 */    58,   69,   82,   61,   62,   82,   82,   82,   76,   67,
 /*   550 */    82,   69,   58,   82,   82,   61,   62,   82,   76,   82,
 /*   560 */    82,   67,   58,   69,   82,   61,   62,   82,   82,   82,
 /*   570 */    76,   67,   82,   69,   82,   63,   82,   82,   63,   82,
 /*   580 */    76,   82,   82,   82,   82,   73,   74,   75,   73,   74,
 /*   590 */    75,   63,   80,   82,   63,   80,   82,   82,   82,   82,
 /*   600 */    82,   73,   74,   75,   73,   74,   75,   63,   80,   82,
 /*   610 */    82,   80,   82,   82,   82,   63,   82,   73,   74,   75,
 /*   620 */    82,   82,   82,   82,   80,   73,   74,   75,   82,   63,
 /*   630 */    82,   82,   80,   82,   82,   82,   82,   63,   82,   73,
 /*   640 */    74,   75,   82,   82,   82,   82,   80,   73,   74,   75,
 /*   650 */    63,   82,   82,   63,   80,   82,   82,   82,   82,   82,
 /*   660 */    73,   74,   75,   73,   74,   75,   63,   80,   82,   63,
 /*   670 */    80,   82,   82,   82,   82,   82,   73,   74,   75,   73,
 /*   680 */    74,   75,   63,   80,   82,   82,   80,   82,   82,   82,
 /*   690 */    63,   82,   73,   74,   75,   82,   82,   82,   82,   80,
 /*   700 */    73,   74,   75,   63,   82,   82,   82,   80,   82,   82,
 /*   710 */    82,   63,   82,   73,   74,   75,   82,   82,   82,   82,
 /*   720 */    80,   73,   74,   75,   63,   82,   82,   63,   80,   82,
 /*   730 */    82,   82,   82,   82,   73,   74,   75,   73,   74,   75,
 /*   740 */    63,   80,   82,   63,   80,   82,   82,   82,   82,   82,
 /*   750 */    73,   74,   75,   73,   74,   75,   63,   80,   82,   82,
 /*   760 */    80,   82,   82,   82,   63,   82,   73,   74,   75,   82,
 /*   770 */    82,   82,   82,   80,   73,   74,   75,   63,   82,   82,
 /*   780 */    82,   80,   82,   82,   82,   63,   82,   73,   74,   75,
 /*   790 */    82,   82,   82,   82,   80,   73,   74,   75,   63,   82,
 /*   800 */    82,   63,   80,   82,   82,   82,   82,   82,   73,   74,
 /*   810 */    75,   73,   74,   75,   63,   80,   82,   63,   80,   82,
 /*   820 */    82,   82,   82,   82,   73,   74,   75,   73,   74,   75,
 /*   830 */    63,   80,   82,   82,   80,   82,   82,   82,   63,   82,
 /*   840 */    73,   74,   75,   82,   82,   82,   82,   80,   73,   74,
 /*   850 */    75,   63,   82,   82,   82,   80,   82,   82,   82,   63,
 /*   860 */    82,   73,   74,   75,   82,   82,   82,   82,   80,   73,
 /*   870 */    74,   75,   63,   82,   82,   63,   80,   82,   82,   82,
 /*   880 */    82,   82,   73,   74,   75,   73,   74,   75,   63,   80,
 /*   890 */    82,   82,   80,   82,   82,   82,   82,   82,   73,   74,
 /*   900 */    75,   82,   82,   82,   82,   80,
};
#define YY_SHIFT_COUNT    (117)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (442)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */    27,  357,  357,  357,  362,  362,  362,  362,  362,  362,
 /*    10 */   362,  357,  357,  357,  357,  357,  357,  357,  357,  357,
 /*    20 */   357,  357,  357,  357,  357,  357,  357,  357,  357,  357,
 /*    30 */   357,  357,  357,  357,  357,  357,  357,  357,  357,  357,
 /*    40 */   357,  357,  357,  357,  357,  357,  357,   91,  128,   91,
 /*    50 */     0,   18,  237,  199,  199,  223,   91,  223,   91,  256,
 /*    60 */   257,  257,  256,   56,   74,  123,  151,  179,  207,  235,
 /*    70 */   263,  291,  319,  319,  319,  319,  336,  352,   36,  408,
 /*    80 */   421,  433,   85,   96,  442,  131,  103,  157,  184,  211,
 /*    90 */   230,  187,  185,  240,  282,  284,  286,  281,  268,  275,
 /*   100 */   288,  289,  292,  340,  347,  348,  358,  359,  360,  356,
 /*   110 */   364,  361,  367,  380,  412,  434,  436,  438,
};
#define YY_REDUCE_COUNT (62)
#define YY_REDUCE_MIN   (-68)
#define YY_REDUCE_MAX   (825)
static const short yy_reduce_ofst[] = {
 /*     0 */   -18,  398,  406,  414,  435,  445,  455,  472,  482,  494,
 /*    10 */   504,  452,  512,  515,  528,  531,  544,  552,  566,  574,
 /*    20 */   587,  590,  603,  606,  619,  627,  640,  648,  661,  664,
 /*    30 */   677,  680,  693,  701,  714,  722,  735,  738,  751,  754,
 /*    40 */   767,  775,  788,  796,  809,  812,  825,  321,  111,  328,
 /*    50 */   -68,  115,  126,  154,  156,  195,  200,  201,  226,  203,
 /*    60 */   215,  217,  234,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   249,  326,  326,  249,  318,  318,  318,  318,  318,  318,
 /*    10 */   318,  249,  249,  249,  249,  249,  249,  249,  249,  249,
 /*    20 */   249,  249,  249,  249,  249,  249,  249,  249,  249,  249,
 /*    30 */   249,  249,  249,  249,  249,  249,  249,  249,  249,  249,
 /*    40 */   249,  249,  249,  249,  249,  249,  249,  322,  321,  249,
 /*    50 */   249,  249,  309,  328,  328,  316,  249,  316,  249,  254,
 /*    60 */   324,  324,  254,  249,  249,  249,  249,  249,  249,  249,
 /*    70 */   249,  249,  276,  275,  266,  279,  280,  281,  282,  283,
 /*    80 */   284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
 /*    90 */   249,  294,  249,  295,  249,  249,  249,  249,  249,  249,
 /*   100 */   249,  249,  249,  249,  249,  249,  249,  249,  249,  297,
 /*   110 */   249,  249,  249,  249,  249,  249,  249,  249,
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
  /*   44 */ "FUNCTION",
  /*   45 */ "CALLBACK",
  /*   46 */ "ASGN",
  /*   47 */ "LBRK",
  /*   48 */ "RBRK",
  /*   49 */ "CALL",
  /*   50 */ "NEW",
  /*   51 */ "STR",
  /*   52 */ "INTEGER",
  /*   53 */ "FLOAT",
  /*   54 */ "error",
  /*   55 */ "program",
  /*   56 */ "programStart",
  /*   57 */ "decls",
  /*   58 */ "stmts",
  /*   59 */ "idDecl",
  /*   60 */ "declTail",
  /*   61 */ "stmt",
  /*   62 */ "asgn",
  /*   63 */ "expr",
  /*   64 */ "ifStart",
  /*   65 */ "else",
  /*   66 */ "elseStart",
  /*   67 */ "whileStart",
  /*   68 */ "whileCond",
  /*   69 */ "repeatStart",
  /*   70 */ "parsList",
  /*   71 */ "par",
  /*   72 */ "parsListTail",
  /*   73 */ "fn",
  /*   74 */ "fnStart",
  /*   75 */ "fnStartKw",
  /*   76 */ "callStart",
  /*   77 */ "argsList",
  /*   78 */ "arg",
  /*   79 */ "argsListTail",
  /*   80 */ "number",
  /*   81 */ "lParStart",
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
 /*  18 */ "par ::= idDecl",
 /*  19 */ "fn ::= fnStart LPAR parsList RPAR decls stmts END",
 /*  20 */ "fnStart ::= fnStartKw",
 /*  21 */ "fnStartKw ::= FUNCTION",
 /*  22 */ "fnStartKw ::= CALLBACK",
 /*  23 */ "asgn ::= ID ASGN expr",
 /*  24 */ "asgn ::= ID LBRK expr RBRK ASGN expr",
 /*  25 */ "stmt ::= callStart ID LPAR argsList RPAR",
 /*  26 */ "callStart ::= CALL",
 /*  27 */ "arg ::= expr",
 /*  28 */ "expr ::= expr OR expr",
 /*  29 */ "expr ::= expr AND expr",
 /*  30 */ "expr ::= expr BOR expr",
 /*  31 */ "expr ::= expr BXOR expr",
 /*  32 */ "expr ::= expr BAND expr",
 /*  33 */ "expr ::= expr NEQ expr",
 /*  34 */ "expr ::= expr EQ expr",
 /*  35 */ "expr ::= expr GT expr",
 /*  36 */ "expr ::= expr GTE expr",
 /*  37 */ "expr ::= expr LT expr",
 /*  38 */ "expr ::= expr LTE expr",
 /*  39 */ "expr ::= expr SR expr",
 /*  40 */ "expr ::= expr SL expr",
 /*  41 */ "expr ::= expr SUB expr",
 /*  42 */ "expr ::= expr SUM expr",
 /*  43 */ "expr ::= expr MOD expr",
 /*  44 */ "expr ::= expr MULT expr",
 /*  45 */ "expr ::= expr DIV expr",
 /*  46 */ "expr ::= INC expr",
 /*  47 */ "expr ::= DEC expr",
 /*  48 */ "expr ::= NOT expr",
 /*  49 */ "expr ::= BNOT expr",
 /*  50 */ "expr ::= INTCAST LPAR expr RPAR",
 /*  51 */ "expr ::= FLOATCAST LPAR expr RPAR",
 /*  52 */ "expr ::= NEW LPAR expr RPAR",
 /*  53 */ "expr ::= SUB LPAR expr RPAR",
 /*  54 */ "expr ::= ID lParStart argsList RPAR",
 /*  55 */ "lParStart ::= LPAR",
 /*  56 */ "expr ::= ID LBRK expr RBRK",
 /*  57 */ "expr ::= ID",
 /*  58 */ "expr ::= STR",
 /*  59 */ "number ::= INTEGER",
 /*  60 */ "number ::= SUB INTEGER",
 /*  61 */ "number ::= FLOAT",
 /*  62 */ "number ::= SUB FLOAT",
 /*  63 */ "declTail ::= COMMA idDecl declTail",
 /*  64 */ "declTail ::=",
 /*  65 */ "stmts ::= stmt stmts",
 /*  66 */ "stmts ::=",
 /*  67 */ "stmt ::= asgn",
 /*  68 */ "else ::= elseStart stmts",
 /*  69 */ "else ::=",
 /*  70 */ "parsList ::=",
 /*  71 */ "parsList ::= par parsListTail",
 /*  72 */ "parsListTail ::=",
 /*  73 */ "parsListTail ::= COMMA par parsListTail",
 /*  74 */ "argsList ::=",
 /*  75 */ "argsList ::= arg argsListTail",
 /*  76 */ "argsListTail ::=",
 /*  77 */ "argsListTail ::= COMMA arg argsListTail",
 /*  78 */ "expr ::= LPAR expr RPAR",
 /*  79 */ "expr ::= number",
 /*  80 */ "expr ::= fn",
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
    case 44: /* FUNCTION */
    case 45: /* CALLBACK */
    case 46: /* ASGN */
    case 47: /* LBRK */
    case 48: /* RBRK */
    case 49: /* CALL */
    case 50: /* NEW */
    case 51: /* STR */
    case 52: /* INTEGER */
    case 53: /* FLOAT */
{
#line 39 "arcangel-parser.y"

    token_destructor((yypminor->yy0));

#line 898 "arcangel-parser.c"
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
  {   55,   -6 }, /* (0) program ::= programStart LPAR RPAR decls stmts END */
  {   56,   -1 }, /* (1) programStart ::= PROGRAM */
  {   57,    0 }, /* (2) decls ::= */
  {   57,   -3 }, /* (3) decls ::= VAR idDecl declTail */
  {   59,   -1 }, /* (4) idDecl ::= ID */
  {   61,   -4 }, /* (5) stmt ::= PRINT LPAR expr RPAR */
  {   61,   -4 }, /* (6) stmt ::= PRINTS LPAR expr RPAR */
  {   61,   -4 }, /* (7) stmt ::= FREE LPAR ID RPAR */
  {   61,   -6 }, /* (8) stmt ::= IF expr ifStart stmts else END */
  {   64,   -1 }, /* (9) ifStart ::= THEN */
  {   66,   -1 }, /* (10) elseStart ::= ELSE */
  {   61,   -5 }, /* (11) stmt ::= whileStart expr whileCond stmts END */
  {   67,   -1 }, /* (12) whileStart ::= WHILE */
  {   68,   -1 }, /* (13) whileCond ::= DO */
  {   61,   -4 }, /* (14) stmt ::= repeatStart stmts UNTIL expr */
  {   69,   -1 }, /* (15) repeatStart ::= REPEAT */
  {   61,   -1 }, /* (16) stmt ::= BREAK */
  {   61,   -1 }, /* (17) stmt ::= CONTINUE */
  {   71,   -1 }, /* (18) par ::= idDecl */
  {   73,   -7 }, /* (19) fn ::= fnStart LPAR parsList RPAR decls stmts END */
  {   74,   -1 }, /* (20) fnStart ::= fnStartKw */
  {   75,   -1 }, /* (21) fnStartKw ::= FUNCTION */
  {   75,   -1 }, /* (22) fnStartKw ::= CALLBACK */
  {   62,   -3 }, /* (23) asgn ::= ID ASGN expr */
  {   62,   -6 }, /* (24) asgn ::= ID LBRK expr RBRK ASGN expr */
  {   61,   -5 }, /* (25) stmt ::= callStart ID LPAR argsList RPAR */
  {   76,   -1 }, /* (26) callStart ::= CALL */
  {   78,   -1 }, /* (27) arg ::= expr */
  {   63,   -3 }, /* (28) expr ::= expr OR expr */
  {   63,   -3 }, /* (29) expr ::= expr AND expr */
  {   63,   -3 }, /* (30) expr ::= expr BOR expr */
  {   63,   -3 }, /* (31) expr ::= expr BXOR expr */
  {   63,   -3 }, /* (32) expr ::= expr BAND expr */
  {   63,   -3 }, /* (33) expr ::= expr NEQ expr */
  {   63,   -3 }, /* (34) expr ::= expr EQ expr */
  {   63,   -3 }, /* (35) expr ::= expr GT expr */
  {   63,   -3 }, /* (36) expr ::= expr GTE expr */
  {   63,   -3 }, /* (37) expr ::= expr LT expr */
  {   63,   -3 }, /* (38) expr ::= expr LTE expr */
  {   63,   -3 }, /* (39) expr ::= expr SR expr */
  {   63,   -3 }, /* (40) expr ::= expr SL expr */
  {   63,   -3 }, /* (41) expr ::= expr SUB expr */
  {   63,   -3 }, /* (42) expr ::= expr SUM expr */
  {   63,   -3 }, /* (43) expr ::= expr MOD expr */
  {   63,   -3 }, /* (44) expr ::= expr MULT expr */
  {   63,   -3 }, /* (45) expr ::= expr DIV expr */
  {   63,   -2 }, /* (46) expr ::= INC expr */
  {   63,   -2 }, /* (47) expr ::= DEC expr */
  {   63,   -2 }, /* (48) expr ::= NOT expr */
  {   63,   -2 }, /* (49) expr ::= BNOT expr */
  {   63,   -4 }, /* (50) expr ::= INTCAST LPAR expr RPAR */
  {   63,   -4 }, /* (51) expr ::= FLOATCAST LPAR expr RPAR */
  {   63,   -4 }, /* (52) expr ::= NEW LPAR expr RPAR */
  {   63,   -4 }, /* (53) expr ::= SUB LPAR expr RPAR */
  {   63,   -4 }, /* (54) expr ::= ID lParStart argsList RPAR */
  {   81,   -1 }, /* (55) lParStart ::= LPAR */
  {   63,   -4 }, /* (56) expr ::= ID LBRK expr RBRK */
  {   63,   -1 }, /* (57) expr ::= ID */
  {   63,   -1 }, /* (58) expr ::= STR */
  {   80,   -1 }, /* (59) number ::= INTEGER */
  {   80,   -2 }, /* (60) number ::= SUB INTEGER */
  {   80,   -1 }, /* (61) number ::= FLOAT */
  {   80,   -2 }, /* (62) number ::= SUB FLOAT */
  {   60,   -3 }, /* (63) declTail ::= COMMA idDecl declTail */
  {   60,    0 }, /* (64) declTail ::= */
  {   58,   -2 }, /* (65) stmts ::= stmt stmts */
  {   58,    0 }, /* (66) stmts ::= */
  {   61,   -1 }, /* (67) stmt ::= asgn */
  {   65,   -2 }, /* (68) else ::= elseStart stmts */
  {   65,    0 }, /* (69) else ::= */
  {   70,    0 }, /* (70) parsList ::= */
  {   70,   -2 }, /* (71) parsList ::= par parsListTail */
  {   72,    0 }, /* (72) parsListTail ::= */
  {   72,   -3 }, /* (73) parsListTail ::= COMMA par parsListTail */
  {   77,    0 }, /* (74) argsList ::= */
  {   77,   -2 }, /* (75) argsList ::= arg argsListTail */
  {   79,    0 }, /* (76) argsListTail ::= */
  {   79,   -3 }, /* (77) argsListTail ::= COMMA arg argsListTail */
  {   63,   -3 }, /* (78) expr ::= LPAR expr RPAR */
  {   63,   -1 }, /* (79) expr ::= number */
  {   63,   -1 }, /* (80) expr ::= fn */
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
#line 1360 "arcangel-parser.c"
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
#line 1375 "arcangel-parser.c"
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
#line 1389 "arcangel-parser.c"
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
#line 1403 "arcangel-parser.c"
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
#line 1415 "arcangel-parser.c"
        break;
      case 5: /* stmt ::= PRINT LPAR expr RPAR */
{  yy_destructor(yypParser,33,&yymsp[-3].minor);
#line 129 "arcangel-parser.y"
{
    progPop(ip);
    progPrint(ip);
}
#line 1424 "arcangel-parser.c"
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
#line 1436 "arcangel-parser.c"
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
#line 1455 "arcangel-parser.c"
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
#line 1467 "arcangel-parser.c"
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
#line 1480 "arcangel-parser.c"
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
#line 1491 "arcangel-parser.c"
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
#line 1502 "arcangel-parser.c"
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
#line 1516 "arcangel-parser.c"
}
        break;
      case 13: /* whileCond ::= DO */
{  yy_destructor(yypParser,39,&yymsp[0].minor);
#line 184 "arcangel-parser.y"
{
    int lbl = (int)arcangel_Stack_top(labelStack);
    progWhileCond(lbl + 1, ip);
}
#line 1526 "arcangel-parser.c"
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
#line 1537 "arcangel-parser.c"
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
#line 1551 "arcangel-parser.c"
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
#line 1567 "arcangel-parser.c"
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
#line 1583 "arcangel-parser.c"
}
        break;
      case 18: /* par ::= idDecl */
#line 230 "arcangel-parser.y"
{
    parsSize++;
}
#line 1591 "arcangel-parser.c"
        break;
      case 19: /* fn ::= fnStart LPAR parsList RPAR decls stmts END */
#line 234 "arcangel-parser.y"
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
#line 1605 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-5].minor);
  yy_destructor(yypParser,27,&yymsp[-3].minor);
  yy_destructor(yypParser,28,&yymsp[0].minor);
        break;
      case 20: /* fnStart ::= fnStartKw */
#line 244 "arcangel-parser.y"
{
    int lbl = labelCount;
    progFnStart(lbl, lbl + 1, 0, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    if (isCallback) {
        progOp(Operation_CALLBACK, ip);
    }
    Entries* entries = calloc(1, sizeof(Entries));
    arcangel_Stack_push(entriesStack, entries);
    arcangel_Stack_push(loopStartStack, (void*)-1);
    arcangel_Stack_push(loopEndStack, (void*)-1);
    parsSize = 0;
}
#line 1626 "arcangel-parser.c"
        break;
      case 21: /* fnStartKw ::= FUNCTION */
{  yy_destructor(yypParser,44,&yymsp[0].minor);
#line 259 "arcangel-parser.y"
{
    isCallback = 0;
}
#line 1634 "arcangel-parser.c"
}
        break;
      case 22: /* fnStartKw ::= CALLBACK */
{  yy_destructor(yypParser,45,&yymsp[0].minor);
#line 262 "arcangel-parser.y"
{
    isCallback = 1;
}
#line 1643 "arcangel-parser.c"
}
        break;
      case 23: /* asgn ::= ID ASGN expr */
#line 266 "arcangel-parser.y"
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
#line 1659 "arcangel-parser.c"
  yy_destructor(yypParser,46,&yymsp[-1].minor);
        break;
      case 24: /* asgn ::= ID LBRK expr RBRK ASGN expr */
#line 278 "arcangel-parser.y"
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
#line 1680 "arcangel-parser.c"
  yy_destructor(yypParser,47,&yymsp[-4].minor);
  yy_destructor(yypParser,48,&yymsp[-2].minor);
  yy_destructor(yypParser,46,&yymsp[-1].minor);
        break;
      case 25: /* stmt ::= callStart ID LPAR argsList RPAR */
#line 295 "arcangel-parser.y"
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
#line 1704 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
        break;
      case 26: /* callStart ::= CALL */
{  yy_destructor(yypParser,49,&yymsp[0].minor);
#line 312 "arcangel-parser.y"
{
    argsSize = 0;
}
#line 1714 "arcangel-parser.c"
}
        break;
      case 27: /* arg ::= expr */
#line 320 "arcangel-parser.y"
{
    ++argsSize;
}
#line 1722 "arcangel-parser.c"
        break;
      case 28: /* expr ::= expr OR expr */
#line 324 "arcangel-parser.y"
{ opMacro(Operation_OR) }
#line 1727 "arcangel-parser.c"
  yy_destructor(yypParser,1,&yymsp[-1].minor);
        break;
      case 29: /* expr ::= expr AND expr */
#line 325 "arcangel-parser.y"
{ opMacro(Operation_AND) }
#line 1733 "arcangel-parser.c"
  yy_destructor(yypParser,2,&yymsp[-1].minor);
        break;
      case 30: /* expr ::= expr BOR expr */
#line 326 "arcangel-parser.y"
{ opMacro(Operation_BOR) }
#line 1739 "arcangel-parser.c"
  yy_destructor(yypParser,3,&yymsp[-1].minor);
        break;
      case 31: /* expr ::= expr BXOR expr */
#line 327 "arcangel-parser.y"
{ opMacro(Operation_BXOR) }
#line 1745 "arcangel-parser.c"
  yy_destructor(yypParser,4,&yymsp[-1].minor);
        break;
      case 32: /* expr ::= expr BAND expr */
#line 328 "arcangel-parser.y"
{ opMacro(Operation_BAND) }
#line 1751 "arcangel-parser.c"
  yy_destructor(yypParser,5,&yymsp[-1].minor);
        break;
      case 33: /* expr ::= expr NEQ expr */
#line 329 "arcangel-parser.y"
{ opMacro(Operation_NEQ) }
#line 1757 "arcangel-parser.c"
  yy_destructor(yypParser,6,&yymsp[-1].minor);
        break;
      case 34: /* expr ::= expr EQ expr */
#line 330 "arcangel-parser.y"
{ opMacro(Operation_EQ) }
#line 1763 "arcangel-parser.c"
  yy_destructor(yypParser,7,&yymsp[-1].minor);
        break;
      case 35: /* expr ::= expr GT expr */
#line 331 "arcangel-parser.y"
{ opMacro(Operation_GT) }
#line 1769 "arcangel-parser.c"
  yy_destructor(yypParser,8,&yymsp[-1].minor);
        break;
      case 36: /* expr ::= expr GTE expr */
#line 332 "arcangel-parser.y"
{ opMacro(Operation_GTE) }
#line 1775 "arcangel-parser.c"
  yy_destructor(yypParser,9,&yymsp[-1].minor);
        break;
      case 37: /* expr ::= expr LT expr */
#line 333 "arcangel-parser.y"
{ opMacro(Operation_LT) }
#line 1781 "arcangel-parser.c"
  yy_destructor(yypParser,10,&yymsp[-1].minor);
        break;
      case 38: /* expr ::= expr LTE expr */
#line 334 "arcangel-parser.y"
{ opMacro(Operation_LTE) }
#line 1787 "arcangel-parser.c"
  yy_destructor(yypParser,11,&yymsp[-1].minor);
        break;
      case 39: /* expr ::= expr SR expr */
#line 335 "arcangel-parser.y"
{ opMacro(Operation_SR) }
#line 1793 "arcangel-parser.c"
  yy_destructor(yypParser,12,&yymsp[-1].minor);
        break;
      case 40: /* expr ::= expr SL expr */
#line 336 "arcangel-parser.y"
{ opMacro(Operation_SL) }
#line 1799 "arcangel-parser.c"
  yy_destructor(yypParser,13,&yymsp[-1].minor);
        break;
      case 41: /* expr ::= expr SUB expr */
#line 337 "arcangel-parser.y"
{ opMacro(Operation_SUB) }
#line 1805 "arcangel-parser.c"
  yy_destructor(yypParser,14,&yymsp[-1].minor);
        break;
      case 42: /* expr ::= expr SUM expr */
#line 338 "arcangel-parser.y"
{ opMacro(Operation_SUM) }
#line 1811 "arcangel-parser.c"
  yy_destructor(yypParser,15,&yymsp[-1].minor);
        break;
      case 43: /* expr ::= expr MOD expr */
#line 339 "arcangel-parser.y"
{ opMacro(Operation_MOD) }
#line 1817 "arcangel-parser.c"
  yy_destructor(yypParser,16,&yymsp[-1].minor);
        break;
      case 44: /* expr ::= expr MULT expr */
#line 340 "arcangel-parser.y"
{ opMacro(Operation_MULT) }
#line 1823 "arcangel-parser.c"
  yy_destructor(yypParser,18,&yymsp[-1].minor);
        break;
      case 45: /* expr ::= expr DIV expr */
#line 341 "arcangel-parser.y"
{ opMacro(Operation_DIV) }
#line 1829 "arcangel-parser.c"
  yy_destructor(yypParser,17,&yymsp[-1].minor);
        break;
      case 46: /* expr ::= INC expr */
{  yy_destructor(yypParser,19,&yymsp[-1].minor);
#line 342 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_INC) }
#line 1836 "arcangel-parser.c"
}
        break;
      case 47: /* expr ::= DEC expr */
{  yy_destructor(yypParser,20,&yymsp[-1].minor);
#line 343 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_DEC) }
#line 1843 "arcangel-parser.c"
}
        break;
      case 48: /* expr ::= NOT expr */
{  yy_destructor(yypParser,21,&yymsp[-1].minor);
#line 344 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_NOT) }
#line 1850 "arcangel-parser.c"
}
        break;
      case 49: /* expr ::= BNOT expr */
{  yy_destructor(yypParser,22,&yymsp[-1].minor);
#line 345 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_BNOT) }
#line 1857 "arcangel-parser.c"
}
        break;
      case 50: /* expr ::= INTCAST LPAR expr RPAR */
{  yy_destructor(yypParser,24,&yymsp[-3].minor);
#line 346 "arcangel-parser.y"
{ uOpMacro(Operation_INT) }
#line 1864 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 51: /* expr ::= FLOATCAST LPAR expr RPAR */
{  yy_destructor(yypParser,25,&yymsp[-3].minor);
#line 347 "arcangel-parser.y"
{ uOpMacro(Operation_FLOAT) }
#line 1873 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 52: /* expr ::= NEW LPAR expr RPAR */
{  yy_destructor(yypParser,50,&yymsp[-3].minor);
#line 348 "arcangel-parser.y"
{
    progPop(ip);
    progOp(Operation_ALLOC, ip);
    progPush(ip);
}
#line 1886 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 53: /* expr ::= SUB LPAR expr RPAR */
{  yy_destructor(yypParser,14,&yymsp[-3].minor);
#line 355 "arcangel-parser.y"
{ uOpMacro(Operation_Unary_NEG) }
#line 1895 "arcangel-parser.c"
  yy_destructor(yypParser,26,&yymsp[-2].minor);
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      case 54: /* expr ::= ID lParStart argsList RPAR */
#line 358 "arcangel-parser.y"
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
#line 1919 "arcangel-parser.c"
  yy_destructor(yypParser,27,&yymsp[0].minor);
        break;
      case 55: /* lParStart ::= LPAR */
{  yy_destructor(yypParser,26,&yymsp[0].minor);
#line 375 "arcangel-parser.y"
{
    argsSize = 0;
}
#line 1928 "arcangel-parser.c"
}
        break;
      case 56: /* expr ::= ID LBRK expr RBRK */
#line 379 "arcangel-parser.y"
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
#line 1947 "arcangel-parser.c"
  yy_destructor(yypParser,47,&yymsp[-2].minor);
  yy_destructor(yypParser,48,&yymsp[0].minor);
        break;
      case 57: /* expr ::= ID */
#line 393 "arcangel-parser.y"
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
#line 1964 "arcangel-parser.c"
        break;
      case 58: /* expr ::= STR */
{  yy_destructor(yypParser,51,&yymsp[0].minor);
#line 405 "arcangel-parser.y"
{
    progStr(currStr, ip);
    progPush(ip);
    free(currStr);
}
#line 1974 "arcangel-parser.c"
}
        break;
      case 59: /* number ::= INTEGER */
{  yy_destructor(yypParser,52,&yymsp[0].minor);
#line 411 "arcangel-parser.y"
{
    progConstInt(tkn.value, ip);
    progPush(ip);
}
#line 1984 "arcangel-parser.c"
}
        break;
      case 60: /* number ::= SUB INTEGER */
{  yy_destructor(yypParser,14,&yymsp[-1].minor);
#line 416 "arcangel-parser.y"
{
    progConstInt(-tkn.value, ip);
    progPush(ip);
}
#line 1994 "arcangel-parser.c"
  yy_destructor(yypParser,52,&yymsp[0].minor);
}
        break;
      case 61: /* number ::= FLOAT */
{  yy_destructor(yypParser,53,&yymsp[0].minor);
#line 421 "arcangel-parser.y"
{
    progConstFloat(tkn.fValue, ip);
    progPush(ip);
}
#line 2005 "arcangel-parser.c"
}
        break;
      case 62: /* number ::= SUB FLOAT */
{  yy_destructor(yypParser,14,&yymsp[-1].minor);
#line 426 "arcangel-parser.y"
{
    progConstFloat(-tkn.fValue, ip);
    progPush(ip);
}
#line 2015 "arcangel-parser.c"
  yy_destructor(yypParser,53,&yymsp[0].minor);
}
        break;
      case 63: /* declTail ::= COMMA idDecl declTail */
      case 73: /* parsListTail ::= COMMA par parsListTail */ yytestcase(yyruleno==73);
      case 77: /* argsListTail ::= COMMA arg argsListTail */ yytestcase(yyruleno==77);
{  yy_destructor(yypParser,31,&yymsp[-2].minor);
#line 113 "arcangel-parser.y"
{
}
#line 2026 "arcangel-parser.c"
}
        break;
      case 78: /* expr ::= LPAR expr RPAR */
{  yy_destructor(yypParser,26,&yymsp[-2].minor);
#line 354 "arcangel-parser.y"
{
}
#line 2034 "arcangel-parser.c"
  yy_destructor(yypParser,27,&yymsp[0].minor);
}
        break;
      default:
      /* (64) declTail ::= */ yytestcase(yyruleno==64);
      /* (65) stmts ::= stmt stmts */ yytestcase(yyruleno==65);
      /* (66) stmts ::= */ yytestcase(yyruleno==66);
      /* (67) stmt ::= asgn (OPTIMIZED OUT) */ assert(yyruleno!=67);
      /* (68) else ::= elseStart stmts */ yytestcase(yyruleno==68);
      /* (69) else ::= */ yytestcase(yyruleno==69);
      /* (70) parsList ::= */ yytestcase(yyruleno==70);
      /* (71) parsList ::= par parsListTail */ yytestcase(yyruleno==71);
      /* (72) parsListTail ::= */ yytestcase(yyruleno==72);
      /* (74) argsList ::= */ yytestcase(yyruleno==74);
      /* (75) argsList ::= arg argsListTail */ yytestcase(yyruleno==75);
      /* (76) argsListTail ::= */ yytestcase(yyruleno==76);
      /* (79) expr ::= number (OPTIMIZED OUT) */ assert(yyruleno!=79);
      /* (80) expr ::= fn (OPTIMIZED OUT) */ assert(yyruleno!=80);
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
#line 2112 "arcangel-parser.c"
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

#line 2138 "arcangel-parser.c"
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
