#include <stdio.h>

static int product(const int a, const int b) {

	return a / (1.0 / b);
}

int main(const int argc, const char **argv) {

	int a = 7, b = 8;

	printf("%d * %d = %d\n", a, b, product(a, b));

	return 0;
}
