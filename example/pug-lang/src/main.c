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
  PARSE_RESULT(Expr) result = parse(stmt(), input);
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
  Context* ctx = trait(Context).create(32);
  EvalResult result2 = trait(Interpreter(Expr)).eval(ctx, result.ok);
  {
    if (!result2.success) {
      eprintf(BOLD RED, "runtime error: ");
      printf("%s\n\n", result2.err.msg);
      return false;
    }
    eprintf(BOLD GREEN, "%d\n\n", result2.ok);
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
  assert(!pug_parseTest("(12*3)(3*4)")); /* error */
  assert(!pug_parseTest("(12*3a)"));     /* error */
  assert(pug_parseTest("5+4*3-2/1"));
  assert(pug_parseTest("( 5 + 4 ) * 3 - 2 / 1  "));
  assert(pug_parseTest("-1"));
  assert(pug_parseTest("+1"));
  assert(pug_parseTest("- 1"));
  assert(pug_parseTest("+ 1"));
  assert(pug_parseTest("1 - -1"));
  assert(!pug_parseTest("9999999999999999999")); /* error */
  assert(!pug_parseTest("1 / 0"));               /* division by zero */
  assert(!pug_parseTest("1 % 0"));               /* division by zero */
  assert(pug_parseTest("10 % 3"));
  assert(pug_parseTest("10 == 3"));
  assert(pug_parseTest("10 == 10"));
  assert(pug_parseTest("10 != 3"));
  assert(pug_parseTest("10 != 10"));
  assert(pug_parseTest("1 <= 10")); /* 1 */
  assert(pug_parseTest("1 < 10"));  /* 1 */
  assert(pug_parseTest("1 > 10"));  /* 0 */
  assert(pug_parseTest("1 >= 10")); /* 0 */

  assert(pug_parseTest("a"));         /* 0 */
  assert(pug_parseTest("a = b = 1")); /* 1 */
}
