//
//  main.cpp
//  messing
//
//  Created by Richard Henry on 09/02/2023.
//

#include <iostream>
#include <boost/json.hpp>

int main(int argc, const char * argv[]) {
    
    const char *msg = std::getenv("HELLO_MSG");
    std::cout << "Hello, " << (msg ? msg : "World!") << std::endl;
    return 0;
}
