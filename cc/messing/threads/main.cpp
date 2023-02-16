//
//  main.cpp
//  threads
//
//  Created by Richard Henry on 16/02/2023.
//

#include "timing.hpp"

// Multithreading support was introduced in C++11

int main(int argc, const char * argv[]) {
    
    timing_test();
    timing_test_steady();

    return 0;
}
