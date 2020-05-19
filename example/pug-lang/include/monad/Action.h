/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#define Action(S, E, T) TYPE_NAME(Action, S, E, T)
#define ActionResult(S, E, T) TYPE_NAME(ActionResult, S, E, T)
#define ActionRunner(S, E, T) TYPE_NAME(ActionRunner, S, E, T)

// -----------------------------------------------------------------------
#define ActionOk(...) Fn(ActionOkArgs(__VA_ARGS__))
#define typedef_ActionOk(...) typedef_Fn(ActionOkArgs(__VA_ARGS__))
#define ActionOkArgs(S, E, T)                                            \
  T,                        /* return type of Action */                  \
      S,                    /* next input state */                       \
      ActionResult(S, E, T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define ActionErr(...) Fn(ActionErrArgs(__VA_ARGS__))
#define typedef_ActionErr(...) typedef_Fn(ActionErrArgs(__VA_ARGS__))
#define ActionErrArgs(S, E, T)                                           \
  E,                        /* an error occurred */                      \
      S,                    /* next input state */                       \
      ActionResult(S, E, T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define UnAction(...) Fn(UnActionArgs(__VA_ARGS__))
#define typedef_UnAction(...) typedef_Fn_r(UnActionArgs(__VA_ARGS__))
#define UnActionArgs(S, E, T)                                            \
  S,                        /* input state */                            \
      ActionOk(S, E, T),    /* continuation called when 'ok' */          \
      ActionErr(S, E, T),   /* continuation called when 'error' */       \
      ActionResult(S, E, T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define trait_ActionRunner(S, E, T)                                      \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ActionResult(S, E, T) ActionResult(S, E, T);            \
  struct ActionResult(S, E, T) {                                         \
    S state;                                                             \
    bool success;                                                        \
    union {                                                              \
      T ok;                                                              \
      E err;                                                             \
    };                                                                   \
  };                                                                     \
                                                                         \
  typedef_Fn(S, ActionResult(S, E, T));                                  \
  typedef_ActionOk(S, E, T);                                             \
  typedef_ActionErr(S, E, T);                                            \
  typedef_UnAction(S, E, T);                                             \
                                                                         \
  typedef struct Action(S, E, T) Action(S, E, T);                        \
  struct Action(S, E, T) {                                               \
    UnAction(S, E, T) unAction;                                          \
  };                                                                     \
                                                                         \
  typedef struct ActionRunner(S, E, T) ActionRunner(S, E, T);            \
  struct ActionRunner(S, E, T) {                                         \
    ActionResult(S, E, T) (*pRunAction)(Action(S, E, T) act, S state);   \
  };                                                                     \
                                                                         \
  ActionRunner(S, E, T) Trait(ActionRunner(S, E, T));                    \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ActionRunner(S, E, T)                                       \
  C_API_BEGIN                                                            \
                                                                         \
  impl_pRunAction(S, E, T);                                              \
                                                                         \
  ActionRunner(S, E, T) Trait(ActionRunner(S, E, T)) {                   \
    return (ActionRunner(S, E, T)){                                      \
        .pRunAction = FUNC_NAME(pRunAction, S, E, T),                    \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_pRunAction(S, E, T)                                         \
                                                                         \
  fn(FUNC_NAME(actOk, T), ActionOkArgs(S, E, T)) {                       \
    g_bind((a, state), *args);                                           \
    return (ActionResult(S, E, T)){                                      \
        .state = state,                                                  \
        .success = true,                                                 \
        .ok = a,                                                         \
    };                                                                   \
  }                                                                      \
                                                                         \
  fn(FUNC_NAME(actErr, T), ActionErrArgs(S, E, T)) {                     \
    g_bind((e, state), *args);                                           \
    return (ActionResult(S, E, T)){                                      \
        .state = state,                                                  \
        .success = false,                                                \
        .err = e,                                                        \
    };                                                                   \
  }                                                                      \
                                                                         \
  static ActionResult(S, E, T)                                           \
      FUNC_NAME(pRunAction, S, E, T)(Action(S, E, T) act, S s) {         \
    __auto_type ok = FUNC_NAME(actOk, T)();                              \
    __auto_type err = FUNC_NAME(actErr, T)();                            \
    return fn_apply(act.unAction, s, ok, err);                           \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define ActionEnv(...) TYPE_NAME(ActionEnv, __VA_ARGS__)

#define def_action(S, E, name, ...)                                      \
  CAT(def_action, VARIADIC_SIZE(__VA_ARGS__))(S, E, name, __VA_ARGS__)

// -----------------------------------------------------------------------
#define def_action1(S, E, name, R)                                       \
  typedef None ActionEnv(name);                                          \
  typedef_Fn(ActionEnv(name), UnAction(S, E, R));                        \
  static Fn(ActionEnv(name), UnAction(S, E, R))                          \
      FUNC_NAME(impl, name)(void);                                       \
  Action(S, E, R) name(void) {                                           \
    Fn(ActionEnv(name), UnAction(S, E, R)) f = FUNC_NAME(impl, name)();  \
    ActionEnv(name) par = {0};                                           \
    return (Action(S, E, R)){fn_apply(f, par)};                          \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ActionEnv(name), UnActionArgs(S, E, R))

// -----------------------------------------------------------------------
#define def_action2(S, E, name, T1, R)                                   \
  typedef struct {                                                       \
    T1 e1;                                                               \
  } ActionEnv(name);                                                     \
  typedef_Fn(ActionEnv(name), UnAction(S, E, R));                        \
  static Fn(ActionEnv(name), UnAction(S, E, R))                          \
      FUNC_NAME(impl, name)(void);                                       \
  Action(S, E, R) name(T1 x1) {                                          \
    Fn(ActionEnv(name), UnAction(S, E, R)) f = FUNC_NAME(impl, name)();  \
    ActionEnv(name) par = {                                              \
        .e1 = x1,                                                        \
    };                                                                   \
    return (Action(S, E, R)){fn_apply(f, par)};                          \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ActionEnv(name), UnActionArgs(S, E, R))

// -----------------------------------------------------------------------
#define def_action3(S, E, name, T1, T2, R)                               \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
  } ActionEnv(name);                                                     \
  typedef_Fn(ActionEnv(name), UnAction(S, E, R));                        \
  static Fn(ActionEnv(name), UnAction(S, E, R))                          \
      FUNC_NAME(impl, name)(void);                                       \
  Action(S, E, R) name(T1 x1, T2 x2) {                                   \
    Fn(ActionEnv(name), UnAction(S, E, R)) f = FUNC_NAME(impl, name)();  \
    ActionEnv(name) par = {                                              \
        .e1 = x1,                                                        \
        .e2 = x2,                                                        \
    };                                                                   \
    return (Action(S, E, R)){fn_apply(f, par)};                          \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ActionEnv(name), UnActionArgs(S, E, R))

// -----------------------------------------------------------------------
#define def_action4(S, E, name, T1, T2, T3, R)                           \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
  } ActionEnv(name);                                                     \
  typedef_Fn(ActionEnv(name), UnAction(S, E, R));                        \
  static Fn(ActionEnv(name), UnAction(S, E, R))                          \
      FUNC_NAME(impl, name)(void);                                       \
  Action(S, E, R) name(T1 x1, T2 x2, T3 x3) {                            \
    Fn(ActionEnv(name), UnAction(S, E, R)) f = FUNC_NAME(impl, name)();  \
    ActionEnv(name) par = {                                              \
        .e1 = x1,                                                        \
        .e2 = x2,                                                        \
        .e3 = x3,                                                        \
    };                                                                   \
    return (Action(S, E, R)){fn_apply(f, par)};                          \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ActionEnv(name), UnActionArgs(S, E, R))

// -----------------------------------------------------------------------
#define A_UNIQUE_ID_NEW() A_UNIQUE_ID_NEW0(__COUNTER__)
#define A_UNIQUE_ID_NEW0(...) CAT(act_unique_identifier, __VA_ARGS__)

#define A_DO()                                                           \
  g_bind((_par_, _s0_, _ok_, _err_), *args);                             \
  __auto_type _s_ = _s0_;

#define A_WITH(...) g_bind((__VA_ARGS__), _par_);

#define A_DO_WITH(...) A_DO() A_WITH(__VA_ARGS__)

#define A_RUN(...)                                                       \
  CAT(A_RUN, VARIADIC_SIZE(__VA_ARGS__))                                 \
  (A_UNIQUE_ID_NEW(), __VA_ARGS__)

#define A_RUN1(_id_, _act_)                                              \
  __auto_type _id_ = runAction(_act_, _s_);                              \
  _s_ = _id_.state;                                                      \
  do {                                                                   \
    if (!_id_.success) {                                                 \
      return fn_apply(_err_, _id_.err, _s_);                             \
    }                                                                    \
  } while (0)

#define A_RUN2(_id_, _act_, _x_)                                         \
  A_RUN1(_id_, _act_);                                                   \
  __auto_type _x_ = _id_.ok;

#define A_RETURN(_x_) return fn_apply(_ok_, _x_, _s_)

#define A_FAIL(_e_) return fn_apply(_err_, _e_, _s_)

// -----------------------------------------------------------------------
#ifndef A_STATE_TYPE
#define A_STATE_TYPE None
#endif

#ifndef A_ERROR_TYPE
#define A_ERROR_TYPE None
#endif

#ifndef A_RETURN_TYPES
#define A_RETURN_TYPES
#endif

// -----------------------------------------------------------------------
#define CPARSEC_ACTION_RETURN_TYPES CPARSEC_ACTION_RETURN_TYPES0()
#define CPARSEC_ACTION_RETURN_TYPES0()                                   \
  SQUASH(None, A_STATE_TYPE, A_RETURN_TYPES)

#define A_RESULT(T) ActionResult(A_STATE_TYPE, A_ERROR_TYPE, T)

#define ACTION(T) Action(A_STATE_TYPE, A_ERROR_TYPE, T)

#define action(name, ...)                                                \
  def_action(A_STATE_TYPE, A_ERROR_TYPE, name, __VA_ARGS__)

#define TRAIT_ACTIONRUNNER(T)                                            \
  trait(ActionRunner(A_STATE_TYPE, A_ERROR_TYPE, T))

#define GENERIC_ACTIONRUNNER(act)                                        \
  GENERIC(act, ACTION, TRAIT_ACTIONRUNNER, CPARSEC_ACTION_RETURN_TYPES)

#define runAction(act, state)                                            \
  GENERIC_ACTIONRUNNER(act).pRunAction(act, state)

// -----------------------------------------------------------------------
#define trait_Action(S, T) trait_ActionRunner(S, A_ERROR_TYPE, T)
#define impl_Action(S, T) impl_ActionRunner(S, A_ERROR_TYPE, T)

BIND_FOR(trait_Action, A_STATE_TYPE, CPARSEC_ACTION_RETURN_TYPES);

ACTION(A_STATE_TYPE) getState(void);
ACTION(None) putState(A_STATE_TYPE s);
ACTION(None) fail(A_ERROR_TYPE e);

#ifdef CPARSEC_CONFIG_IMPLEMENT

BIND_FOR(impl_Action, A_STATE_TYPE, CPARSEC_ACTION_RETURN_TYPES);

action(getState, A_STATE_TYPE) {
  A_DO() {
    A_RETURN(_s_);
  }
}

action(putState, A_STATE_TYPE, None) {
  A_DO_WITH(s) {
    _s_ = s;
    A_RETURN((None){0});
  }
}

action(fail, A_ERROR_TYPE, None) {
  A_DO_WITH(e) {
    A_FAIL(e);
  }
}

#endif

// -----------------------------------------------------------------------
// ~~~c
// /* -*- coding: utf-8-unix -*- */
//
// #define A_STATE_TYPE int
// #define A_ERROR_TYPE String
// #define A_RETURN_TYPES int
//
// #define CPARSEC_CONFIG_IMPLEMENT
// #include "monad/Action.h"
//
// // ACTION(int) plus(int x, int y);
// action(plus, int, int, int) {
//   A_DO_WITH(x,y) {
//     A_RUN(getState(), s);
//     A_RUN(putState(s+1));
//     printf("%d: plus(%d, %d)\n", s, x, y);
//     A_RETURN(x+y);
//   }
// }
//
// // ACTION(int) devide(int x, int y);
// action(divide, int, int, int) {
//   A_DO_WITH(x,y) {
//     A_RUN(getState(), s);
//     A_RUN(putState(s+1));
//     printf("%d: div(%d, %d)\n", s, x, y);
//     if (y == 0) {
//       A_FAIL("Division by zero");
//     }
//     A_RETURN(x/y);
//   }
// }
//
// // ACTION(int) sequence(ACTION(int) a, ACTION(int) b);
// action(sequence, ACTION(int), ACTION(int), int) {
//   A_DO_WITH(a, b) {
//     A_RUN(a);
//     A_RUN(b, x);
//     A_RETURN(x);
//   }
// }
//
// void actionTest0(ACTION(int) a) {
//   A_RESULT(int) res = runAction(a, 0);
//   if (res.success) {
//     printf("%d: %d\n", res.state, res.ok);
//   } else {
//     printf("%d: error: %s\n", res.state, res.err);
//   }
// }
//
// void actionTest(void) {
//   ACTION(int) a = plus(10, 20);
//   ACTION(int) b = divide(10, 2);
//   ACTION(int) c = divide(1, 0);
//   actionTest0(a);
//   actionTest0(b);
//   actionTest0(c);
//   actionTest0(sequence(a, sequence(b, c)));
// }
// ~~~
