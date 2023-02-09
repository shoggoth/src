//
//  functor.cc
//  functor
//
//  Created by Richard Henry on 09/02/2023.
//

#include "functor.h"

static void increment_array();

int main(int argc, const char * argv[]) {

    std::cout << "Hello, Functor!" << std::endl;
    
    functors();
    increment_array();
    
    return 0;
}

void functors() {
    
    std::cout << "in functors()" << std::endl;
}

// MARK: -

static int increment(int x) {  return (x + 1); }

class incrementer {

    int inc;
 
public:
    incrementer(int n) : inc(n) { std::cout << "incrementer constructed with " << n << std::endl; }
    
    int operator () (int num) const { return num + inc; }
};

void increment_array() {
    
    int arr[] = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
    
    // Use a function to increment it
    std::transform(arr, arr + std::size(arr), arr, increment);
    // Use a functor to do the same thing
    std::transform(arr, arr + std::size(arr), arr, incrementer(1));
    // Use a lambda to do the same thing
    std::transform(arr, arr + std::size(arr), arr, [](int x) { return x + 1; });

    for (auto i : arr) std::cout << i << ", ";
}
