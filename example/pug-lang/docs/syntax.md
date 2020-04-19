# Pug programming language Syntax

## Statements

| Example | Explanation                                                         |
| ------- | ------------------------------------------------------------------- |
| `...`   | list of statements (*declaration* or *expression* separated by `;`) |
| `...;`  | list of statements (statements may ends with optional `;`)          |

## Declaratons

| Example          | Explanation         |
| ---------------- | ------------------- |
| `let var = expr` | variable definition |

## Block, lambda expression

| Example                               | Explanation                                           |
| ------------------------------------- | ----------------------------------------------------- |
| `{...}`                               | block (enclosed list of statements)                   |
| `{...;}`                              | block (enclosed statements may ends with optional `;` |
| <code>&#124;x y ...&#124; expr</code> | lambda (i.e. function object, closure)                |

## Binary operators

| Operator                  | Associativity     | Example                             | Explanation                         |
| ------------------------- | ----------------- | ----------------------------------- | ----------------------------------- |
| `=`                       | non-associative   | `var = expr`                        | assignment                          |
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
