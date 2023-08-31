# c-bootstrap

[![Build Status](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2Frdtscp%2Fc-bootstrap%2Fbadge&style=flat)](https://actions-badge.atrox.dev/rdtscp/c-bootstrap/goto)
[![codecov](https://codecov.io/gh/rdtscp/c-bootstrap/branch/master/graph/badge.svg)](https://codecov.io/gh/rdtscp/c-bootstrap)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

-   Bootstrapping a simple C/C++ based compiler written in C++.
-   The compiler will intially be used to build simple C programs, and then extend the grammar enough so that it is able to compile itself.

## Requirements

-   cmake
-   For Binary CodeGen:
    -   nasm
    -   macOS with Intel x64 CPU

## Usage

### Building

Building/Running/Testing in a unix environment:

# Build Code

    git clone https://github.com/rdtscp/c-bootstrap.git
    cd ./c-bootstrap/
    mkdir build ; cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ../
    make -j && ctest --output-on-failure

### Dot Graph Output

Building/Running/Testing in a unix environment:

    # Run Examples below
    ./acc ../test/tests/Test_Parser/BinOp/test.cpp test.dot --graph
    ./acc ../test/tests/Test_Parser/BinOp/test.cpp test_opt.dot --graph --opt

Then paste the contents of test.dot and test_opt.dot into a dot-graph generator (I recommend <http://www.webgraphviz.com/>).
You can then see a graphical representation of the input program.

### Binary Generation

Building/Running/Testing on macOS:

    # Run Examples below
    ./acc ../test/tests/main.cpp test.out
    ./test.out

* * *

## Pre-Processor Grammar

    #define
    #else
    #endif
    #ifdef
    #ifndef
    #include

## Mini-C/C++ Grammar

Progressively changing to adopt new features of the language(s).

    program      -> (include)* (decl)* EOF

    decl         -> classdecl ";"
                 -> vardecl ";"
                 -> arrvardecl ";"
                 -> fundecl ";"
                 -> fundef
                 -> typedecl ";"
                 -> enumdecl ";"

    classdecl    -> "class" IDENT "{" (decl | accessmod )* "}" ";"

    accessmod    -> "public:" | "private:" | "protected:"

    vardecl      -> type IDENT

    arrvardecl   -> vardecl "[" INT_LITERAL "]"

    fundecl      -> type (IDENT | opoverloads) "(" params ")"

    opoverloads  -> "operator="
                  | "operator=="
                  | "operator!="

    fundef       -> type IDENT "(" params ")" block

    typedecl     -> "typedef" type IDENT

    enumdecl     -> "enum" "{" statelist "}"

    statelist    -> (IDENT ["=" INT_LITERAL] ",")* IDENT ["=" INT_LITERAL]

    type         -> ("int" | "char" | "void", "unsigned int", "bool" | classtype ) (ASTERIX)*

    classtype    -> IDENT

    params       -> [ type IDENT ("," type IDENT)* ]

    stmt         -> vardecl ";"
                  | block
                  | "do" stmt "while" "(" exp ")" ";"
                  | "while" "(" exp ")" stmt
                  | "if" "(" exp ")" stmt ["else" stmt]
                  | "return" [exp] ";"
                  | "delete" IDENT ";"
                  | "delete[]" IDENT ";"
                  | "throw" STRING_LITERAL ";"
                  | expr "=" expr ";"
                  | expr ";"

    block        -> "{" stmt* "}"

    expr         -> boolExpr ( "?" boolExpr ":" boolExpr )*
                  | boolExpr "+=" boolExpr

    boolExpr     -> equalExpr
                  | equalExpr "||" equalExpr ( ( "||" || "&&" ) equalExpr )*
                  | equalExpr "&&" equalExpr ( ( "||" || "&&" ) equalExpr )*

    equalExpr    -> compExpr
                  | compExpr "!=" compExpr
                  | compExpr "==" compExpr

    compExpr     -> addExpr
                  | addExpr "<" addExpr
                  | addExpr ">" addExpr
                  | addExpr "<=" addExpr
                  | addExpr ">=" addExpr

    addExpr      -> mulExpr
                  | mulExpr "+" mulExpr ( ( "+" || "-" ) mulExpr )*
                  | mulExpr "-" mulExpr ( ( "+" || "-" ) mulExpr )*

    mulExpr      -> unaryExpr
                  | unaryExpr "*" unaryExpr ( ( "*" || "/" || "%" ) unaryExpr )*
                  | unaryExpr "/" unaryExpr ( ( "_" || "/" || "%" ) unaryExpr )*
                  | unaryExpr "%" unaryExpr ( ( "_" || "/" || "%" ) unaryExpr )*


    unaryExpr    -> "sizeof" "(" type ")"
                  | "*" objExpr
                  | "(" type ")"
                  | "-" objExpr
                  | "++" objExpr
                  | "new" type [ "(" litExpr ("," litExpr)* ")" ] ";"
                  | "&" objExpr
                  | "!" objExpr
                  | objExpr "[" expr "]
                  | objExpr

    objExpr      -> funCall (objExprOp)*
                  | IDENT (objExprOp)*
                  | "this" (objExprOp)*
                  | litExpr
                
    objExprOp    -> "." funCall
                  | "->" funCall
                  | "." objExpr
                  | "->" objExpr
                  | "[" objExpr "]" 


    funCall      -> IDENT "(" litExpr ("," litExpr)\* ")"
                  | IDENT "(" litExpr ")"
                  | IDENT "(" ")"


    litExpr      -> INT_LITERAL
                  | CHAR_LITERAL
                  | STRING_LITERAL
                  | "true"
                  | "false"
                  | "(" expr ")"
                  | expr

* * *

### macOS Notes

    # Building Assembly:
    acc main.cpp
    # Building Objects
    nasm -f macho64 x86.s
    # Link Objects
    ld -no_pie -macosx_version_min 10.15 -lSystem -o x86 x86.o
