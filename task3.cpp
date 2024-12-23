#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <shared_mutex>
#include <cstdlib>


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

int main() {
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

    return 0;
}
