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

static void thread_with_lambda(int reps);
static void thread_with_functor();

void basic_thread_test() {
    
    thread_with_functor();
    thread_with_lambda(3);
}

static void thread_with_lambda(int reps) {
    
    auto main_thread = std::this_thread::get_id();
    
    auto lambda = [main_thread, reps](std::string name) {
        std::cout << name << " thread id : " << std::this_thread::get_id() << " (main ID : " << main_thread << ")" << std::endl;
        for (auto _ = reps; _--;) {
            std::cout << name << " starting sleeping" << std::endl;
            std::this_thread::sleep_for(1s);
            std::cout << name << " finished sleeping" << std::endl;
        }
    };
    
    std::thread t1(lambda, "Background");
    lambda("Main");
    t1.join();
}

static void thread_with_functor() {
    
    struct functor {
        
        functor(std::string name = "Anon") : name(name) {}
        
        void operator()(int count) {
            
            std::cout << name << " thread id : " << std::this_thread::get_id() << std::endl;

            for (auto i = count; i--;) { std::cout << "Thread using function object as callable: " << i << " of " << count << std::endl; }
        }
        
    private:
        std::string     name;
    };
    
    std::thread t1(functor("Threaded"), 5);
    functor()(3);
    t1.join();
}
