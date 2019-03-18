#ifndef ARCANGEL_COMPILER_H
#define ARCANGEL_COMPILER_H

#include <stdlib.h>
#include "arcangel-vm.h"

typedef struct Token {
    int type;
    int value;
    float fValue;
    char str[150];
    int empty;
} Token;

typedef struct Entry {
    char name[150];
} Entry;

typedef struct Entries {
    Entry entries[256];
    int entriesSize;
} Entries;

void token_destructor(Token t);
void *ParseAlloc(void *(*mallocProc)(size_t));

void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  Token yyminor       /* The value for the token */
                 /* Optional %extra_argument parameter */
);
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
);

void progEnd(char** ip);
void progHeap(int size, char** ip);
int addEntry(char* str);
void progPop(char** ip);
void progPrint(char** ip);
int findGlobalEntry(char* str);
void progGGet(int addr, char** ip);
void progGet(int addr, char** ip);
void progSGet(int addr, char** ip);
void progOp(Operation op, char** programPtr);
void progEndIf(int lbl, char** ip);
void progIfStart(int elseLbl, char** ip);
void progElse(int lbl, int endLbl, char** ip);
void progWhileEnd(int startLbl, int endLbl, char** ip);
void progWhileStart(int startLbl, char** ip);
void progWhileCond(int endLbl, char** ip);
void progRepeatEnd(int startLbl, int endLbl, char** ip);
void progRepeatStart(int startLbl, char** ip);
void progGoto(int lbl, char** ip);
void progFnEnd(int endLbl, char** ip);
void progRet(char** ip);
void progConstInt(int value, char** ip);
void progPush(char** ip);
void progFnStart(int lbl, int endLbl, int argsSize, char** ip);
void progGSet(int addr, char** ip);
void progSet(int addr, char** ip);
void progPopB(char** ip);
void progRSet(char** ip);
void progCall(char** ip);
void progRGet(char** ip);
void progConstFloat(float value, char** ip);
int findEntry(char* str);
void progStr(char* str, char** ip);
void progPrintStr(char** ip);

extern char** ip;
extern Token tkn;
extern int gError;
extern arcangel_Stack* entriesStack;
extern int labelCount;
extern arcangel_Stack* labelStack;
extern arcangel_Stack* loopStartStack;
extern arcangel_Stack* loopEndStack;
extern int isCallback;
extern char* currStr;
extern int parsSize;
extern int argsSize;

char* arcangel_compile(char* source, int* size);

#endif
