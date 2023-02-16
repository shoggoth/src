//
//  timing.cpp
//  threads
//
//  Created by Richard Henry on 16/02/2023.
//

#include "timing.hpp"
#include <iostream>
#include <thread>
#include <chrono>

static void timing_test() {
    
    using namespace std::chrono;
    auto t = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    auto u = system_clock::now().time_since_epoch().count();
    std::cout << "Hello, World! at " << t.count() << " - " << u << "\n";
    
    // From C++14 using std and also its numeric literals:
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
    
    auto v = system_clock::now().time_since_epoch().count();
    std::cout << "Goodbye, cruel World! at " << v << " - " << v - u << "\n";
}

static void timing_test_steady() {
    
    using namespace std::chrono;
    auto t = duration_cast<nanoseconds>(steady_clock::now().time_since_epoch());
    auto u = steady_clock::now().time_since_epoch().count();
    std::cout << "Hello, steady World! at " << t.count() << " - " << u << "\n";
    
    // From C++14 using std and also its numeric literals:
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    
    auto v = steady_clock::now().time_since_epoch().count();
    std::cout << "Goodbye, cruel (but steady) World! at " << v << " - " << v - u << "\n";
}

void timing_tests() {
    
    timing_test();
    timing_test_steady();
}

// MARK: FPS

// Desired frame rate
typedef std::chrono::duration<int, std::ratio<1, 60>> frame_duration;

void update() {
    
    // https://stackoverflow.com/questions/20375140/c11-threads-sleep-for-a-remaining-time

    // Get start time
    auto start_time = std::chrono::steady_clock::now();
    // Get end time
    auto end_time = start_time + frame_duration(1);
    // Here happens the actual update stuff

    // Sleep if necessary
    std::this_thread::sleep_until(end_time);
}
