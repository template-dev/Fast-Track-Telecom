#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int sharedData = 0;

void writerThread() {
    std::lock_guard<std::mutex> _(mtx);
    for (int i = 1; i <= 5; ++i) {
      sharedData = i;
      std::cout << "Writer wrote: " << sharedData << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void readerThread() {
    for (int i = 0; i < 5; ++i) {
      std::cout << "Reader read: " << sharedData << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::thread writer(writerThread);
    std::thread reader(readerThread);

    writer.join();
    reader.join();

    return 0;
}
