#include "arcangel-vm.h"

int (*arcangel_printf)(const char*, ...) = 0;

void arcangel_setPrintf(int (*f)(const char*, ...)) {
    arcangel_printf = f;
}

void* (*arcangel_calloc)(first, second) = 0;

void arcangel_setCalloc(void* (*f)(first, second)) {
    arcangel_calloc = f;
}

void (*arcangel_free)(void*) = 0;

void arcangel_setFree(void (*f)(void*)) {
    arcangel_free = f;
}

typedef struct arcangel_Stack {
    void** stack;
    int elements;
    int size;
} Stack;

void (*arcangel_sysCall)(Stack*) = 0;

void arcangel_setSysCall(void (*f)(Stack*)) {
    arcangel_sysCall = f;
}

void Stack_init(Stack* obj) {
    obj->stack = arcangel_calloc(obj->size, sizeof(void*));
}

Stack* Stack_new() {
    Stack* obj = arcangel_calloc(1, sizeof(Stack));
    obj->elements = 0;
    obj->size = 10;
    Stack_init(obj);
    return obj;
}
Stack* arcangel_Stack_new() {
    return Stack_new();
}

void Stack_push(Stack* obj, void* value) {
    if (obj->elements == obj->size) {
        void** oldStack = obj->stack;
        int oldSize = obj->size;
        obj->size *= 2;
        Stack_init(obj);
        int i;
        for (i = 0; i < oldSize; ++i) {
            obj->stack[i] = oldStack[i];
        }
        arcangel_free(oldStack);
    }
    obj->stack[obj->elements] = value;
    obj->elements++;
}
void arcangel_Stack_push(Stack* obj, void* value) {
    Stack_push(obj, value);
}

void* Stack_pop(Stack* obj) {
    obj->elements--;
    void* r = obj->stack[obj->elements];
    return r;
}
void* arcangel_Stack_pop(Stack* obj) {
    return Stack_pop(obj);
}

void* Stack_top(Stack* obj) {
    void* r = obj->stack[obj->elements - 1];
    return r;
}
void* arcangel_Stack_top(Stack* obj) {
    return Stack_top(obj);
}

int Stack_hasItems(Stack* obj) {
    int r = obj->elements > 0;
    return r;
}
int arcangel_Stack_hasItems(Stack* obj) {
    return Stack_hasItems(obj);
}

void* Stack_bottom(Stack* obj) {
    void* r = obj->stack[0];
    return r;
}
void* arcangel_Stack_bottom(Stack* obj) {
    return Stack_bottom(obj);
}

void* Stack_get(Stack* obj, int n) {
    void* r = obj->stack[obj->elements - 1 - n];
    return r;
}
void* arcangel_Stack_get(Stack* obj, int n) {
    return Stack_get(obj, n);
}

void Stack_delete(Stack* obj) {
    arcangel_free(obj->stack);
    arcangel_free(obj);
}
void arcangel_Stack_delete(Stack* obj) {
    Stack_delete(obj);
}

typedef struct HashTable {
    void** hashTable;
    int* keys;
    int size;
    int elements;
} HashTable;

void HashTable_init(HashTable* obj) {
    obj->hashTable = arcangel_calloc(sizeof(void*), obj->size);
    obj->keys = arcangel_calloc(sizeof(int), obj->size);
    int i = 0;
    for (i = 0; i < obj->size; ++i) {
        obj->hashTable[i] = (void*)0;
    }
    for (i = 0; i < obj->size; ++i) {
        obj->keys[i] = 0;
    }
}

HashTable* HashTable_new() {
    HashTable* obj = arcangel_calloc(1, sizeof(HashTable));
    obj->size = 10;
    obj->elements = 0;
    HashTable_init(obj);
    return obj;
}

void HashTable_put(HashTable* obj, int key, void* value) {
    ++obj->elements;
    if (obj->elements > obj->size / 2) {
        void** oldHashTable = obj->hashTable;
        int* oldKeys = obj->keys;
        int oldSize = obj->size;
        obj->size = obj->size * 2;
        obj->elements = 0;
        HashTable_init(obj);
        int i;
        for (i = 0; i < oldSize; ++i) {
            if (oldHashTable[i] != (void*)0) {
                HashTable_put(obj, oldKeys[i], oldHashTable[i]);
            }
        }
        arcangel_free(oldHashTable);
        arcangel_free(oldKeys);
        obj->elements++;
    }
    int currKey = key % obj->size;
    while (obj->hashTable[currKey] != (void*)0 && obj->keys[currKey] != key) {
        currKey = (currKey + 1) % obj->size;
    }
    obj->hashTable[currKey] = value;
    obj->keys[currKey] = key;
}

void* HashTable_get(HashTable* obj, int key) {
    int currKey = key % obj->size;
    if (obj->hashTable[currKey] == (void*)0) {
        return (void*)0;
    }
    int i = 0;
    while (obj->keys[currKey] != key) {
        currKey = (currKey + 1) % obj->size;
        if (obj->hashTable[currKey] == (void*)0) {
            return (void*)0;
        }
        ++i;
        /* TODO: Check that -1 not a bug. */
        if (i == obj->size - 1) {
            return (void*)0;
        }
    }
    return obj->hashTable[currKey];
}

void HashTable_delete(HashTable* obj) {
    arcangel_free(obj->hashTable);
    arcangel_free(obj->keys);
    arcangel_free(obj);
}

typedef union ReferenceValue {
    int intVal;
    float floatVal;
    void* arrayVal;
} ReferenceValue;

typedef struct arcangel_Reference {
    char type;
    ReferenceValue value;
} Reference;

int getIntValue(Reference* r) {
    switch (r->type) {
    case ReferenceType_INTEGER:
        return r->value.intVal;
    case ReferenceType_FLOAT:
        return (int)r->value.floatVal;
    case ReferenceType_ARRAY:
        return (int)r->value.arrayVal;
    }
    return 0;
}

float getFloatValue(Reference* r) {
    switch (r->type) {
    case ReferenceType_INTEGER:
        return (float)r->value.intVal;
    case ReferenceType_FLOAT:
        return r->value.floatVal;
    case ReferenceType_ARRAY:
        return (float)(int)r->value.arrayVal;
    }
    return 0;
}

int arcangel_toInt(Stack* stack, int arg) {
    Reference* r = (Reference*)Stack_get(stack, arg);
    int i = getIntValue(r);
    return i;
}

float arcangel_toFloat(Stack* stack, int arg) {
    Reference* r = (Reference*)Stack_get(stack, arg);
    float i = getFloatValue(r);
    return i;
}

char* getStrValue(Reference* r);

char* arcangel_toString(Stack* stack, int arg) {
    Reference* r = (Reference*)Stack_get(stack, arg);
    char* i = getStrValue(r);
    return i;
}

int opInt(Operation op, int i1, int i2) {
    switch (op) {
    case Operation_SUM:
        return i1 + i2;
    case Operation_SUB:
        return i1 - i2;
    case Operation_MULT:
        return i1 * i2;
    case Operation_DIV:
        return i1 / i2;
    case Operation_MOD:
        return i1 % i2;
    case Operation_EQ:
        return i1 == i2;
    case Operation_NEQ:
        return i1 != i2;
    case Operation_GT:
        return i1 > i2;
    case Operation_LT:
        return i1 < i2;
    case Operation_GTE:
        return i1 >= i2;
    case Operation_LTE:
        return i1 <= i2;
    case Operation_AND:
        return i1 && i2;
    case Operation_OR:
        return i1 || i2;
    case Operation_BAND:
        return i1 & i2;
    case Operation_BOR:
        return i1 | i2;
    case Operation_BXOR:
        return i1 ^ i2;
    case Operation_SL:
        return i1 << i2;
    case Operation_SR:
        return i1 >> i2;
    default:
        return 0;
    }
}

float opFloat(Operation op, float i1, float i2) {
    switch (op) {
    case Operation_SUM:
        return i1 + i2;
    case Operation_SUB:
        return i1 - i2;
    case Operation_MULT:
        return i1 * i2;
    case Operation_DIV:
        return i1 / i2;
    case Operation_MOD:
        return (int)i1 % (int)i2;
    case Operation_EQ:
        return i1 == i2;
    case Operation_NEQ:
        return i1 != i2;
    case Operation_GT:
        return i1 > i2;
    case Operation_LT:
        return i1 < i2;
    case Operation_GTE:
        return i1 >= i2;
    case Operation_LTE:
        return i1 <= i2;
    case Operation_AND:
        return i1 && i2;
    case Operation_OR:
        return i1 || i2;
    case Operation_BAND:
        return (int)i1 & (int)i2;
    case Operation_BOR:
        return (int)i1 | (int)i2;
    case Operation_BXOR:
        return (int)i1 ^ (int)i2;
    case Operation_SL:
        return (int)i1 << (int)i2;
    case Operation_SR:
        return (int)i1 >> (int)i2;
    default:
        return 0;
    }
}

int uOpInt(Operation op, int i1) {
    switch (op) {
    case Operation_Unary_NEG:
        return -i1;
    case Operation_Unary_INC:
        return ++i1;
    case Operation_Unary_DEC:
        return --i1;
    case Operation_Unary_NOT:
        return !i1;
    case Operation_Unary_BNOT:
        return ~i1;
    default:
        return 0;
    }
}

float uOpFloat(Operation op, float i1) {
    switch (op) {
    case Operation_Unary_NEG:
        return -i1;
    case Operation_Unary_INC:
        return ++i1;
    case Operation_Unary_DEC:
        return --i1;
    case Operation_Unary_NOT:
        return !i1;
    case Operation_Unary_BNOT:
        return ~(int)i1;
    default:
        return 0;
    }
}

void opRef(Operation op, Reference* r1, Reference* r2) {
    int floatOp = 0;
    if (r1->type == ReferenceType_FLOAT || r2->type == ReferenceType_FLOAT) {
        floatOp = 1;
    }
    if (floatOp) {
        float f1 = getFloatValue(r1);
        float f2 = getFloatValue(r2);
        r1->type = ReferenceType_FLOAT;
        r1->value.floatVal = opFloat(op, f1, f2);
    } else {
        int i1 = getIntValue(r1);
        int i2 = getIntValue(r2);
        r1->type = ReferenceType_INTEGER;
        r1->value.intVal = opInt(op, i1, i2);
    }
}

void uOpRef(Operation op, Reference* r1) {
    int floatOp = 0;
    if (r1->type == ReferenceType_FLOAT) {
        floatOp = 1;
    }
    if (floatOp) {
        float f1 = getFloatValue(r1);
        r1->type = ReferenceType_FLOAT;
        r1->value.floatVal = uOpFloat(op, f1);
    } else {
        int i1 = getIntValue(r1);
        r1->type = ReferenceType_INTEGER;
        r1->value.intVal = uOpInt(op, i1);
    }
}

void printRef(Reference* r) {
    switch (r->type) {
    case ReferenceType_INTEGER:
        if (arcangel_printf != 0) {
            arcangel_printf("%i", r->value.intVal);
        }
        break;
    case ReferenceType_FLOAT:
        if (arcangel_printf != 0) {
            arcangel_printf("%f", r->value.floatVal);
        }
        break;
    case ReferenceType_ARRAY:
        if (arcangel_printf != 0) {
            arcangel_printf("%p", r->value.arrayVal);
        }
        break;
    }
}

Operation readOp(char** programPtr) {
    Operation r = (Operation)*((char*)(*programPtr));
    *programPtr += sizeof(char);
    return r;
}

ReferenceType readRefType(char** programPtr) {
    ReferenceType r = (ReferenceType)*((char*)(*programPtr));
    *programPtr += sizeof(char);
    return r;
}

int readInt(char** programPtr) {
    int r = *((int*)(*programPtr));
    *programPtr += sizeof(int);
    return r;
}

char readChar(char** programPtr) {
    char r = *((char*)(*programPtr));
    *programPtr += sizeof(char);
    return r;
}

float readFloat(char** programPtr) {
    float r = *((float*)(*programPtr));
    *programPtr += sizeof(float);
    return r;
}

void* readArray(char** programPtr) {
    void* r = *((void**)(*programPtr));
    *programPtr += sizeof(void*);
    return r;
}

void setRef(Reference* r, char** programPtr) {
    r->type = readRefType(programPtr);
    switch (r->type) {
    case ReferenceType_INTEGER:
        r->value.intVal = readInt(programPtr);
        break;
    case ReferenceType_FLOAT:
        r->value.floatVal = readFloat(programPtr);
        break;
    case ReferenceType_ARRAY:
        r->value.arrayVal = readArray(programPtr);
        break;
    }
}

void skipRefType(char** programPtr) {
    *programPtr += sizeof(char);
}

void skipInt(char** programPtr) {
    *programPtr += sizeof(int);
}

void skipFloat(char** programPtr) {
    *programPtr += sizeof(float);
}

void skipArray(char** programPtr) {
    *programPtr += sizeof(void*);
}

void skipRef(char** programPtr) {
    ReferenceType type = readRefType(programPtr);
    switch (type) {
    case ReferenceType_INTEGER:
        skipInt(programPtr);
        break;
    case ReferenceType_FLOAT:
        skipFloat(programPtr);
        break;
    case ReferenceType_ARRAY:
        skipArray(programPtr);
        break;
    }
}

char* getStrValue(Reference* r) {
     if (r->type == ReferenceType_ARRAY) {
        int size = 0;
        Reference* ptr = (Reference*)r->value.arrayVal;
        char c;
        do {
            c = (char)(ptr[size].value.intVal);
            ++size;
        } while (c != '\0');
        char* str = arcangel_calloc(size, sizeof(char));
        int i;
        for (i = 0; i < size; ++i) {
            str[i] = (char)(ptr[i].value.intVal);
        }
        str[size - 1] = '\0';
        return str;
    } else {
        return 0;
    }
}

typedef struct arcangel_State {
    char* program;
    void* callback;
    HashTable* labels;
    Stack* stack;
    Stack* callStack;
    Stack* heapStack;
    Reference* r1;
    Reference* r2;
} State;

void execute(State* state, char* programPtr) {
    int halt = 0;
    Reference* heap;
    int addr;
    int size;
    Reference* aux;
    void* ptr;
    Reference* r1 = state->r1;
    Reference* r2 = state->r2;
    Stack* stack = state->stack;
    Stack* callStack = state->callStack;
    Stack* heapStack = state->heapStack;
    HashTable* labels = state->labels;
    int lbl;
    char c;
    int i;
    float f;
    Reference* gHeap;

    while (1) {
        if (halt) {
            break;
        }

        Operation op = readOp(&programPtr);
        switch (op) {
        case Operation_PUSH:
            aux = (Reference*)arcangel_calloc(1, sizeof(Reference));
            *(aux) = *r1;
            Stack_push(stack, aux);
            break;
        case Operation_POP:
            aux = (Reference*)Stack_pop(stack);
            *(r1) = *aux;
            arcangel_free(aux);
            break;
        case Operation_POPB:
            aux = (Reference*)Stack_pop(stack);
            *(r2) = *aux;
            arcangel_free(aux);
            break;
        case Operation_HEAP:
            size = readInt(&programPtr);
            if (size == 0) {
                size = 1;
            }
            heap = (Reference*)arcangel_calloc(sizeof(Reference), size);
            Stack_push(heapStack, heap);
            break;
        case Operation_Void_CONST:
            setRef(r1, &programPtr);
            break;
        case Operation_Void_CONSTB:
            setRef(r2, &programPtr);
            break;
        case Operation_SET:
            addr = readInt(&programPtr);
            *(heap + addr) = *r1;
            break;
        case Operation_GET:
            addr = readInt(&programPtr);
            *r1 = *(heap + addr);
            break;
        case Operation_SGET:
            addr = readInt(&programPtr);
            if (addr < stack->elements) {
                *r1 = *(((Reference*)Stack_get(stack, addr)));
            } else {
                arcangel_printf("Stack underflow\n");
                halt = 1;
            }
            break;
        case Operation_GETB:
            addr = readInt(&programPtr);
            *r2 = *(heap + addr);
            break;
        case Operation_GGET:
            addr = readInt(&programPtr);
            gHeap = Stack_bottom(heapStack);
            *r1 = *(gHeap + addr);
            break;
        case Operation_GSET:
            addr = readInt(&programPtr);
            gHeap = Stack_bottom(heapStack);
            *(gHeap + addr) = *r1;
            break;
        case Operation_SUM:
        case Operation_SUB:
        case Operation_MULT:
        case Operation_DIV:
        case Operation_MOD:
        case Operation_EQ:
        case Operation_NEQ:
        case Operation_GT:
        case Operation_LT:
        case Operation_GTE:
        case Operation_LTE:
        case Operation_AND:
        case Operation_OR:
        case Operation_BAND:
        case Operation_BOR:
        case Operation_BXOR:
        case Operation_SL:
        case Operation_SR:
            opRef(op, r1, r2);
            break;
        case Operation_Unary_NEG:
        case Operation_Unary_INC:
        case Operation_Unary_DEC:
        case Operation_Unary_NOT:
        case Operation_Unary_BNOT:
            uOpRef(op, r1);
            break;
        case Operation_INT:
            i = getIntValue(r1);
            r1->type = ReferenceType_INTEGER;
            r1->value.intVal = i;
            break;
        case Operation_FLOAT:
            f = getFloatValue(r1);
            r1->type = ReferenceType_FLOAT;
            r1->value.floatVal = f;
            break;
        case Operation_Void_PRINT:
            if (arcangel_printf != 0) {
                printRef(r1);
                arcangel_printf("\n");
            }
            break;
        case Operation_Load_LBL:
            skipInt(&programPtr);
            break;
        case Operation_Load_END:
            if (arcangel_printf != 0) {
                arcangel_printf("Wrong END instruction.");
            }
            halt = 1;
            break;
        case Operation_Flow_HALT:
            halt = 1;
            break;
        case Operation_Flow_GOTO:
            lbl = readInt(&programPtr);
            void* addr = HashTable_get(labels, lbl);
            programPtr = (char*)addr;
            break;
        case Operation_Flow_JMPZ:
            lbl = readInt(&programPtr);
            if ((r1->type == ReferenceType_INTEGER && r1->value.intVal == 0) ||
                (r1->type == ReferenceType_FLOAT && r1->value.floatVal == 0.0) ||
                (r1->type == ReferenceType_ARRAY && r1->value.arrayVal == 0)) {
                void* addr = HashTable_get(labels, lbl);
                programPtr = (char*)addr;
            }
            break;
        case Operation_Flow_CALL:
            lbl = getIntValue(r1);
            Stack_push(callStack, programPtr);
            programPtr = (char*)HashTable_get(labels, lbl);
            if (programPtr == 0) {
                if (arcangel_printf != 0) {
                    arcangel_printf("Bad function pointer.");
                }
                halt = 1;
            }
            break;
        case Operation_Flow_RET:
            Stack_pop(heapStack);
            arcangel_free(heap);
            heap = (Reference*)Stack_top(heapStack);
            if (callStack->elements == 0) {
                /*if (arcangel_printf != 0) {
                    arcangel_printf("Call stack underflow.");
                }*/
                halt = 1;
            } else {
                programPtr = (char*)Stack_pop(callStack);
            }
            break;
        case Operation_ALLOC:
            size = getIntValue(r1);
            ptr = arcangel_calloc(size, sizeof(Reference));
            r1->type = ReferenceType_ARRAY;
            r1->value.arrayVal = ptr;
            break;
        case Operation_FREE:
            if (r1->type == ReferenceType_ARRAY) {
                arcangel_free(r1->value.arrayVal);
            } else {
                if (arcangel_printf != 0) {
                    arcangel_printf("Trying to free a non array reference.");
                }
                halt = 1;
            }
            break;
        case Operation_POINT:
            if (r1->type == ReferenceType_ARRAY) {
                int addr = getIntValue(r2);
                Reference* ref = (Reference*)r1->value.arrayVal;
                ref += addr;
                r1->value.arrayVal = (void*)ref;
            } else {
                if (arcangel_printf != 0) {
                    arcangel_printf("Trying to point a non array reference.");
                }
                halt = 1;
            }
            break;
        case Operation_RGET:
            if (r1->type == ReferenceType_ARRAY) {
                *r1 = *((Reference*)r1->value.arrayVal);
            } else {
                if (arcangel_printf != 0) {
                    arcangel_printf("Trying to rget a non array reference.");
                }
                halt = 1;
            }
            break;
        case Operation_RSET:
            if (r1->type == ReferenceType_ARRAY) {
                *((Reference*)r1->value.arrayVal) = *r2;
            } else {
                if (arcangel_printf != 0) {
                    arcangel_printf("Trying to rset a non array reference.");
                }
                halt = 1;
            }
            break;
        case Operation_STR:
            size = 0;
            char* strStart = programPtr;
            do {
                ++size;
                c = readChar(&programPtr);
            } while (c != 0);
            ptr = arcangel_calloc(size, sizeof(Reference));
            r1->type = ReferenceType_ARRAY;
            r1->value.arrayVal = ptr;
            programPtr = strStart;
            size = 0;
            do {
                c = readChar(&programPtr);
                ((Reference*)ptr)[size].type = ReferenceType_INTEGER;
                ((Reference*)ptr)[size].value.intVal = (int)c;
                ++size;
            } while (c != 0);
            break;
        case Operation_PRINTS:
            if (r1->type == ReferenceType_ARRAY) {
                if (arcangel_printf != 0) {
                    size = 0;
                    ptr = r1->value.arrayVal;
                    do {
                        c = (char)(((Reference*)ptr)[size].value.intVal);
                        if (c == '\0') {
                            break;
                        }
                        arcangel_printf ("%c", c);
                        ++size;
                    } while (c != '\0');
                }
            } else {
                if (arcangel_printf != 0) {
                    arcangel_printf ("Trying to prints a non array reference.\n");
                }
                halt = 1;
            }
            break;
        case Operation_SYSCALL:
            if (arcangel_sysCall != 0) {
                arcangel_sysCall(stack);
            }
            break;
        case Operation_CALLBACK:
            break;
        }
    }
}

State* arcangel_State_load(char* program) {
    State* state = arcangel_calloc(1, sizeof(State));
    state->callback = 0;
    state->program = program;
    char* programPtr = program;
    Reference* r1 = (Reference*)arcangel_calloc(1, sizeof(Reference));
    state->r1 = r1;
    Reference* r2 = (Reference*)arcangel_calloc(1, sizeof(Reference));
    state->r2 = r2;
    HashTable* labels = HashTable_new();
    state->labels = labels;
    int halt = 0;
    int lbl;
    char c;

    while (1) {
        if (halt) {
            break;
        }

        Operation op = readOp(&programPtr);
        switch (op) {
        case Operation_Void_CONST:
        case Operation_Void_CONSTB:
            skipRef(&programPtr);
            break;
        case Operation_HEAP:
        case Operation_Flow_GOTO:
        case Operation_Flow_JMPZ:
        case Operation_GET:
        case Operation_SET:
        case Operation_GETB:
        case Operation_GSET:
        case Operation_GGET:
        case Operation_SGET:
            skipInt(&programPtr);
            break;
        case Operation_Load_LBL:
            lbl = readInt(&programPtr);
            HashTable_put(labels, lbl, programPtr);
            break;
        case Operation_Load_END:
            halt = 1;
            break;
        case Operation_STR:
            do {
                c = readChar(&programPtr);
            } while (c != '\0');
            break;
        case Operation_CALLBACK:
            state->callback = programPtr;
            break;
        default:
            break;
        }
    }

    Stack* callStack = Stack_new();
    state->callStack = callStack;
    Stack* heapStack = Stack_new();
    state->heapStack = heapStack;
    Stack* stack = Stack_new();
    state->stack = stack;
    execute(state, state->program);
    return state;
}

Reference* arcangel_State_callback(State* obj) {
    if (obj->callback == 0) {
        arcangel_printf("No callback set.");
        return 0;
    } else {
        execute(obj, (char*)obj->callback);
        Reference* aux = (Reference*)Stack_pop(obj->stack);
        return aux;
    }
}

int arcangel_State_hasCallback(State* obj) {
    return obj->callback != 0;
}

void arcangel_State_delete(State* obj) {
    arcangel_free(obj->r1);
    arcangel_free(obj->r2);
    HashTable_delete(obj->labels);
    Stack_delete(obj->callStack);
    Reference* heap = Stack_pop(obj->heapStack);
    arcangel_free(heap);
    Stack_delete(obj->heapStack);
    Stack_delete(obj->stack);
}
