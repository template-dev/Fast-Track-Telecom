#include <iostream>
#include <bitset>
#include <chrono>

uint8_t Inversion(uint8_t *begin, uint8_t *end, const uint8_t &data, const uint8_t &mask) {
  if (begin == end) {
    return 0;
  }
  
  uint8_t result = data;
  for (; begin != end; ++begin) {
    result ^= mask;
  }
  return result;
}

int main(int argc, char *argv[]) {
  uint8_t data = 0b10101010;
  uint8_t mask = (1u << 2);
  
  auto start = std::chrono::high_resolution_clock::now();
  uint8_t result = Inversion(&data, &data + 1, data, mask);
  auto end = std::chrono::high_resolution_clock::now();
  
  std::cout << "Result: " << std::bitset<8>(result) << std::endl;
  std::cout << "Inversion: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "mcs \n";
  
  return 0;
}