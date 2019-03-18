#ifndef ARCANGEL_VM_H
#define ARCANGEL_VM_H

/* Operations on Arcangel VM.
   Arcangel VM has 2 registers (A and B), a label hash table (for addresses) and 3 stacks:
   - value stack (AKA stack, for arithmetic)
   - call stack (with return pointers)
   - heap stack (saves the heaps of the call stack) */
typedef enum Operation {
    /* stack.push(A) */
    Operation_PUSH,
    /* A = stack.pop() */
    Operation_POP,
    /* B = stack.pop() */
    Operation_POPB,
    /* heap = new Heap() */
    /* heapStack.push(heap) */
    Operation_HEAP,
    /* A = A op B. */
    Operation_SUM,
    Operation_SUB,
    Operation_MULT,
    Operation_DIV,
    Operation_MOD,
    Operation_EQ,
    Operation_NEQ,
    Operation_GT,
    Operation_LT,
    Operation_GTE,
    Operation_LTE,
    Operation_AND,
    Operation_OR,
    /* A = A op B (bitwise) */
    Operation_BAND,
    Operation_BOR,
    Operation_BXOR,
    Operation_SL,
    Operation_SR,

    /* A = op A */
    Operation_Unary_NEG,
    Operation_Unary_INC,
    Operation_Unary_DEC,
    Operation_Unary_NOT,
    Operation_Unary_BNOT,

    /* print(A) */
    Operation_Void_PRINT,
    /* A = x */
    Operation_Void_CONST,
    /* B = x */
    Operation_Void_CONSTB,

    /* heap[i] = A */
    Operation_SET,
    /* A = heap[i] */
    Operation_GET,
    /* B = heap[i] */
    Operation_GETB,
    /* A = stack.get(i) */
    Operation_SGET,

    /* IP = address.get(i) (i is a label) */
    Operation_Flow_GOTO,
    /* if (A == 0) IP = address.get(i) (i is a label) */
    Operation_Flow_JMPZ,
    /* callStack.push(IP) */
    /* IP = address.get(A) */
    Operation_Flow_CALL,
    /* heapStack.pop() */
    /* heap = heapStack.top() */
    /* if (callStack.empty()) halt() else IP = callStack.pop() */
    Operation_Flow_RET,
    /* halt() */
    Operation_Flow_HALT,

    /* address.set(i, IP) (loading phase IP) */
    Operation_Load_LBL,
    /* halt() (loading phase) */
    Operation_Load_END,

    /* A = new[A] */
    Operation_ALLOC,
    /* free(A) */
    Operation_FREE,
    /* A = A + (int)B (A is pointer) */
    Operation_POINT,
    /* A = *A */
    Operation_RGET,
    /* *A = B */
    Operation_RSET,
    /* A = string(i) */
    Operation_STR,
    /* prints(A as string) */
    Operation_PRINTS,
    /* if syscall syscall() */
    Operation_SYSCALL,

    /* callback = IP (loading phase) */
    Operation_Load_CALLBACK,

    /* globalHeap = heapStack.get(0) */
    /* A = globalHeap[i] */
    Operation_GGET,
    /* globalHeap = heapStack.get(0) */
    /* globalHeap[i] = A */
    Operation_GSET,
    /* A = (int)A */
    Operation_INT,
    /* A = (float)A */
    Operation_FLOAT
} Operation;

typedef enum ReferenceType {
    ReferenceType_INTEGER,
    ReferenceType_FLOAT,
    ReferenceType_ARRAY
} ReferenceType;

void arcangel_setPrintf(int (*f)(const char*, ...));
void arcangel_setCalloc(void* (*f)(first, second));
void arcangel_setFree(void (*f)(void*));
typedef struct arcangel_Reference arcangel_Reference;
typedef struct arcangel_Stack arcangel_Stack;
arcangel_Stack* arcangel_Stack_new();
void arcangel_Stack_push(arcangel_Stack* obj, void* value);
void* arcangel_Stack_pop(arcangel_Stack* obj);
void* arcangel_Stack_top(arcangel_Stack* obj);
void* arcangel_Stack_bottom(arcangel_Stack* obj);
void* arcangel_Stack_get(arcangel_Stack* obj, int n);
int arcangel_Stack_hasItems(arcangel_Stack* obj);
void arcangel_Stack_delete(arcangel_Stack* obj);
void arcangel_setSysCall(void (*f)(arcangel_Stack*));
typedef struct arcangel_State arcangel_State;
arcangel_State* arcangel_State_load(char* bytecode);
void arcangel_State_delete(arcangel_State* obj);
arcangel_Reference* arcangel_State_callback(arcangel_State* obj);
int arcangel_State_hasCallback(arcangel_State* obj);
int arcangel_toInt(arcangel_Stack* stack, int arg);
float arcangel_toFloat(arcangel_Stack* stack, int arg);
char* arcangel_toString(arcangel_Stack* stack, int arg);

#endif
