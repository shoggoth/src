//
//  constexprs.cpp
//  modern
//
//  Created by Richard Henry on 15/02/2023.
//

#include "constexprs.hpp"
#include <iostream>

#define LEN 10

int len_foo() {
    int i = 2;
    return i;
}

constexpr int len_foo_constexpr() {
    return 5;
}

constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n-1) + fibonacci(n-2);
}

// C++11 provides constexpr to let the user explicitly declare that the function or object constructor will become a constant expression at compile time.

int constexprs() {
    
    char arr_1[10];                      // legal
    char arr_2[LEN];                     // legal

    int len = 10;
    // char arr_3[len];                  // illegal

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;
    // char arr_4[len_2];                // illegal, but ok for most of the compilers
    char arr_4[len_2_constexpr];         // legal

    // char arr_5[len_foo()+5];          // illegal
    char arr_6[len_foo_constexpr() + 1]; // legal

    // 1, 1, 2, 3, 5, 8, 13, 21, 34, 55
    std::cout << fibonacci(10) << std::endl;

    char arr_7[fibonacci(9)];
    
    constexpr int len_8 = 1 + 2 + 7;
    
    char arr_8[len_8];
    
    return 0;
}
