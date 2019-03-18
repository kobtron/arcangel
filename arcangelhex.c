#include "arcangel-vm.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char* fileToBuff(char* file) {
    char *source = 0;
    if (file == 0) {
        file = "a.out";
    }
    FILE *fp = fopen(file, "rb");
    fseek(fp, 0L, SEEK_END);
    long bufsize = ftell(fp);
    source = malloc(sizeof(char) * (bufsize + 1));
    fseek(fp, 0L, SEEK_SET);
    int newLen = fread(source, sizeof(char), bufsize, fp);
    source[newLen++] = '\0';
    fclose(fp);
    return source;
}

void printHexRef(char** programPtr) {
    ReferenceType type = readRefType(programPtr);
    switch (type) {
    case ReferenceType_INTEGER:
        printf("INTEGER ");
        int intVal = readInt(programPtr);
        printf("%i\n", intVal);
        break;
    case ReferenceType_FLOAT:
        printf("FLOAT ");
        float floatVal = readFloat(programPtr);
        printf("%f\n", floatVal);
        break;
    case ReferenceType_ARRAY:
        printf("ARRAY ");
        void * arrayVal = readArray(programPtr);
        printf("%p\n", arrayVal);
        break;
    }
}

void arcangel_printHex(char* programPtr) {
    int halt = 0;
    int size;
    int addr;
    int lbl;
    char c;
    while (1) {
        if (halt) {
            break;
        }

        Operation op = readOp(&programPtr);
        switch (op) {
        case Operation_PUSH:
            printf("PUSH\n");
            break;
        case Operation_POP:
            printf("POP\n");
            break;
        case Operation_POPB:
            printf("POPB\n");
            break;
        case Operation_HEAP:
            printf("HEAP ");
            size = readInt(&programPtr);
            printf("%i\n", size);
            break;
        case Operation_Void_CONST:
            printf("CONST ");
            printHexRef(&programPtr);
            break;
        case Operation_Void_CONSTB:
            printf("CONSTB ");
            printHexRef(&programPtr);
            break;
        case Operation_SET:
            printf("SET ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_GET:
            printf("GET ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_SGET:
            printf("SGET ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_GETB:
            printf("GETB ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_GGET:
            printf("GGET ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_GSET:
            printf("GSET ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_SUM:
            printf("SUM\n");
            break;
        case Operation_SUB:
            printf("SUB\n");
            break;
        case Operation_MULT:
            printf("MULT\n");
            break;
        case Operation_DIV:
            printf("DIV\n");
            break;
        case Operation_MOD:
            printf("MOD\n");
            break;
        case Operation_EQ:
            printf("EQ\n");
            break;
        case Operation_NEQ:
            printf("NEQ\n");
            break;
        case Operation_GT:
            printf("GT\n");
            break;
        case Operation_LT:
            printf("LT\n");
            break;
        case Operation_GTE:
            printf("GTE\n");
            break;
        case Operation_LTE:
            printf("LTE\n");
            break;
        case Operation_AND:
            printf("AND\n");
            break;
        case Operation_OR:
            printf("OR\n");
            break;
        case Operation_BAND:
            printf("BAND\n");
            break;
        case Operation_BOR:
            printf("BOR\n");
            break;
        case Operation_BXOR:
            printf("BXOR\n");
            break;
        case Operation_SL:
            printf("SL\n");
            break;
        case Operation_SR:
            printf("SR\n");
            break;
        case Operation_Unary_NEG:
            printf("NEG\n");
            break;
        case Operation_Unary_INC:
            printf("INC\n");
            break;
        case Operation_Unary_DEC:
            printf("DEC\n");
            break;
        case Operation_Unary_NOT:
            printf("NOT\n");
            break;
        case Operation_Unary_BNOT:
            printf("BNOT\n");
            break;
        case Operation_INT:
            printf("INT\n");
            break;
        case Operation_FLOAT:
            printf("FLOAT\n");
            break;
        case Operation_Void_PRINT:
            printf("PRINT\n");
            break;
        case Operation_Load_LBL:
            printf("LBL ");
            addr = readInt(&programPtr);
            printf("%i\n", addr);
            break;
        case Operation_Load_END:
            printf("END\n");
            halt = 1;
            break;
        case Operation_Flow_HALT:
            printf("HALT\n");
            halt = 1;
            break;
        case Operation_Flow_GOTO:
            printf("GOTO ");
            lbl = readInt(&programPtr);
            printf("%i\n", lbl);
            break;
        case Operation_Flow_JMPZ:
            printf("JMPZ ");
            lbl = readInt(&programPtr);
            printf("%i\n", lbl);
            break;
        case Operation_Flow_CALL:
            printf("CALL\n");
            break;
        case Operation_Flow_RET:
            printf("RET\n");
            break;
        case Operation_ALLOC:
            printf("ALLOC\n");
            break;
        case Operation_FREE:
            printf("FREE\n");
            break;
        case Operation_POINT:
            printf("POINT\n");
            break;
        case Operation_RGET:
            printf("RGET\n");
            break;
        case Operation_RSET:
            printf("RSET\n");
            break;
        case Operation_STR:
            printf("STR \"");
            size = 0;
            char* strStart = programPtr;
            do {
                ++size;
                c = readChar(&programPtr);
                printf("%c", c);
            } while (c != 0);
            printf("\"\n");
            break;
        case Operation_PRINTS:
            printf("PRINTS\n");
            break;
        case Operation_SYSCALL:
            printf("SYSCALL\n");
            break;
        case Operation_Load_CALLBACK:
            printf("CALLBACK\n");
            break;
        default:
            printf("error\n");
            exit(1);
        }
    }
}

int main(int argc, char** argv) {
    char * file = 0;
    if (argc >= 2) {
        file = argv[1];
    }
    char* program = fileToBuff(file);
    arcangel_setCalloc(calloc);
    arcangel_setFree(free);
    arcangel_setPrintf(printf);
    if (program) {
        arcangel_printHex(program);
    }
    return 0;
}
