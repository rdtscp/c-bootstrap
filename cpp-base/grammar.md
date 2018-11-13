# Mini-C/C++ Grammar
```
program    -> include* ((structdecl)* (vardecl)* (fundecl)*)* EOF

include    -> "#include" STRING_LITERAL

structdecl -> structtype "{" (vardecl)+ "}" ";"       # structure declaration

vardecl    -> type IDENT ";"                          # normal declaration, e.g. int a;
            | type IDENT "[" INT_LITERAL "]" ";"      # array declaration, e.g. int a[2];

fundecl    -> type IDENT "(" params ")" block         # function declaration

type       -> ("int" | "char" | "void" | structtype) ["*"]
structtype -> "struct" IDENT

params     -> [ type IDENT ("," type IDENT)* ]

stmt       -> block
            | "while" "(" exp ")" stmt                # while loop
            | "if" "(" exp ")" stmt ["else" stmt]     # if then else
            | "return" [exp] ";"                      # return
            | exp "=" exp ";"                         # assignment
            | exp ";"                                 # expression statement, e.g. a function call

block      -> "{" ((vardecl)* (stmt)*)* "}"

exp        -> "(" exp ")"
            | (IDENT | INT_LITERAL)
            | "-" exp
            | CHAR_LITERAL
            | STRING_LITERAL
            | exp ( ">" | "<" | ">=" | "<=" | "!=" | "==" | "+" | "-" | "/" | "*" | "%" | "||" | "&&" ) exp  # binary operators
            | arrayaccess | fieldaccess | valueat | funcall | sizeof | typecast

funcall      -> IDENT "(" [ exp ("," exp)* ] ")"
arrayaccess  -> exp "[" exp "]"                       # array access
fieldaccess  -> exp "." IDENT                         # structure field member access
valueat      -> "*" exp                               # Value at operator (pointer indirection)
sizeof       -> "sizeof" "(" type ")"                 # size of type
typecast     -> "(" type ")" exp                      # type casting
```