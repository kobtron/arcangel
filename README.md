# arcangel
Tiny language, compiler and virtual machine

## Generating the compiler and the virtual machine
```
gcc arcangel-compiler.c arcangel-parser.c arcangel-vm.c arcangelc.c -o arcangelc
gcc arcangel-vm.c arcangel.c -o arcangel
```
## Compiling and running an example
```
arcangelc examples/hello.arc
arcangel a.out
```
## Regenerating the lexer and parser
The lexer is generated with flex
```
flex arcangel-lexer.l
move lex.yy.c arcangel-lexer.h
```
The parser is generated with Lemon
```
Lemon.exe arcangel-parser.y
```
This last command depends on you having the Lemon and the lempar.c template.
