//
//  main.cpp
//  cpp_sim
//
//  Created by Richard Henry on 14/02/2023.
//

extern "C" {
#include "space.h"
}
#include <iostream>

int main(int argc, const char * argv[]) {

    sm_space *space = new_space(0, 0, 0);
    
    std::cout << "Hello, C++ World " << space << std::endl;
    
    step_space(space);
    
    return 0;
}
