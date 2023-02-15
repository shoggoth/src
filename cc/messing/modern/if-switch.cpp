//
//  if-switch.cpp
//  modern
//
//  Created by Richard Henry on 15/02/2023.
//

#include "if-switch.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

void if_switch() {
    
    std::vector<int> vec = {1, 2, 3, 4};

    // since c++17, can be simplified by using `auto`
    const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
    if (itr != vec.end()) {
        *itr = 3;
    }

    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 3);
        itr != vec.end()) {
        *itr = 4;
    }

    // should output: 1, 4, 3, 4. can be simplified using `auto`
    for (std::vector<int>::iterator element = vec.begin(); element != vec.end();
        ++element)
        std::cout << *element << std::endl;
}
