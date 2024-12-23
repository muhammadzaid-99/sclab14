#include <iostream>
#include <thread>
#include <cstdlib>

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

int main() {
    std::thread t1(printNumbers);
    std::thread t2(printSquares);
    t1.join();
    t2.join();

    return 0;
}
