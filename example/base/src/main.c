/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

void test_Eq(void) {
  assert(g_eq(1, 1));
  assert(g_neq(0, 1));
}

void test_Ord(void) {
  assert(g_le(0, 1));
  assert(g_lt(0, 1));
  assert(g_ge(2, 1));
  assert(g_gt(2, 1));
  assert(g_min(2, 1) == 1);
  assert(g_max(2, 1) == 2);
  assert(g_cmp(0, 1) == -1);
  assert(g_cmp(1, 1) == 0);
  assert(g_cmp(1, 0) == 1);
}

void test_Array1(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("length(a) = %zu\n", g_length(a));
  for (int* it = g_begin(a); it != g_end(a); it++) {
    printf("%d ", *it);
  }
  printf("\n");
  g_free(a);
}

void test_List1(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("length(xs) = %zu\n", g_length(xs));
  for (List(int) ys = xs; !g_null(ys); ys = g_tail(ys)) {
    printf("%d ", g_head(ys));
  }
  printf("\n");
  g_free(xs);
}

void test_Array2(void) {
  printf("a = g_array(String, \"a\", \"b\", \"c\")\n");
  Array(String) a = g_array(String, "a", "b", "c");
  printf("length(a) = %zu\n", g_length(a));
  for (String* it = g_begin(a); it != g_end(a); it++) {
    printf("\"%s\" ", *it);
  }
  printf("\n");
  g_free(a);
}

void test_List2(void) {
  printf("xs = g_list(String, \"a\", \"b\", \"c\")\n");
  List(String) xs = g_list(String, "a", "b", "c");
  printf("length(xs) = %zu\n", g_length(xs));
  for (List(String) ys = xs; !g_null(ys); ys = g_tail(ys)) {
    printf("\"%s\" ", g_head(ys));
  }
  printf("\n");
  g_free(xs);
}

void test_Array3(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("for each x in a, x = 2 * x\n");
  ItrT(Array(int)) A = trait(Itr(Array(int)));
  for (Itr(Array(int)) it = A.itr(a); !A.null(it); it = A.next(it)) {
    A.set(2 * A.get(it), it);
  }
  for (Itr(Array(int)) it = A.itr(a); !A.null(it); it = A.next(it)) {
    printf("%d ", A.get(it));
  }
  printf("\n");
  g_free(a);
}

void test_List3(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("for each x in xs, x = 2 * x\n");
  ItrT(List(int)) I = trait(Itr(List(int)));
  for (Itr(List(int)) it = I.itr(xs); !I.null(it); it = I.next(it)) {
    I.set(2 * I.get(it), it);
  }
  for (Itr(List(int)) it = I.itr(xs); !I.null(it); it = I.next(it)) {
    printf("%d ", I.get(it));
  }
  printf("\n");
  g_free(xs);
}

void test_Array4(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("for each x in a, x = 2 * x\n");
  for (Itr(Array(int)) it = g_itr(a); !g_null(it); it = g_next(it)) {
    g_set(2 * g_get(it), it);
  }
  for (Itr(Array(int)) it = g_itr(a); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");
  g_free(a);
}

void test_List4(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("for each x in xs, x = 2 * x\n");
  for (Itr(List(int)) it = g_itr(xs); !g_null(it); it = g_next(it)) {
    g_set(2 * g_get(it), it);
  }
  for (Itr(List(int)) it = g_itr(xs); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");
  g_free(xs);
}

#if !defined(__GNUC__)

void test_Array5(void) {
}
void test_List5(void) {
}

#else

void test_Array5(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  __auto_type a = g_array(int, 1, 2, 3, 4, 5);
  printf("for each x in a, x = 2 * x\n");
  g_for(it, a) {
    g_set(2 * g_get(it), it);
  }
  g_for(it, a) {
    printf("%d ", g_get(it));
  }
  printf("\n");
  g_free(a);
}

void test_List5(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  __auto_type xs = g_list(int, 1, 2, 3, 4, 5);
  printf("for each x in xs, x = 2 * x\n");
  g_for(it, xs) {
    g_set(2 * g_get(it), it);
  }
  g_for(it, xs) {
    printf("%d ", g_get(it));
  }
  printf("\n");
  g_free(xs);
}

#endif

void test_Array6(void) {
  printf("a = g_array(int, 0, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 0, 1, 2, 3, 4, 5);
  Slice(Array(int)) s;

  printf("s = g_slice(a, 2) ... [0, 2)\n");
  s = g_slice(a, 2);
  for (Itr(Slice(Array(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(a, -2) ... [0, N-2)\n");
  s = g_slice(a, -2);
  for (Itr(Slice(Array(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(a, 1, 3) ... [1, 3)\n");
  s = g_slice(a, 1, 3);
  for (Itr(Slice(Array(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(a, -3, -1) ... [N-3, N-1)\n");
  s = g_slice(a, -3, -1);
  for (Itr(Slice(Array(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(a, 1, -1) ... [1, N-1)\n");
  s = g_slice(a, 1, -1);
  for (Itr(Slice(Array(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List6(void) {
  printf("xs = g_list(int, 0, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 0, 1, 2, 3, 4, 5);
  Slice(List(int)) s;

  printf("s = g_slice(xs, 2) ... [0, 2)\n");
  s = g_slice(xs, 2);
  for (Itr(Slice(List(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(xs, -2) ... [0, N-2)\n");
  s = g_slice(xs, -2);
  for (Itr(Slice(List(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(xs, 1, 3) ... [1, 3)\n");
  s = g_slice(xs, 1, 3);
  for (Itr(Slice(List(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(xs, -3, -1) ... [N-3, N-1)\n");
  s = g_slice(xs, -3, -1);
  for (Itr(Slice(List(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(xs, 1, -1) ... [1, N-1)\n");
  s = g_slice(xs, 1, -1);
  for (Itr(Slice(List(int))) it = g_itr(s); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

#if !defined(__GNUC__)

void test_Array7(void) {
}
void test_List7(void) {
}

#else

void test_Array7(void) {
  printf("a = g_array(int, 0, 1, 2, 3, 4, 5)\n");
  __auto_type a = g_array(int, 0, 1, 2, 3, 4, 5);

  printf("s = g_slice(a, 2, 3)\n");
  __auto_type s = g_slice(a, 2, 3);
  g_for(it, s) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(a, 2)\n");
  s = g_slice(a, 2);
  g_for(it, s) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List7(void) {
  printf("xs = g_list(int, 0, 1, 2, 3, 4, 5)\n");
  __auto_type xs = g_list(int, 0, 1, 2, 3, 4, 5);

  printf("s = g_slice(xs, 2, 3)\n");
  __auto_type s = g_slice(xs, 2, 3);
  g_for(it, s) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  printf("s = g_slice(xs, 2)\n");
  s = g_slice(xs, 2);
  g_for(it, s) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

#endif

void test_Array8(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);

  printf("iterate by using g_skip(2, it) instead of g_next(it)\n");
  for (Itr(Array(int)) it = g_itr(a); !g_null(it); it = g_skip(2, it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List8(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);

  printf("iterate by using g_skip(2, it) instead of g_next(it)\n");
  for (Itr(List(int)) it = g_itr(xs); !g_null(it); it = g_skip(2, it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

#if !defined(__GNUC__)

void test_Array9(void) {
}
void test_List9(void) {
}

#else

void test_Array9(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  __auto_type a = g_array(int, 1, 2, 3, 4, 5);

  printf("iterate by using g_for(it, a, 2) instead of g_for(it, a)\n");
  g_for(it, a, 2) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List9(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  __auto_type xs = g_list(int, 1, 2, 3, 4, 5);

  printf("iterate by using g_for(it, xs, 2) instead of g_for(it, xs)\n");
  g_for(it, xs, 2) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

#endif

void test_Array10(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("iterator Itr(C) is also an iterable\n");
  Itr(Array(int)) i = g_itr(a);
  for (Itr(Array(int)) it = g_itr(i); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List10(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("iterator Itr(C) is also an iterable\n");
  Itr(List(int)) i = g_itr(xs);
  for (Itr(List(int)) it = g_itr(i); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

void test_Array11(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("s = g_slice(a, 1, -1)\n");
  Slice(Array(int)) s = g_slice(a, 1, -1);
  printf("iterator Itr(Slice(C)) is also an iterable\n");
  Itr(Slice(Array(int))) i = g_itr(s);
  for (Itr(Slice(Array(int))) it = g_itr(i); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List11(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("s = g_slice(xs, 1, -1)\n");
  Slice(List(int)) s = g_slice(xs, 1, -1);
  printf("iterator Itr(Slice(C)) is also an iterable\n");
  Itr(Slice(List(int))) i = g_itr(s);
  for (Itr(Slice(List(int))) it = g_itr(i); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

void test_Array12(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("g_reverse(a)\n");
  g_reverse(a);
  printf("a == ");
  for (Itr(Array(int)) it = g_itr(a); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List12(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("g_reverse(xs)\n");
  g_reverse(xs);
  printf("xs == ");
  for (Itr(List(int)) it = g_itr(xs); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

void test_Array13(void) {
  printf("a = g_array(int, 1, 2, 3, 4, 5)\n");
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("s = g_slice(a, 1, -1)\n");
  Slice(Array(int)) s = g_slice(a, 1, -1);
  printf("g_reverse(s)\n");
  g_reverse(s);
  printf("a == ");
  for (Itr(Array(int)) it = g_itr(a); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(a);
}

void test_List13(void) {
  printf("xs = g_list(int, 1, 2, 3, 4, 5)\n");
  List(int) xs = g_list(int, 1, 2, 3, 4, 5);
  printf("s = g_slice(xs, 1, -1)\n");
  Slice(List(int)) s = g_slice(xs, 1, -1);
  printf("g_reverse(s)\n");
  g_reverse(s);
  printf("xs == ");
  for (Itr(List(int)) it = g_itr(xs); !g_null(it); it = g_next(it)) {
    printf("%d ", g_get(it));
  }
  printf("\n");

  g_free(xs);
}

int main(void) {
  test_Eq();
  test_Ord();
  test_Array1();
  test_List1();
  test_Array2();
  test_List2();
  test_Array3();
  test_List3();
  test_Array4();
  test_List4();
  test_Array5();
  test_List5();
  test_Array6();
  test_List6();
  test_Array7();
  test_List7();
  test_Array8();
  test_List8();
  test_Array9();
  test_List9();
  test_Array10();
  test_List10();
  test_Array11();
  test_List11();
  test_Array12();
  test_List12();
  test_Array13();
  test_List13();
  return 0;
}
