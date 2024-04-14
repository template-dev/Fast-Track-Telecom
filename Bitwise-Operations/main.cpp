#include <iostream>
#include <bitset>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>

uint8_t Inversion(const uint8_t *begin, const uint8_t *end, uint8_t data, const uint8_t &mask) {
  if (begin == end) {
    return 0;
  }
  
  for (; begin != end; ++begin) {
    data ^= mask;
  }
  return data;
}

void ParallelInversion(const uint8_t *begin, const uint8_t *end, uint8_t data, const uint8_t &mask, std::promise<uint8_t> && resultPromise) {
  std::mutex mtx;
  if (begin == end) {
    return;
  }
  
  std::lock_guard<std::mutex> lock(mtx);
  for (; begin != end; ++begin) {
    data ^= mask;
  }
  resultPromise.set_value(data);
}

int main(int argc, char *argv[]) {
  uint8_t data = 0b10101010;
  uint8_t mask = (1u << 2);
  
  auto start = std::chrono::high_resolution_clock::now();
  uint8_t result = Inversion(&data, &data + 1, data, mask);
  auto end = std::chrono::high_resolution_clock::now();
  
  std::cout << "Result: " << std::bitset<8>(result) << std::endl;
  std::cout << "Inversion: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "mcs \n";
  
  // ----------
  start = std::chrono::high_resolution_clock::now();
  std::future<uint8_t> fut = std::async(std::launch::async, [&]{
    return Inversion(&data, &data + 1, data, mask);
  });
  end = std::chrono::high_resolution_clock::now();
  std::cout << "\nResult: " << std::bitset<8>(fut.get()) << std::endl;
  std::cout << "Inversion by async: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "mcs \n";
  
  // ----------
  std::promise<uint8_t> prom;
  std::future<uint8_t> futForParallel = prom.get_future();
  
  start = std::chrono::high_resolution_clock::now();
  std::thread parallelInversion(ParallelInversion, &data, &data + 1, data, mask, std::move(prom));
  end = std::chrono::high_resolution_clock::now();
  parallelInversion.join();
  
  std::cout << "\nResult: " << std::bitset<8>(futForParallel.get()) << std::endl;
  std::cout << "ParallelInversion: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "mcs \n";
  
  return 0;
}