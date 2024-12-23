#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <map>
#include <shared_mutex>
#include <cstdlib>
#include <chrono>

// Lab Task 1: Multithreading
void printNumbers() {
    for (int i = 1; i <= 10; ++i) {
        std::cout << "Number: " << i << std::endl;
    }
}

void printSquares() {
    for (int i = 1; i <= 10; ++i) {
        std::cout << "Square: " << i * i << std::endl;
    }
}

// Lab Task 2: Thread Synchronization
std::mutex counterMutex;
int sharedCounter = 0;

void incrementCounter() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++sharedCounter;
    }
}

// Lab Task 3: Concurrent Data Structures
std::map<int, int> sharedMap;
std::shared_mutex mapMutex;

void writeToMap(int id) {
    std::unique_lock<std::shared_mutex> lock(mapMutex);
    sharedMap[id] = id * id;
    std::cout << "Thread " << id << " wrote " << id * id << std::endl;
}

void readFromMap(int id) {
    std::shared_lock<std::shared_mutex> lock(mapMutex);
    if (sharedMap.find(id) != sharedMap.end()) {
        std::cout << "Thread " << id << " read " << sharedMap[id] << std::endl;
    } else {
        std::cout << "Thread " << id << " found no entry" << std::endl;
    }
}

// Lab Task 4: Bank Transaction System
std::atomic<int> accountBalance(1000);
std::shared_mutex accountMutex; 

void performTransaction(int id) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::shared_mutex> lock(accountMutex);
        int transaction = (std::rand() % 200) - 100; // Random deposit/withdrawal
        int oldBalance = accountBalance.load();
        accountBalance.fetch_add(transaction);
        std::cout << "Thread " << id << " performed transaction: " << transaction
                  << ", Old Balance: " << oldBalance
                  << ", New Balance: " << accountBalance.load() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Task 1: Multithreading
    std::thread t1(printNumbers);
    std::thread t2(printSquares);
    t1.join();
    t2.join();

    // Task 2: Thread Synchronization
    std::thread t3(incrementCounter);
    std::thread t4(incrementCounter);
    std::thread t5(incrementCounter);
    t3.join();
    t4.join();
    t5.join();
    std::cout << "Final Counter Value: " << sharedCounter << std::endl;

    // Task 3: Concurrent Data Structures
    std::vector<std::thread> mapThreads;
    for (int i = 0; i < 5; ++i) {
        mapThreads.emplace_back(writeToMap, i);
    }
    for (int i = 0; i < 5; ++i) {
        mapThreads.emplace_back(readFromMap, i);
    }
    for (auto &t : mapThreads) {
        t.join();
    }

    // Task 4: Bank Transaction System
    std::vector<std::thread> transactionThreads;
    for (int i = 0; i < 5; ++i) {
        transactionThreads.emplace_back(performTransaction, i);
    }
    for (auto &t : transactionThreads) {
        t.join();
    }
    std::cout << "Final Account Balance: " << accountBalance.load() << std::endl;

    return 0;
}
