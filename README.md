# ZOLP
Zero Order Logic Parser with Conjunctive Normal Formula generator

The parser is basically Recursive Descend Parser while I maintain the parsed tokens in an Abstract Syntax Tree (AST) for further modifications, and easier handling.

a raw depiction of what this program makes:
INPUT: 
  ![(a and b) or (!b and c)] & [(A and B) or (C and D)]
OUTPUT:
INITIAL Parsing:
    and
    |-- or
    |   |-- and
    |   |   |-- [D]
    |   |   +-- [C]
    |   +-- and
    |       |-- [B]
    |       +-- [A]
    +-- !or
        |-- and
        |   |-- [c]
        |   +-- ![b]
        +-- and
            |-- [b]
            +-- [a]


 AFTER atomizeNegation:
    and
    |-- or
    |   |-- and
    |   |   |-- [D]
    |   |   +-- [C]
    |   +-- and
    |       |-- [B]
    |       +-- [A]
    +-- and
        |-- or
        |   |-- ![c]
        |   +-- [b]
        +-- or
            |-- ![b]
            +-- ![a]


 AFTER CNF:
    and
    |-- and
    |   |-- and
    |   |   |-- or
    |   |   |   |-- [D]
    |   |   |   +-- [B]
    |   |   +-- or
    |   |       |-- [C]
    |   |       +-- [B]
    |   +-- and
    |       |-- or
    |       |   |-- [D]
    |       |   +-- [A]
    |       +-- or
    |           |-- [C]
    |           +-- [A]
    +-- and
        |-- or
        |   |-- ![c]
        |   +-- [b]
        +-- or
            |-- ![b]
            +-- ![a]


