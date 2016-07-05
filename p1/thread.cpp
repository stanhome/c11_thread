#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <thread>

void thread_task() {
    std::cout << "[thread_task] hello thread" << std::endl;
}


int main(int argc, const char *argv[]) {
    std::cout << "main begin" << std::endl;
    std::thread t(thread_task);
    std::cout << "after t instance" << std::endl;
    t.join();
    std::cout << "after t join" << std::endl;

    return EXIT_SUCCESS;
}