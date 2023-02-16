//
//  basics.cpp
//  threads
//
//  Created by Richard Henry on 16/02/2023.
//

#include "basics.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using namespace std::chrono;
using namespace std::chrono_literals;

void basic_thread_test() {
    
    auto main_thread = std::this_thread::get_id();
    
    auto lambda = [main_thread](std::string name) {
        std::cout << name << " thread id : " << std::this_thread::get_id() << " (main ID : " << main_thread << ")" << std::endl;
        for (auto _ = 3; _--;) {
            std::cout << name << " starting sleeping" << std::endl;
            std::this_thread::sleep_for(1s);
            std::cout << name << " finished sleeping" << std::endl;
        }
    };
    
    std::thread t1(lambda, "Background");
    lambda("Main");
    t1.join();
}
