#include <iostream>
#include "gumpert.h"

int main(const int argc, const char **argv) {

	std::cout << "Hello, world" << std::endl;

	const gumpert * const gum = new gumpert();
	gumpert gum2;
	gumpert gum3(42);

	gum2.pub = 9;

	std::cout << "gum.test returned " << gum->test(3) << std::endl;
	std::cout << "gum.test returned " << gum2.test(3) << std::endl;

	return 0;
}
