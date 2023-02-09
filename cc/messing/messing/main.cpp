//
//  main.cpp
//  messing
//
//  Created by Richard Henry on 09/02/2023.
//

#include <iostream>
#include <boost/json.hpp>

#include "lib_test.h"

int main(int argc, const char * argv[]) {
    
    const char *msg = std::getenv("HELLO_MSG");
    std::cout << "Hello, " << (msg ? msg : "World!") << std::endl;
    
    test_out();
    
    return 0;
}
