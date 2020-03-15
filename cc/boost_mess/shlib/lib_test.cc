#include "lib_test.h"

#include <iostream>
#include <boost/thread.hpp>

void test_throw() {

	throw std::string{ "Exception string from test_throw()." };
}

void exc_test() {

	std::cout << "In the dylib. Hello!" << std::endl;

	try { test_throw(); }

	catch(std::string ex) { std::cout << "Caught (local) : " << ex << std::endl; }

	test_throw();
}

int TestClass::printFoo() {

	std::cout << "printFoo reports foo as " << foo << std::endl;

	return foo;
}

void TestClass::startWorker() {

	int sum = 0;

	new boost::thread([&sum] {

		sum += 23;
	});
}
