#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>

std::mutex counterMutex;
int sharedCounter = 0;

void incrementCounter() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++sharedCounter;
    }
}

int main() {
    std::thread t3(incrementCounter);
    std::thread t4(incrementCounter);
    std::thread t5(incrementCounter);
    t3.join();
    t4.join();
    t5.join();
    std::cout << "Final Counter Value: " << sharedCounter << std::endl;

    return 0;
}
