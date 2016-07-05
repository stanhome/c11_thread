//
// Created by stan on 16-7-5.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>       // std::chrono::milliseconds

#define PRINT_SEPARATOR(_title_) std::cout << "==== " _title_ " ==== " << std::endl

/******** Test 1 mutex::try_lock ********************/

volatile int counter(0);    // non-atomic counter.
std::mutex mtx;             //locks access to counter

void attempt_10k_increases() {
    for (int i = 0; i < 10000; ++i) {
        if (mtx.try_lock()) {
            // only increase if currently not locked.
            ++counter;
            mtx.unlock();
        }
    }
}

void test1()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i < 10; ++i) {
        threads[i]  = std::thread(attempt_10k_increases);
    }

    for (auto &th : threads)
        th.join();

    std::cout << counter << " successful increase of the counter.\n";
}

/******** Test 2 timed_mutex::try_lock_for ********************/

std::timed_mutex tmtx;

void fireworks () {
    // waiting to get a lock: each thread prints "-" every 200ms:
    while (!tmtx.try_lock_for(std::chrono::milliseconds(200))) {
        std::cout << "-";
    }

    // got a lock! - wait for 1s, then this thread prints "*"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "*\n";
    tmtx.unlock();
}

void test2()
{
    std::thread threads[7];
    // spawn 10 threads:
    for (int i = 0; i < 7; ++i) {
        threads[i] = std::thread(fireworks);
    }

    for (auto &th : threads)
        th.join();
}

/******** Test 3 lock_guard ********************/
std::mutex mtx3;
void print_even(int x) {
    if (x % 2 == 0)
        std::cout << "thread #" << x << "\n";
    else
        throw (std::logic_error("not even"));
}

void print_thread_id(int id) {
    try {
        //using a local lock_guard to lock mtx3 guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lck(mtx3);
        print_even(id);
    } catch (std::logic_error &) {
        std::cout << "[exception caught]\n";
    }
}

void test3()
{
    std::thread threads[10];
    // spawn 10 threads.
    for (int i = 0; i < 10; ++i) {
        threads[i] = std::thread(print_thread_id, i + 1);
    }

    for (auto &th : threads)
        th.join();
}

/******** Test 4 unique_lock ********************/
std::mutex mtx4;
void print_block (int n, char c) {
    // critical section (exclusive access to std::cout signaled by lifetime of lck):
    std::unique_lock<std::mutex> lck(mtx);
    for (int i = 0; i < n; ++i) {
        std::cout << c;
    }
    std::cout << '\n';
}

void test4()
{
    std::thread th1(print_block, 100, '*');
    std::thread th2(print_block, 100, '$');

    th1.join();
    th2.join();
}


int main(int argc, const char *argv[]) {
    PRINT_SEPARATOR("Test 1: mutex::try_lock");
    test1();

    PRINT_SEPARATOR("Test 2 timed_mutex::try_lock_for");
//    test2();

    PRINT_SEPARATOR("Test 3 lock_guard ");
    test3();

    PRINT_SEPARATOR("Test 4 unique_lock");
    test4();

    return EXIT_SUCCESS;
}