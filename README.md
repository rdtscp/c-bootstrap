# c-bootstrap

[![Build Status](https://travis-ci.com/acwilson96/c-bootstrap.svg?branch=master)](https://travis-ci.com/acwilson96/c-bootstrap)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

* Bootstrapping a simple C based compiler written in C++.
* The compiler will intially be used to build simple C programs, and then extend the grammar enough so that it is able to compile itself.

### Pre-Processor Grammar
 * #define
 * #endif
 * #ifndef
 * #include

### Mini-C/C++ Grammar
Progressively changing to adopt new features of the language(s).
```
program      -> (include)* (decl)* EOF

decl         -> (structdecl)* (vardecl)* (fundecl)*

structdecl   -> structtype "{" (vardecl)+ "}" ";"

vardecl      -> type IDENT ";"
              | type IDENT "[" INT_LITERAL "]" ";"

fundecl      -> type IDENT "(" params ")" block

type         -> ("int" | "char" | "void" | structtype) (ASTERIX)*
structtype   -> "struct" IDENT

params       -> [ type IDENT ("," type IDENT)* ]

stmt         -> vardecl
              | block
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
```