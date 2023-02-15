//
//  nulls.cpp
//  modern
//
//  Created by Richard Henry on 15/02/2023.
//

#include "nulls.hpp"
#include <iostream>
#include <type_traits>

void foo(char *);
void foo(int);

int nulls() {
    
    if (std::is_same<decltype(NULL), decltype(0)>::value)
        std::cout << "NULL == 0" << std::endl;
    if (std::is_same<decltype(NULL), decltype((void *)0)>::value)
        std::cout << "NULL == (void *)0" << std::endl;
    if (std::is_same<decltype(NULL), std::nullptr_t>::value)
        std::cout << "NULL == nullptr" << std::endl;
    
    foo(0);          // Will call foo(int)
    // foo(NULL);    // Doesn't compile
    foo(nullptr);    // Will call foo(char*)
    
    return 0;
}

void foo(char *) { std::cout << "foo(char *) is called" << std::endl; }

void foo(int i) { std::cout << "foo(int) is called" << std::endl; }
