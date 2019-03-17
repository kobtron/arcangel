#include <stdio.h>
#include <stdlib.h>
#include "../arcangel-compiler.h"

int main2() {
    char source[] = "\
    program()\
        var hello\
        hello = \"Hello world\\n\"\
        prints(hello)\
        free(hello)\
    end";
    arcangel_setPrintf(printf);
    arcangel_setCalloc(calloc);
    arcangel_setFree(free);
    char* program = arcangel_compile(source);
    arcangel_State* A = arcangel_State_load(program);
    arcangel_State_delete(A);
    return 0;
}
