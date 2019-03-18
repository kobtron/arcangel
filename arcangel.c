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
        arcangel_State* A = arcangel_State_load(program);
        if (arcangel_State_hasCallback(A)) {
            arcangel_Reference* r = arcangel_State_callback(A);
            if (r != 0) {
                free(r);
            }
        }
        arcangel_State_delete(A);
    }
    return 0;
}
