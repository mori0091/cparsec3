/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

void test1(void) {
  Array(int) a = g_array(int, 1, 2, 3, 4, 5);
  printf("length(a) = %zu\n", g_length(a));
  for (int* it = g_begin(a); it != g_end(a); it++) {
    printf("%d ", *it);
  }
  printf("\n");
  g_free(a);
}

void test2(void) {
  Array(String) a = g_array(String, "a", "b", "c");
  printf("length(a) = %zu\n", g_length(a));
  for (String* it = g_begin(a); it != g_end(a); it++) {
    printf("\"%s\" ", *it);
  }
  printf("\n");
  g_free(a);
}

void test3(void) {
  List(int) xs = g_list(int, 1, 2, 3, 4, 5, 6);
  printf("length(xs) = %zu\n", g_length(xs));
  for (List(int) ys = xs; !g_null(ys); ys = g_tail(ys)) {
    printf("%d ", g_head(ys));
  }
  printf("\n");
  g_free(xs);
}

void test4(void) {
  List(String) xs = g_list(String, "a", "b", "c");
  printf("length(xs) = %zu\n", g_length(xs));
  for (List(String) ys = xs; !g_null(ys); ys = g_tail(ys)) {
    printf("\"%s\" ", g_head(ys));
  }
  printf("\n");
  g_free(xs);
}

int main(void) {
  test1();
  test2();
  test3();
  test4();
  return 0;
}
