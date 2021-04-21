#include <iostream>

int main() {

	[] { std::cout << "Hello"; }();


	auto lambda = [] { std::cout << ", World!" << std::endl; };

	lambda();

	return 0;
}
