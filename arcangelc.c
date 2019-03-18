#include "arcangel-compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char* fileToBuff(char* file) {
    char *source = 0;
    if (file == 0) {
        file = "main.arc";
    }
    FILE *fp = fopen(file, "r");
    fseek(fp, 0L, SEEK_END);
    long bufsize = ftell(fp);
    source = malloc(sizeof(char) * (bufsize + 1));
    fseek(fp, 0L, SEEK_SET);
    int newLen = fread(source, sizeof(char), bufsize, fp);
    source[newLen++] = '\0';
    fclose(fp);
    return source;
}

void sysCall(arcangel_Stack* stack) {
    printf("Calling C form Arcangel\n");
    int i = arcangel_toInt(stack, 0);
    printf("First argument: %i\n", i);
    float f = arcangel_toFloat(stack, 1);
    printf("Second argument: %f\n", f);
    char* s = arcangel_toString(stack, 2);
    printf("Third argument: %s\n", s);
    free(s);
}

int main(int argc, char** argv) {
    char * file = 0;
    if (argc >= 2) {
        file = argv[1];
    }
    char* source = fileToBuff(file);
    arcangel_setCalloc(calloc);
    arcangel_setFree(free);
    arcangel_setPrintf(printf);
    int size = 0;
    char* program = arcangel_compile(source, &size);
    free(source);
    if (program) {
        FILE *my_file = fopen("a.out", "wb");
        fwrite(program, sizeof(char), size, my_file);
    }
    return 0;
}
