// KEventsLib.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "types.hpp"
#include "thread_pool.hpp"
#include <conio.h>

void testFunction_1();
void testFunction_2(int i);


int main()
{
    KEvents::ThreadPool pool(10);
    auto f1 = std::bind(testFunction_1);
    auto f2 = std::bind(testFunction_2, 10);

    pool.appendTask(std::move(f1));
    pool.appendTask(std::move(f2));
    _getch();
}


void testFunction_1()
{
    std::cout << "Testing Execution on ThreadID: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "Testing Execution on ThreadID: " << "After wait" << std::endl;
}


void testFunction_2(int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Testing Execution on ThreadID on: " << i << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Testing Execution on ThreadID: " << std::this_thread::get_id() << std::endl;
    //logger->info("Testing execution on thread id int is");
}