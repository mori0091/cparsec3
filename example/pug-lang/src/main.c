/* -*- coding: utf-8-unix -*- */

#define CPARSEC_CONFIG_IMPLEMENT /* generate parsec code if defined */

#include "interpreter/interpreter.h"
#include "parser/stmt.h"

struct Options {
  bool use_color;
  bool self_test;
  CharBuff script;
} options;

struct Options pug_getopts(int argc, char** argv);
void pug_help(String argv0);
bool pug_parseTest(String input);
void pug_self_test(void);

// -----------------------------------------------------------------------
int main(int argc, char** argv) {
  options = pug_getopts(argc, argv);
  if (options.self_test) {
    pug_self_test();
  }
  if (options.script.length) {
    return !pug_parseTest(options.script.data);
  }

  return 0;
}

// -----------------------------------------------------------------------
#ifdef _WIN32
/* for Windows platform (including Mingw) */
static bool is_ANSI_escape_available(void) {
  return false;
}
#else
/* for Other platform (including MSYS) */
#include <unistd.h>
static bool is_ANSI_escape_available(void) {
  return isatty(fileno(stdout));
}
#endif

#define BOLD ";1"
#define DIM ";2"
#define UNDERLINE ";4"
#define BLACK ";30"
#define RED ";31"
#define GREEN ";32"
#define YELLOW ";33"
#define BLUE ";34"
#define MAGENTA ";35"
#define CYAN ";36"
#define WHITE ";37"

static void eprintf(const char* restrict esc, const char* restrict fmt,
                    ...) {
  va_list ap;
  va_start(ap, fmt);
  if (!options.use_color) {
    vprintf(fmt, ap);
  } else {
    printf("\033[0%sm", esc);
    vprintf(fmt, ap);
    printf("\033[0m");
  }
  va_end(ap);
}

// -----------------------------------------------------------------------
#include <libgen.h>

struct Options pug_getopts(int argc, char** argv) {
  char* arg0 = basename(argv[0]);
  if (argc < 2) {
    pug_help(arg0);
    exit(0);
  }
  struct Options opt = {0};
  /* "--color=auto" in default */
  opt.use_color = is_ANSI_escape_available();
  for (int i = 1; i < argc; ++i) {
    if (g_eq("-h", argv[i]) || g_eq("--help", argv[i])) {
      pug_help(arg0);
      exit(0);
    } else if (g_eq("--self-test", argv[i])) {
      opt.self_test = true;
    } else if (g_eq("--color", argv[i]) ||
               g_eq("--color=always", argv[i])) {
      opt.use_color = true;
    } else if (g_eq("--color=auto", argv[i])) {
      opt.use_color = is_ANSI_escape_available();
    } else if (g_eq("--color=never", argv[i])) {
      opt.use_color = false;
    } else if (g_eq("-e", argv[i])) {
      if (!argv[i + 1]) {
        fprintf(stderr, "%s: option requires an argument -- '%s'\n", arg0,
                argv[i]);
        exit(2);
      }
      mem_printf(&opt.script, "%s", argv[++i]);
    } else {
      fprintf(stderr, "%s: invalid option -- '%s'\n", arg0, argv[i]);
      exit(2);
    }
  }
  return opt;
}

// -----------------------------------------------------------------------
void pug_help(String arg0) {
  int n = snprintf(0, 0, "%s", arg0);
  printf("SYNOPSIS\n");
  printf("    %*s [-h] [--help]\n", n, arg0);
  printf("    %*s [--self-test]\n", n, "");
  printf("    %*s [--color[=WHEN]]\n", n, "");
  printf("    %*s [-e SCRIPT]...\n", n, "");
  printf("\n");
  printf("OPTIONS\n");
  printf("    -h, --help\n");
  printf("            display this help and exit.\n");
  printf("        --self-test\n");
  printf("            execute self-tests.\n");
  printf("        --color[=WHEN]\n");
  printf("            colorize the output; \n");
  printf("            WHEN can be 'always' (default if omitted), \n");
  printf("            'auto', or 'never'.\n");
  printf("    -e SCRIPT\n");
  printf("            add the SCRIPT to the commands to be executed.\n");
  printf("\n");
}

// -----------------------------------------------------------------------
#define PARSE_RESULT(T) ParseResult(CPARSEC_STREAM_TYPE, T)

bool pug_parseTest(String input) {
  // Establishes a "scoped" memory allocation context. All memory
  // allocated within this scope (by calling `mem_malloc()` explicitly or
  // implicitly) will be deallocated automatically when leaving this
  // scope.
  g_scoped(MemCtx) _ = mem_ctx_begin();

  printf("> %s\n", input);

  // parse the source code and translate it to AST (Abstract Syntax Tree)
  PARSE_RESULT(Expr) result = parse(program(), input);
  {
    Show(PARSE_RESULT(Expr)) s = trait(Show(PARSE_RESULT(Expr)));
    if (!result.success) {
      eprintf(BOLD RED, "syntax error:");
      printf("%s\n\n", s.show(result));
      return false;
    }
    // show the AST
    eprintf(BOLD CYAN, ">> %s\n", s.show(result));
  }

  // evaluate the AST
  Context* ctx = trait(Context).create();
  EvalResult result2 = trait(Interpreter(Expr)).eval(ctx, result.ok);
  {
    if (!result2.success) {
      eprintf(BOLD RED, "runtime error: ");
      printf("%s\n\n", result2.err.msg);
      return false;
    }
    eprintf(BOLD GREEN, "%s\n\n", trait(Show(Expr)).show(result2.ok));
    return true;
  }
}

// -----------------------------------------------------------------------
void pug_self_test(void) {
  assert(pug_parseTest("123"));
  assert(pug_parseTest("12*3"));
  assert(pug_parseTest("12/3"));
  assert(pug_parseTest("(12*3)/4"));
  assert(pug_parseTest("(12/3)*4"));
  assert(pug_parseTest("12*(3/4)"));
  assert(pug_parseTest("12/(3*4)"));
  assert(pug_parseTest("(12*3)/(3*4)"));
  assert(pug_parseTest("(12*3)/(34)"));
  assert(!pug_parseTest("(12*3)(3*4)")); /* syntax error */
  assert(!pug_parseTest("(12*3a)"));     /* syntax error */
  assert(pug_parseTest("5+4*3-2/1"));
  assert(pug_parseTest("( 5 + 4 ) * 3 - 2 / 1   "));
  assert(pug_parseTest("-1"));
  assert(pug_parseTest("+1"));
  assert(pug_parseTest("- 1"));
  assert(pug_parseTest("+ 1"));
  assert(pug_parseTest("1 - -1"));
  assert(!pug_parseTest("9999999999999999999")); /* syntax error */
  assert(!pug_parseTest("1 / 0"));               /* division by zero */
  assert(!pug_parseTest("1 % 0"));               /* division by zero */
  assert(pug_parseTest("10 % 3"));
  assert(pug_parseTest("10 == 3"));  /* false */
  assert(pug_parseTest("10 == 10")); /* true */
  assert(pug_parseTest("10 != 3"));  /* true */
  assert(pug_parseTest("10 != 10")); /* false */
  assert(pug_parseTest("1 <= 10"));  /* true */
  assert(pug_parseTest("1 < 10"));   /* true */
  assert(pug_parseTest("1 > 10"));   /* false */
  assert(pug_parseTest("1 >= 10"));  /* false */
  assert(pug_parseTest("!0 == -1"));
  assert(pug_parseTest("!1 == -2"));
  assert(pug_parseTest("!true == false"));
  assert(pug_parseTest("!false == true"));

  /* a variable must be initialized when defining it. */
  assert(pug_parseTest("let a = 100")); /* 100 */

  /* defining the same variable, the previous one will be shadowed. */
  assert(pug_parseTest("let a = 100; let a = 2")); /* 2 */

  /* defining the same variable of different type is permitted. the
   * previous one will be shadowed. */
  assert(pug_parseTest("let a = 100; let a = true")); /* true */

  /* evaluating a variable results its value. */
  assert(pug_parseTest("let a = 100; a")); /* 100 */

  /* evaluating an undefined variable is not permitted. */
  assert(!pug_parseTest("a"));         /* Undefined variable */
  assert(!pug_parseTest("let a = a")); /* Undefined variable */

  /* assignment expression results the value assigned. */
  assert(pug_parseTest("let a = 1; a = 100")); /* 100 */

  /* assignment of different type is not permitted. */
  assert(!pug_parseTest("let a = 1; a = ()")); /* Type mismatch */

  /* comparison operators results `true` or `false` */
  assert(pug_parseTest("1 == 1"));                     /* true */
  assert(pug_parseTest("0 == 1"));                     /* false */
  assert(pug_parseTest("let a = 0 < 1; a == true "));  /* true */
  assert(pug_parseTest("let a = 1 < 1; a == false ")); /* true */

  /* `!expr` results the complement of `expr` if `expr` was integer. */
  assert(pug_parseTest("!0 ")); /* -1 */
  assert(pug_parseTest("!1 ")); /* -2 */

  /* `!expr` results the logical `not` of `expr` if `expr` was bool. */
  assert(pug_parseTest("!true "));  /* false */
  assert(pug_parseTest("!false ")); /* true */

  /* arithmetic operators are left-associative. */
  assert(pug_parseTest("100 + 2 + 3 == (100 + 2) + 3"));
  assert(pug_parseTest("100 - 2 - 3 == (100 - 2) - 3"));
  assert(pug_parseTest("100 * 2 * 3 == (100 * 2) * 3"));
  assert(pug_parseTest("100 / 2 / 5 == (100 / 2) / 5"));

  /* comparison operators are non-associative */
  assert(!pug_parseTest("1 == 2 == 0")); /* syntax error */
  assert(!pug_parseTest("1 != 2 == 1")); /* syntax error */
  assert(!pug_parseTest("2 != 2 == 0")); /* syntax error */
  assert(!pug_parseTest("2 == 2 == 1")); /* syntax error */
  assert(!pug_parseTest("1 <= 2 <= 3")); /* syntax error */
  assert(!pug_parseTest("1 <= 2 < 3"));  /* syntax error */
  assert(!pug_parseTest("1 < 2 <= 3"));  /* syntax error */
  assert(!pug_parseTest("1 < 2 < 3"));   /* syntax error */
  assert(!pug_parseTest("3 >= 2 >= 1")); /* syntax error */
  assert(!pug_parseTest("3 >= 2 > 1"));  /* syntax error */
  assert(!pug_parseTest("3 > 2 >= 1"));  /* syntax error */
  assert(!pug_parseTest("3 > 2 > 1"));   /* syntax error */

  /* assignment operators are non-associative. */
  assert(!pug_parseTest("let a = 0; let b = 0; a = b = 1"));
  /* -> syntax error */

  /* a statement must be declaration, block, or expression. */
  assert(pug_parseTest("let a = 1")); /* 1 */
  assert(pug_parseTest("{1}"));       /* 1 */
  assert(pug_parseTest("1"));         /* 1 */

  /* empty block is not permitted. */
  assert(!pug_parseTest("{}")); /* syntax error */

  /* empty statement is not permitted. */
  assert(!pug_parseTest(";")); /* syntax error */

  /* each statement in a list of statements must be separated by ";". */
  assert(!pug_parseTest("1 2"));   /* syntax error */
  assert(!pug_parseTest("1 {2}")); /* syntax error */
  assert(!pug_parseTest("{1} 2")); /* syntax error */

  /* a list of statements may ends with an optional ";". */
  assert(pug_parseTest("1;"));    /* 1 */
  assert(pug_parseTest("{1;}"));  /* 1 */
  assert(pug_parseTest("{1};"));  /* 1 */
  assert(pug_parseTest("{1;};")); /* 1 */

  /* a list of statements are evaluated in order, and results the last
   * value. */
  assert(pug_parseTest("1;2"));      /* 2 */
  assert(pug_parseTest("1;2;3"));    /* 3 */
  assert(pug_parseTest("{1;2}"));    /* 2 */
  assert(pug_parseTest("{1;2;3}"));  /* 3 */
  assert(pug_parseTest("{1}"));      /* 1 */
  assert(pug_parseTest("{1}; 2"));   /* 2 */
  assert(pug_parseTest("1; {2}"));   /* 2 */
  assert(pug_parseTest("{1}; {2}")); /* 2 */

  assert(pug_parseTest("let a = 1; let b = 2; a + b")); /* 3 */
  assert(pug_parseTest("let a = 1; let b = 2; a"));     /* 1 */
  assert(pug_parseTest("let a = 1; let b = 2; b"));     /* 2 */
}
