#include "gumpert.h"
#include <iostream>

gumpert::gumpert() {

	std::cout << "constructed with nowt " << std::endl;

	prv = pub = 1;
}

gumpert::gumpert(const int foo) {

	std::cout << "constructed with " << foo << std::endl;

	prv = foo;
	pub = 1;
}

const int gumpert::test(const int inward) const {

	std::cout << inward << " came in." << std::endl;

	return inward * 23 * prv * pub;
}

constexpr int eleet() { return 31337; }
