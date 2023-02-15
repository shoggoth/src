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

    // Should output: 1, 4, 3, 4. can be simplified using `auto`
    for (std::vector<int>::iterator element = vec.begin(); element != vec.end();
        ++element)
        std::cout << *element << std::endl;
    
    // In the above code, we can see that the itr variable is defined in the scope of the entire if_switch(),
    // which causes us to rename the other when a variable need to traverse the entire std::vector again.
    // C++17 eliminates this limitation so that we can do this in if(or switch):
    
    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 4); itr != vec.end()) {
        *itr = 6;
    }
    
    // Should output: 1, 6, 3, 4. Simplified using `auto`
    for (auto element = vec.begin(); element != vec.end(); ++element)
        std::cout << *element << std::endl;
    
}
