# Pug programming language Syntax

## Common syntax rules
- list of statements described in top-level are seperated with `;`.
- list of statements described in top-level may or may not ends with `;`.

- list of statements enclosed in `{` and `}` are seperated with `;`.
- list of statements enclosed in `{` and `}` may or may not ends with `;`.

- empty statement are not permitted.
  - i.e. ` `, `;`, `{}`, `{;}`, `...;;`, `{...;;}` are not permitted.

For example:
~~~rust
expr;              // ok
expr;              // ok
expr;              // ok
~~~

~~~rust
expr;              // ok
expr;              // ok
expr               // ok: last `;` may be omitted.
~~~

~~~rust
{ expr; expr }     // ok
~~~

~~~rust
{ expr; expr; }    // ok
~~~

~~~rust
{ expr; expr; ; }  // syntax error: empty statement is not permitted.
~~~

~~~rust
{ expr; expr };    // ok (note that `;` is needed after `}`)
{ expr; expr; }    // ok
~~~

~~~rust
{ expr; expr };    // ok (note that `;` is needed after `}`)
{ expr; expr; };   // ok (note that `;` is needed after `}`)

expr;              // ok
;                  // syntax error: empty statement is not permitted.

~~~

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

| Example                                                                     | Explanation                                 |
| --------------------------------------------------------------------------- | ------------------------------------------- |
| `var x : type`                                                              | variable declaration                        |
| `let x = expr`                                                              | variable definition                         |
| <code>type Bool = True &#124; False</code>                                  | type definition (enum type `Bool`)          |
| <code>type Suits = Spades &#124; Diamonds &#124; Hearts &#124; Clubs</code> | type definition (enum type `Suits`)         |
| <code>type Color a = RGB a a a &#124; HSV a a a</code>                      | type definition (generic type `Color a`)    |
| <code>type Maybe a = Just a &#124; Nothing</code>                           | type definition (generic type `Maybe a`)    |
| <code>type Result a e = Ok a &#124; Err e</code>                            | type definition (generic type `Result a e`) |
| <code>type List a = Cons a (List a) &#124; Nil</code>                       | type definition (generic type `List a`)     |

## Block, if, match, and lambda expression

| Example                                                   | Explanation                                           |
| --------------------------------------------------------- | ----------------------------------------------------- |
| `{ stmts }`                                               | block (enclosed list of statements)                   |
| `if expr { stmts } else { stmts }`                        | if block                                              |
| `if expr { stmts } else if expr { stmts } else { stmts }` | if block                                              |
| `match expr { pat1 => expr1 ; pat2 => expr2 ... }`        | pattern match                                         |
| <code>&#124;x&#124; expr</code>                           | lambda (i.e. function object, closure)                |
| <code>&#124;x y ...&#124; expr</code>                     | lambda (i.e. function object, closure)                |
| <code>&#124;x&#124; &#124;y&#124; ... expr</code>         | lambda (i.e. function object, closure)                |

## Binary operators

| Operator                  | Associativity     | Example                             | Explanation                         |
| ------------------------- | ----------------- | ----------------------------------- | ----------------------------------- |
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
| `"Hello\nWorld"`  | string                                |

## Basic Type

| Example                                        | Explanation                        |
| -------                                        | ------------                       |
| `()`                                           | unit type                          |
| `bool`                                         | boolean type                       |
| `int`                                          | integer type                       |
| `String`                                       | string type                        |
| <code>&#124;A&#124; B</code>                   | function type (A -> B)             |
| <code>&#124;A B ...&#124; C</code>             | function type (A -> B -> ... -> C) |
| <code>&#124;A&#124; &#124;B&#124; ... C</code> | function type (A -> B -> ... -> C) |
