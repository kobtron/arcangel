%{
    #include <string.h>
    #include "arcangel-compiler.h"
    #include "arcangel-parser.h"
    int line = 1, col = 1;
%}
%option never-interactive
%%

[0-9]+ {
    col += (int) strlen(yytext);
    tkn.value = atoi(yytext);
    return INTEGER;
}

[0-9]*\.[0-9]+ {
    col += (int) strlen(yytext);
    tkn.fValue = atof(yytext);
    return FLOAT;
}

"*" { col += (int) strlen(yytext); return MULT; }
"/" { col += (int) strlen(yytext); return DIV; }
"%" { col += (int) strlen(yytext); return MOD; }
"+" { col += (int) strlen(yytext); return SUM; }
"-" { col += (int) strlen(yytext); return SUB; }
"<<" { col += (int) strlen(yytext); return SL; }
">>" { col += (int) strlen(yytext); return SR; }
"<=" { col += (int) strlen(yytext); return LTE; }
"<" { col += (int) strlen(yytext); return LT; }
">=" { col += (int) strlen(yytext); return GTE; }
">" { col += (int) strlen(yytext); return GT; }
"==" { col += (int) strlen(yytext); return EQ; }
"!=" { col += (int) strlen(yytext); return NEQ; }
"&" { col += (int) strlen(yytext); return BAND; }
"^" { col += (int) strlen(yytext); return BXOR; }
"|" { col += (int) strlen(yytext); return BOR; }
"and" { col += (int) strlen(yytext); return AND; }
"or" { col += (int) strlen(yytext); return OR; }

"(" { col += (int) strlen(yytext); return LPAR; }
")" { col += (int) strlen(yytext); return RPAR; }

"[" { col += (int) strlen(yytext); return LBRK; }
"]" { col += (int) strlen(yytext); return RBRK; }

"++" { col += (int) strlen(yytext); return INC; }
"--" { col += (int) strlen(yytext); return DEC; }
"!" { col += (int) strlen(yytext); return NOT; }
"~" { col += (int) strlen(yytext); return BNOT; }

"=" { col += (int) strlen(yytext); return ASGN; }
"," { col += (int) strlen(yytext); return COMMA; }

"var" { col += (int) strlen(yytext); return VAR; }
"prints" { col += (int) strlen(yytext); return PRINTS; }
"print" { col += (int) strlen(yytext); return PRINT; }
"if" { col += (int) strlen(yytext); return IF; }
"then" { col += (int) strlen(yytext); return THEN; }
"end" { col += (int) strlen(yytext); return END; }
"else" { col += (int) strlen(yytext); return ELSE; }
"program" { col += (int) strlen(yytext); return PROGRAM; }
"function" { col += (int) strlen(yytext); return FUNCTION; }
"callback" { col += (int) strlen(yytext); return CALLBACK; }
"call" { col += (int) strlen(yytext); return CALL; }
"while" { col += (int) strlen(yytext); return WHILE; }
"do" { col += (int) strlen(yytext); return DO; }
"repeat" { col += (int) strlen(yytext); return REPEAT; }
"until" { col += (int) strlen(yytext); return UNTIL; }
"break" { col += (int) strlen(yytext); return BREAK; }
"continue" { col += (int) strlen(yytext); return CONTINUE; }
"return" { col += (int) strlen(yytext); return RETURN; }
"int" { col += (int) strlen(yytext); return INTCAST; }
"float" { col += (int) strlen(yytext); return FLOATCAST; }
"new" { col += (int) strlen(yytext); return NEW; }
"free" { col += (int) strlen(yytext); return FREE; }

[A-Za-z][A-Za-z0-9]* {
    col += (int) strlen(yytext);
    strcpy(tkn.str, yytext);
    return ID;
}

[ \t] { col += (int) strlen(yytext); }
[\n\r] {
    ++line;
    col = 1;
}

["]([^"\\\n]|\\.)*["] {
    int len = (int) strlen(yytext);
    col += len;
    int i;
    int j = 0;
    currStr = calloc(len, sizeof(char));

    for (i = 1; i < len - 1; ++i) {
        char c = yytext[i];
        if (c == '\\') {
            ++i;
            c = yytext[i];

            switch (c) {
            case 'a':
                c = '\a';
                break;
            case 'b':
                c = '\b';
                break;
            case 'f':
                c = '\f';
                break;
            case 'n':
                c = '\n';
                break;
            case 'r':
                c = '\r';
                break;
            case 't':
                c = '\t';
                break;
            case 'v':
                c = '\v';
                break;
            case '\\':
                c = '\\';
                break;
            case '\'':
                c = '\'';
                break;
            case '\"':
                c = '\"';
                break;
            }
        }
        currStr[j] = c;
        ++j;
    }

    currStr[j] = 0;
    return STR;
}

. {
    gError = 1;
}

%%
/**
 * reset the line and column count
 *
 *
 */
void reset_lexer(void)
{

  line = 1;
  col  = 1;

}

/**
 * yyerror() is invoked when the lexer or the parser encounter
 * an error. The error message is passed via *s
 *
 *
 */
void yyerror(char *s)
{
  printf("error: %s at line: %d col: %d\n",s,line,col);

}

int yywrap(void)
{
  return 1;
}
