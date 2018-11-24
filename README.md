# c-bootstrap

[![Build Status](https://travis-ci.com/acwilson96/c-bootstrap.svg?branch=master)](https://travis-ci.com/acwilson96/c-bootstrap)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

* Bootstrapping a simple C based compiler written in C++.
* The compiler will intially be used to build simple C programs, and then extend the grammar enough so that it is able to compile itself.

### Mini-C/C++ Grammar
Progressively changing to adopt new features of the language(s).
```
program      -> (include)* (decl)* EOF

include      -> "#include" STRING_LITERAL

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
              | exp "=" exp ";"
              | exp ";"

block        -> "{" stmt* "}"

exp          -> "(" exp2 ")"
              | exp2

exp2         -> exp3
              | exp3 "||" exp3
              | exp3 "&&" exp3

exp3         -> exp4
              | exp4 "!=" exp4
              | exp4 "==" exp4

exp4         -> exp5
              | exp5 "<" exp5
              | exp5 ">" exp5
              | exp5 "<=" exp5
              | exp5 ">=" exp5

exp5         -> exp6
              | exp6 "+" exp6
              | exp6 "-" exp6

exp6         -> exp7
              | exp7 "*" exp7
              | exp7 "/" exp7
              | exp7 "%" exp7

exp7         -> "sizeof" "(" type ")"
              | "*" exp8
              | "(" type ")" exp8
              | "-" exp8
              | exp8

exp8         -> IDENT "(" exp9 ("," exp9)* ")"
              | IDENT "(" exp9 ")"
              | IDENT "(" ")"
              | IDENT
              | exp9 "." IDENT
              | exp9 "[" exp9 "]"
              | exp9

exp9         -> INT_LITERAL
              | CHAR_LITERAL
              | STRING_LITERAL
```