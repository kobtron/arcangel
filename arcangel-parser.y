%include {
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

}

%token_type {Token}
%default_type {Token}
%token_destructor {
    token_destructor($$);
}

//%type expr {Token}
//%type id {Token}

%left OR.
%left AND.
%left BOR.
%left BXOR.
%left BAND.
%left NEQ.
%left EQ.
%left GT.
%left GTE.
%left LT.
%left LTE.
%left SR.
%left SL.
%left SUB.
%left SUM.
%left MOD.
%left DIV.
%left MULT.
%left INC.
%left DEC.
%left NOT.
%left BNOT.
%left ELSE.
%left INTCAST.
%left FLOATCAST.

%parse_accept {
}

%syntax_error {
    printf("Syntax error!\n\n\n");
    gError = 1;
}

program ::= programStart LPAR RPAR decls stmts END. {
    progEnd(ip);
    Entries* entries = arcangel_Stack_pop(entriesStack);
    free(entries);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
programStart ::= PROGRAM. {
    Entries* entries = calloc(1, sizeof(Entries));
    arcangel_Stack_push(entriesStack, entries);
    arcangel_Stack_push(loopStartStack, (void*)-1);
    arcangel_Stack_push(loopEndStack, (void*)-1);
    parsSize = 0;
}

decls ::= . {
    Entries* entries = arcangel_Stack_top(entriesStack);
    progHeap(entries->entriesSize, ip);
    int i;
    for (i = 0; i < parsSize; ++i) {
        progSGet((parsSize - 1) - i, ip);
        progSet(i, ip);
    }
}
decls ::= VAR idDecl declTail. {
    Entries* entries = arcangel_Stack_top(entriesStack);
    progHeap(entries->entriesSize, ip);
    int i;
    for (i = 0; i < parsSize; ++i) {
        progSGet((parsSize - 1) - i, ip);
        progSet(i, ip);
    }
}
declTail ::= COMMA idDecl declTail.
declTail ::= .

idDecl ::= ID(A). {
    int found = addEntry(A.str);
    if (found) {
        printf("Repeated variable %s\n", A.str);
        gError = 1;
    }
}

stmts ::= stmt stmts.
stmts ::= .

stmt ::= asgn.

stmt ::= PRINT LPAR expr RPAR. {
    progPop(ip);
    progPrint(ip);
}

stmt ::= PRINTS LPAR expr RPAR. {
    progPop(ip);
    progPrintStr(ip);
}

stmt ::= FREE LPAR ID(A) RPAR. {
    entryMacro(A)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progOp(Operation_FREE, ip);
    }
}

stmt ::= IF expr ifStart stmts else END. {
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progEndIf(lbl, ip);
}
ifStart ::= THEN. {
    int lbl = labelCount;
    progIfStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
}

else ::= elseStart stmts.
elseStart ::= ELSE. {
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progElse(lbl, lbl + 1, ip);
    arcangel_Stack_push(labelStack, (void*)(lbl + 1));
}
else ::= .

stmt ::= whileStart expr whileCond stmts END. {
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progWhileEnd(lbl, lbl + 1, ip);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
whileStart ::= WHILE. {
    int lbl = labelCount;
    progWhileStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    arcangel_Stack_push(loopStartStack, (void*)(lbl));
    arcangel_Stack_push(loopEndStack, (void*)(lbl + 1));
}
whileCond ::= DO. {
    int lbl = (int)arcangel_Stack_top(labelStack);
    progWhileCond(lbl + 1, ip);
}

stmt ::= repeatStart stmts UNTIL expr. {
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progRepeatEnd(lbl, lbl + 1, ip);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
repeatStart ::= REPEAT. {
    int lbl = labelCount;
    progRepeatStart(lbl, ip);
    arcangel_Stack_push(labelStack, (void*)lbl);
    labelCount += 2;
    arcangel_Stack_push(loopStartStack, (void*)(lbl));
    arcangel_Stack_push(loopEndStack, (void*)(lbl + 1));
}

stmt ::= BREAK. {
    int lbl = (int)arcangel_Stack_top(loopEndStack);

    if (lbl == -1) {
        printf("break must be inside a loop\n");
        gError = 1;
    } else {
        progGoto(lbl, ip);
    }
}

stmt ::= CONTINUE. {
    int lbl = (int)arcangel_Stack_top(loopStartStack);

    if (lbl == -1) {
        printf("continue must be inside a loop\n");
        gError = 1;
    } else {
        progGoto(lbl, ip);
    }
}

stmt ::= RETURN expr. {
    progRet(ip);
}

parsList ::= .
parsList ::= par parsListTail.
parsListTail ::= .
parsListTail ::= COMMA par parsListTail.
par ::= idDecl. {
    parsSize++;
}

fn ::= fnStart LPAR parsList RPAR decls stmts END. {
    int lbl = (int)arcangel_Stack_pop(labelStack);
    progFnEnd(lbl + 1, ip);
    progConstInt(lbl, ip);
    progPush(ip);
    Entries* entries = arcangel_Stack_pop(entriesStack);
    free(entries);
    arcangel_Stack_pop(loopStartStack);
    arcangel_Stack_pop(loopEndStack);
}
fnStart ::= fnStartKw. {
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

fnStartKw ::= FUNCTION. {
    isCallback = 0;
}
fnStartKw ::= CALLBACK. {
    isCallback = 1;
}

asgn ::= ID(A) ASGN expr. {
    entryMacro(A)
    if (found) {
        progPop(ip);
        if (isGlobal) {
            progGSet(addr, ip);
        } else {
            progSet(addr, ip);
        }
    }
}

asgn ::= ID(A) LBRK expr RBRK ASGN expr. {
    entryMacro(A)
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

stmt ::= callStart ID(A) LPAR argsList RPAR. {
    entryMacro(A)
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
callStart ::= CALL. {
    argsSize = 0;
}

argsList ::= .
argsList ::= arg argsListTail.
argsListTail ::= .
argsListTail ::= COMMA arg argsListTail.
arg ::= expr. {
    ++argsSize;
}

expr ::= expr OR expr. { opMacro(Operation_OR) }
expr ::= expr AND expr. { opMacro(Operation_AND) }
expr ::= expr BOR expr. { opMacro(Operation_BOR) }
expr ::= expr BXOR expr. { opMacro(Operation_BXOR) }
expr ::= expr BAND expr. { opMacro(Operation_BAND) }
expr ::= expr NEQ expr. { opMacro(Operation_NEQ) }
expr ::= expr EQ expr. { opMacro(Operation_EQ) }
expr ::= expr GT expr. { opMacro(Operation_GT) }
expr ::= expr GTE expr. { opMacro(Operation_GTE) }
expr ::= expr LT expr. { opMacro(Operation_LT) }
expr ::= expr LTE expr. { opMacro(Operation_LTE) }
expr ::= expr SR expr. { opMacro(Operation_SR) }
expr ::= expr SL expr. { opMacro(Operation_SL) }
expr ::= expr SUB expr. { opMacro(Operation_SUB) }
expr ::= expr SUM expr. { opMacro(Operation_SUM) }
expr ::= expr MOD expr. { opMacro(Operation_MOD) }
expr ::= expr MULT expr. { opMacro(Operation_MULT) }
expr ::= expr DIV expr. { opMacro(Operation_DIV) }
expr ::= INC expr. { uOpMacro(Operation_Unary_INC) }
expr ::= DEC expr. { uOpMacro(Operation_Unary_DEC) }
expr ::= NOT expr. { uOpMacro(Operation_Unary_NOT) }
expr ::= BNOT expr. { uOpMacro(Operation_Unary_BNOT) }
expr ::= INTCAST LPAR expr RPAR. { uOpMacro(Operation_INT) }
expr ::= FLOATCAST LPAR expr RPAR. { uOpMacro(Operation_FLOAT) }
expr ::= NEW LPAR expr RPAR. {
    progPop(ip);
    progOp(Operation_ALLOC, ip);
    progPush(ip);
}

expr ::= LPAR expr RPAR.
expr ::= SUB LPAR expr RPAR. { uOpMacro(Operation_Unary_NEG) }
expr ::= number.
expr ::= fn.
expr ::= ID(A) lParStart argsList RPAR. {
    entryMacro(A)
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
lParStart ::= LPAR. {
    argsSize = 0;
}

expr ::= ID(A) LBRK expr RBRK. {
    entryMacro(A)
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
expr ::= ID(A). {
    entryMacro(A)
    if (found) {
        if (isGlobal) {
            progGGet(addr, ip);
        } else {
            progGet(addr, ip);
        }
        progPush(ip);
    }
}

expr ::= STR. {
    progStr(currStr, ip);
    progPush(ip);
    free(currStr);
}

number ::= INTEGER. {
    progConstInt(tkn.value, ip);
    progPush(ip);
}

number ::= SUB INTEGER. {
    progConstInt(-tkn.value, ip);
    progPush(ip);
}

number ::= FLOAT. {
    progConstFloat(tkn.fValue, ip);
    progPush(ip);
}

number ::= SUB FLOAT. {
    progConstFloat(-tkn.fValue, ip);
    progPush(ip);
}
