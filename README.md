# c-bootstrap

[![Build Status](https://travis-ci.com/rdtscp/c-bootstrap.svg?branch=master)](https://travis-ci.com/rdtscp/c-bootstrap)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

-   Bootstrapping a simple C based compiler written in C++.
-   The compiler will intially be used to build simple C programs, and then extend the grammar enough so that it is able to compile itself.

## Mini-C/C++ Grammar

Progressively changing to adopt new features of the language(s).

    program      -> (include)* (decl)* EOF

    decl         -> structdecl ";"
                 -> classdecl ";"
                 -> vardecl ";"
                 -> arrvardecl ";"
                 -> fundecl ";"
                 -> fundef
                 -> typedecl ";"
                 -> enumdecl ";"

    structdecl   -> structtype "{" (vardecl)+ "}"

    classdecl    -> "class" IDENT "{" (decl | accessmod )* "}" ";"

    accessmod    -> "public:" | "private:" | "protected:"

    vardecl      -> type IDENT

    arrvardecl   -> vardecl "[" INT_LITERAL "]"

    fundecl      -> type IDENT "(" params ")"

    fundef       -> type IDENT "(" params ")" block

    typedecl     -> "typedef" (type | structdecl) IDENT

    enumdecl     -> "enum" "{" statelist "}"

    statelist    -> (IDENT ["=" INT_LITERAL] ",")* IDENT ["=" INT_LITERAL]

    type         -> ("int" | "char" | "void" | structtype | classtype ) (ASTERIX)*

    structtype   -> "struct" IDENT

    classtype    -> IDENT

    params       -> [ type IDENT ("," type IDENT)* ]

    stmt         -> vardecl ";"
                  | block
                  | "do" stmt "while" "(" exp ")" ";"
                  | "while" "(" exp ")" stmt
                  | "if" "(" exp ")" stmt ["else" stmt]
                  | "return" [exp] ";"
                  | expr "=" expr ";"
                  | expr ";"

    block        -> "{" stmt* "}"

    expr         -> "(" boolExpr ")"
                  | boolExpr

    boolExpr     -> equalExpr
                  | equalExpr "||" equalExpr
                  | equalExpr "&&" equalExpr

    equalExpr    -> compExpr
                  | compExpr "!=" compExpr
                  | compExpr "==" compExpr

    compExpr     -> addExpr
                  | addExpr "<" addExpr
                  | addExpr ">" addExpr
                  | addExpr "<=" addExpr
                  | addExpr ">=" addExpr

    addExpr      -> mulExpr
                  | mulExpr "+" mulExpr
                  | mulExpr "-" mulExpr

    mulExpr      -> unaryExpr
                  | unaryExpr "*" unaryExpr
                  | unaryExpr "/" unaryExpr
                  | unaryExpr "%" unaryExpr

    unaryExpr    -> "sizeof" "(" type ")"
                  | "*" objExpr
                  | "(" type ")" objExpr
                  | "-" objExpr
                  | "new" type [ "(" litExpr ("," litExpr)* ")" ] ";"
                  | objExpr

    objExpr      -> IDENT "(" litExpr ("," litExpr)* ")"
                  | IDENT "(" litExpr ")"
                  | IDENT "(" ")"
                  | IDENT
                  | litExpr "." IDENT
                  | litExpr "[" litExpr "]"
                  | litExpr

    litExpr      -> INT_LITERAL
                  | CHAR_LITERAL
                  | STRING_LITERAL
                  | exp

### macOS Notes

    # Building Assembly:
    acc main.cpp
    # Building Objects
    nasm -f macho x86.s
    # Link Objects
    ld -macosx_version_min 10.14 -lSystem -o x86 x86.o

## History

Notes of deprecated/replaced functionality below:

### Pre-Processor Grammar

     #define
     #endif
     #ifdef
     #ifndef
     #if
     #elif
     #else
     #include
