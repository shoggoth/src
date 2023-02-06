#include <stdio.h>

static void print_args(int count, char **args);

int main(int argc, char **argv) {
  
  printf("Content-Type: text/html;charset=utf-8\n\n");

  print_args(argc, argv);

  return 0;
}

static void print_args(int count, char **args) {

  printf("<h2>Supplied Arguments</h2>\n");
  printf("count: %d\n", count);
  printf("<ol>\n");
  for (int i = 0; i < count; ++i) {
	printf("<li>%s</li>\n", args[i]);
  }
  printf("</ol>\n");
}
