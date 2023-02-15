//
//  main.cpp
//  link_c
//
//  Created by Richard Henry on 15/02/2023.
//

#include "foo.h"
#include <iostream>
#include <functional>

int main() {
    
    std::cout << "Result from C code: " << add(1, 2) << "." << std::endl;
    
    // TODO: Learn how to use the ref here in the lambda.
    [out = std::ref(std::cout << "Result from C code: " << add(1, 2))]() { out.get() << ".." << std::endl; }();
    
    return 0;
}
