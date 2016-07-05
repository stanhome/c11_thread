#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <thread> //std::thread, std::this_thread::sleep_for

#include <chrono> //std::chrono::seconds

#define PRINT_SEPARATOR(_title_) std::cout << "==== " _title_ " ==== " << std::endl

/******** Test 1 ********************/

void thread_task() {
    std::cout << "[thread_task] hello thread" << std::endl;
}


void test1()
{
    std::cout << "test1 begin" << std::endl;
    std::thread t(thread_task);
    std::cout << "after t instance" << std::endl;
    t.join();
    std::cout << "after t join" << std::endl;
}

/******** Test 2 ********************/

void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "=>Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f2(int &n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "=>Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void test2()
{
    int n = 0;
    std::thread t1; // t1 is not a thread.
    std::thread t2(f1, n + 1); //pass by value.
    std::thread t3(f2, std::ref(n)); //pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(), t3 is no longer a thread.

    t2.join();
    t4.join();

    std::cout << "Final value of n is " << n << std::endl;
}

/******** Test 3 ********************/

void thread_task_1d(int n) {
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "=>hello thread " << std::this_thread::get_id() << " paused " << n << " seconds" << std::endl;
}

void test3()
{
    std::thread threads[5];
    std::cout << "Spawning 5 threads...\n";
    for (int i = 0; i < 5; ++i) {
        threads[i] = std::thread(thread_task_1d, i + 1);
    }

    std::cout << "Done spawning threads! Now wait for them to join\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Main thread after watting for 2 seconds.\n";

    for (auto &t : threads) {
        t.join();
    }
    std::cout << "All threads joined." << std::endl;
}

int main(int argc, const char *argv[]) {
    PRINT_SEPARATOR("Test 1");
    test1();

    PRINT_SEPARATOR("Test 2");
    test2();

    PRINT_SEPARATOR("Test 3");
    test3();

    return EXIT_SUCCESS;
}