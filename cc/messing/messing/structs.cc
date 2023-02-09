//
//  structs.cc
//  messing
//
//  Created by Richard Henry on 09/02/2023.
//

#include "structs.h"

struct Foo {
    
    Foo(int foo = 0) : foo(foo) { }
    ~Foo() { std::cout << this << " destructed" << std::endl; }
    
private:
    int         foo;
};

void structs() {
    
    auto bar = Foo();
    
    std::cout << "in structs()" << std::endl;
}
