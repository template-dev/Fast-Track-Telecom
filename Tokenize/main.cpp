#include "parsers.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  std::string input = "1234 + 5 - 10 * max 5";
  auto tokens = Tokenize(input);
  for (const auto &token : tokens) {
    if (std::holds_alternative<Number>(token)) {
      std::cout << "Number: " << std::get<Number>(token).value << std::endl;
    } else if (std::holds_alternative<UnknownToken>(token)) {
      std::cout << "Unknown Token: " << std::get<UnknownToken>(token).value << std::endl;
    } else if (std::holds_alternative<Max>(token)) {
      std::cout << "Max" << std::endl;
    } else if (std::holds_alternative<Sqr>(token)) {
      std::cout << "Sqr" << std::endl;
    } else if (std::holds_alternative<Minus>(token)) {
      std::cout << "Minus" << std::endl;
    } else if (std::holds_alternative<Plus>(token)) {
      std::cout << "Plus" << std::endl;
    } else if (std::holds_alternative<Multiply>(token)) {
      std::cout << "Multiply" << std::endl;
    }
  }
  
  return 0;
}