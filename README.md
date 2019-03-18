# arcangel
Tiny language, compiler and virtual machine

## Reenerating the lexer and parser
The lexer is generated with flex
```
flex arcangel-lexer.l
move lex.yy.c arcangel-lexer.h
```
The parser is generated with Lemon
```
Lemon.exe arcangel-parser.y
```
