#include "arcangel-vm.h"
#include "arcangel-compiler.h"
#include <string.h>

void copyMem(void* str1, const void* str2, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        ((char*)str1)[i] = ((char*)str2)[i];
    }
}

void progOp(Operation op, char** programPtr) {
    int size = sizeof(char);
    char cOp = (char)op;
    copyMem(*programPtr, &cOp, size);
    *programPtr += size;
}

void progRefType(ReferenceType rt, char** programPtr) {
    int size = sizeof(char);
    char cRt = (char)rt;
    copyMem(*programPtr, &cRt, size);
    *programPtr += size;
}

void progInt(int i, char** programPtr) {
    int size = sizeof(int);
    copyMem(*programPtr, &i, size);
    *programPtr += size;
}

void progChar(char c, char** programPtr) {
    int size = sizeof(char);
    copyMem(*programPtr, &c, size);
    *programPtr += size;
}

void progString(char* str, char** ip) {
    do {
        progChar(*str, ip);
        str += 1;
    } while (*str != '\0');
    progChar('\0', ip);
}

void progFloat(float f, char** programPtr) {
    int size = sizeof(float);
    copyMem(*programPtr, &f, size);
    *programPtr += size;
}

void progHeap(int size, char** ip) {
    progOp(Operation_HEAP, ip);
    progInt(size, ip);
}

void progPush(char** ip) {
    progOp(Operation_PUSH, ip);
}

void progPop(char** ip) {
    progOp(Operation_POP, ip);
}

void progPopB(char** ip) {
    progOp(Operation_POPB, ip);
}

void progRGet(char** ip) {
    progOp(Operation_RGET, ip);
}

void progRSet(char** ip) {
    progOp(Operation_RSET, ip);
}

void progGoto(int lbl, char** ip) {
    progOp(Operation_Flow_GOTO, ip);
    progInt(lbl, ip);
}

void progLbl(int lbl, char** ip) {
    progOp(Operation_Load_LBL, ip);
    progInt(lbl, ip);
}

void progSet(int addr, char** ip) {
    progOp(Operation_SET, ip);
    progInt(addr, ip);
}

void progConstInt(int value, char** ip) {
    progOp(Operation_Void_CONST, ip);
    progRefType(ReferenceType_INTEGER, ip);
    progInt(value, ip);
}

void progConstBInt(int value, char** ip) {
    progOp(Operation_Void_CONSTB, ip);
    progRefType(ReferenceType_INTEGER, ip);
    progInt(value, ip);
}

void progConstFloat(float value, char** ip) {
    progOp(Operation_Void_CONST, ip);
    progRefType(ReferenceType_FLOAT, ip);
    progFloat(value, ip);
}

void progConstBFloat(float value, char** ip) {
    progOp(Operation_Void_CONSTB, ip);
    progRefType(ReferenceType_FLOAT, ip);
    progFloat(value, ip);
}

void progPrint(char** ip) {
    progOp(Operation_Void_PRINT, ip);
}

void progPrintStr(char** ip) {
    progOp(Operation_PRINTS, ip);
}

void progGet(int addr, char** ip) {
    progOp(Operation_GET, ip);
    progInt(addr, ip);
}

void progGetB(int addr, char** ip) {
    progOp(Operation_GETB, ip);
    progInt(addr, ip);
}

void progGGet(int addr, char** ip) {
    progOp(Operation_GGET, ip);
    progInt(addr, ip);
}

void progGSet(int addr, char** ip) {
    progOp(Operation_GSET, ip);
    progInt(addr, ip);
}

void progSGet(int addr, char** ip) {
    progOp(Operation_SGET, ip);
    progInt(addr, ip);
}

void progRet(char** ip) {
    progOp(Operation_Flow_RET, ip);
}

void progFnStart(int lbl, int endLbl, int argsSize, char** ip) {
    progGoto(endLbl, ip);
    progLbl(lbl, ip);
    int i;
    for (i = argsSize - 1; i >= 0; --i) {
        progPop(ip);
        progSet(i, ip);
    }
}

void progFnEnd(int endLbl, char** ip) {
    progConstInt(0, ip);
    progPush(ip);
    progRet(ip);
    progLbl(endLbl, ip);
}

void progIfStart(int elseLbl, char** ip) {
    progOp(Operation_Flow_JMPZ, ip);
    progInt(elseLbl, ip);
}

void progElse(int lbl, int endLbl, char** ip) {
    progOp(Operation_Flow_GOTO, ip);
    progInt(endLbl, ip);
    progOp(Operation_Load_LBL, ip);
    progInt(lbl, ip);
}

void progEndIf(int lbl, char** ip) {
    progOp(Operation_Load_LBL, ip);
    progInt(lbl, ip);
}

void progWhileStart(int startLbl, char** ip) {
    progLbl(startLbl, ip);
}

void progWhileCond(int endLbl, char** ip) {
    progOp(Operation_Flow_JMPZ, ip);
    progInt(endLbl, ip);
}

void progWhileEnd(int startLbl, int endLbl, char** ip) {
    progGoto(startLbl, ip);
    progLbl(endLbl, ip);
}

void progRepeatStart(int startLbl, char** ip) {
    progLbl(startLbl, ip);
}

void progRepeatEnd(int startLbl, int endLbl, char** ip) {
    progOp(Operation_Flow_JMPZ, ip);
    progInt(startLbl, ip);
    progLbl(endLbl, ip);
}

void progPushConstInt(int value, char** ip) {
    progConstInt(value, ip);
    progOp(Operation_PUSH, ip);
}

void progPushConstFloat(float value, char** ip) {
    progConstFloat(value, ip);
    progOp(Operation_PUSH, ip);
}

void progCall(char** ip) {
    progOp(Operation_Flow_CALL, ip);
}

void progPushVar(int addr, char** ip) {
    progOp(Operation_GET, ip);
    progInt(addr, ip);
    progOp(Operation_PUSH, ip);
}

void progEnd(char** ip) {
    progOp(Operation_Flow_HALT, ip);
    progOp(Operation_Load_END, ip);
}

void progAlloc(int size, char** ip) {
    progConstInt(size, ip);
    progOp(Operation_ALLOC, ip);
}

void progPoint(int addr, int index, char** ip) {
    progGet(addr, ip);
    progConstBInt(index, ip);
    progOp(Operation_POINT, ip);
}

void progStr(char* str, char** ip) {
    progOp(Operation_STR, ip);
    progString(str, ip);
}


void token_destructor(Token t)
{
}

char** ip;
Token tkn;
int gError = 0;

arcangel_Stack* entriesStack;

int addEntryG(Entry* entries, int* sizePtr, char* str);
int addEntry(char* str) {
    Entries* entries = arcangel_Stack_top(entriesStack);
    return addEntryG(entries->entries, &(entries->entriesSize), str);
}
int addEntryG(Entry* entries, int* sizePtr, char* str) {
    int entriesSize = *sizePtr;
    int i;
    int found = 0;
    for (i = 0; i < entriesSize; ++i) {
        if (strcmp(str, entries[i].name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        strcpy(entries[i].name, str);
        ++entriesSize;
    }

    *sizePtr = entriesSize;
    return found;
}

int findEntryG(Entry* entries, int* sizePtr, char* str);
int findEntry(char* str) {
    Entries* entries = arcangel_Stack_top(entriesStack);
    return findEntryG(entries->entries, &(entries->entriesSize), str);
}
int findGlobalEntry(char* str) {
    Entries* entries = arcangel_Stack_bottom(entriesStack);
    return findEntryG(entries->entries, &(entries->entriesSize), str);
}
int findEntryG(Entry* entries, int* sizePtr, char* str) {
    int entriesSize = *sizePtr;
    int i;
    int found = 0;
    for (i = 0; i < entriesSize; ++i) {
        if (strcmp(str, entries[i].name) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        return i;
    } else {
        return -1;
    }
}

int labelCount = 1;
arcangel_Stack* labelStack;
arcangel_Stack* loopStartStack;
arcangel_Stack* loopEndStack;
int isCallback = 0;
char* currStr;
int parsSize;
int argsSize;

#include "arcangel-lexer.h"
/*
#include "lex.yy.c"
*/

char* arcangel_compile(char* source) {
    char* program = calloc(4000, sizeof(char));
    char* programPtr = program;
    ip = &programPtr;
    char* sourcePtr = source;
    int yv;
    labelStack = arcangel_Stack_new();
    loopStartStack = arcangel_Stack_new();
    loopEndStack = arcangel_Stack_new();
    entriesStack = arcangel_Stack_new();
    void* pParser = ParseAlloc (malloc);
    yy_scan_string(sourcePtr);
    while( (yv=yylex()) != 0)
    {
        if (gError) {
            yyerror("Lexical");
            break;
        }
        tkn.type = yv;
        Parse (pParser, yv, tkn);
        if (gError) {
            yyerror("Syntactic");
            break;
        }
    }
    Parse (pParser, 0, tkn);
    ParseFree(pParser, free );
    arcangel_Stack_delete(labelStack);
    arcangel_Stack_delete(loopStartStack);
    arcangel_Stack_delete(loopEndStack);
    arcangel_Stack_delete(entriesStack);
    if (!gError) {
        return program;
    }
    free(program);
    return 0;
}

