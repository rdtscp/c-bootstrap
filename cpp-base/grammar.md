# Mini-C/C++ Grammar
```
program    -> (include)* (decl)* EOF

include    -> "#include" STRING_LITERAL

decl       -> (structdecl)* (vardecl)* (fundecl)*

structdecl -> structtype "{" (vardecl)+ "}" ";"

vardecl    -> type IDENT ";"
            | type IDENT "[" INT_LITERAL "]" ";"

fundecl    -> type IDENT "(" params ")" block

type       -> ("int" | "char" | "void" | structtype) (ASTERIX)*
structtype -> "struct" IDENT

params     -> [ type IDENT ("," type IDENT)* ]

stmt       -> vardecl
            | block
            | "while" "(" exp ")" stmt
            | "if" "(" exp ")" stmt ["else" stmt]
            | "return" [exp] ";"
            | exp "=" exp ";"
            | exp ";"

block      -> "{" stmt* "}"

exp        -> "(" exp ")"
            | (IDENT | INT_LITERAL)
            | "-" exp
            | CHAR_LITERAL
            | STRING_LITERAL
            | exp ( ">" | "<" | ">=" | "<=" | "!=" | "==" | "+" | "-" | "/" | "*" | "%" | "||" | "&&" ) exp  # binary operators
            | arrayaccess | fieldaccess | valueat | funcall | sizeof | typecast

funcall      -> IDENT "(" [ exp ("," exp)* ] ")"
arrayaccess  -> exp "[" exp "]"
fieldaccess  -> exp "." IDENT
valueat      -> "*" exp
sizeof       -> "sizeof" "(" type ")"
typecast     -> "(" type ")" exp
```