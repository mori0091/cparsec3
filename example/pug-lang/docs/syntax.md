# Pug programming language Syntax

## Comments

| Example | Explanation                                                               |
| ------- | ------------------------------------------------------------------------- |
| `//...` | line comments (`//` followed by any characters and ends with end-of-line) |

## Statements

| Example | Explanation                                                               |
| ------- | ------------------------------------------------------------------------- |
| `...`   | list of statements (*declarations* and/or *expressions* separated by `;`) |
| `...;`  | list of statements (statements may ends with optional `;`)                |

## Declaratons

| Example         | Explanation          |
| --------------- | -------------------- |
| `var x : type`  | variable declaration |
| `let x = expr`  | variable definition  |

## Block, lambda expression

| Example                                                   | Explanation                                           |
| --------------------------------------------------------- | ----------------------------------------------------- |
| `{ stmts }`                                               | block (enclosed list of statements)                   |
| `if expr { stmts } else { stmts }`                        | if block                                              |
| `if expr { stmts } else if expr { stmts } else { stmts }` | if block                                              |
| <code>&#124;x&#124; expr</code>                           | lambda (i.e. function object, closure)                |
| <code>&#124;x y ...&#124; expr</code>                     | lambda (i.e. function object, closure)                |
| <code>&#124;x&#124; &#124;y&#124; ... expr</code>         | lambda (i.e. function object, closure)                |

## Binary operators

| Operator                  | Associativity     | Example                             | Explanation                         |
| ------------------------- | ----------------- | ----------------------------------- | ----------------------------------- |
| `=`                       | non-associative   | `x = expr`                          | assignment                          |
| <code>&#124;&#124;</code> | right-associative | <code>expr &#124;&#124; expr</code> | logical OR                          |
| `&&`                      | right-associative | `expr && expr`                      | logical AND                         |
| `==`                      | non-associative   | `expr == expr`                      | equality comparison                 |
| `!=`                      | non-associative   | `expr != expr`                      | nonequality comparison              |
| `<=`                      | non-associative   | `expr <= expr`                      | less than or equal to comparison    |
| `<`                       | non-associative   | `expr < expr`                       | less than comparison                |
| `>=`                      | non-associative   | `expr >= expr`                      | greater than or equal to comparison |
| `>`                       | non-associative   | `expr > expr`                       | greater than comparison             |
| `+`                       | left-associative  | `expr + expr`                       | arithmetic addition                 |
| `-`                       | left-associative  | `expr - expr`                       | arithmetic subtraction              |
| `*`                       | left-associative  | `expr * expr`                       | arithmetic multiplication           |
| `/`                       | left-associative  | `expr / expr`                       | arithmetic division                 |
| `%`                       | left-associative  | `expr % expr`                       | arithmetic reminder                 |

## Unary operators

| Operator | Example  | Explanation                      |
| -------- | -------- | -------------------------------- |
| `+`      | `+ expr` | (equivalent to `expr`)           |
| `-`      | `- expr` | arithmetic negation              |
| `!`      | `! expr` | logical NOT / bitwise complement |

## Parenthesis

| Example  | Explanation              |
| -------- | ------------------------ |
| `(expr)` | parenthesized expression |

## Literal

| Example           | Explanation                           |
| ----------------- | ------------------------------------- |
| `1`, `10`, `-100` | decimal integer                       |
| `false`, `true`   | boolean                               |
| `()`              | the singleton value of the unit type. |

## Basic Type

| Example | Explanation  |
| ------- | ------------ |
| `()`    | unit type    |
| `bool`  | boolean type |
| `int`   | integer type |
