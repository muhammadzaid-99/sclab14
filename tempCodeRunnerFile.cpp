#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <shared_mutex>
#include <cstdlib>
#include <chrono>

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
