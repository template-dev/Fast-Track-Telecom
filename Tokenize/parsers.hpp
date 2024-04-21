#ifndef FAST_TRACK_TELECOM_PARSERS_HPP
#define FAST_TRACK_TELECOM_PARSERS_HPP

#include "tokens.hpp"
#include <vector>
#include <cctype>
#include <variant>
#include <unordered_map>

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken,
              MinToken, AbsToken, Plus, Minus, Multiply, Modulo, Divide, Max, Sqr>;

const std::unordered_map<char, Token> kSymbol2Token{
        {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}}};

int ToDigit(unsigned char symbol) { return symbol - '0'; }

Number ParseNumber(const std::string &input, size_t &pos) {
  int value = 0;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isdigit(symbol)) {
    value = value * 10 + ToDigit(symbol);
    if (pos == input.size() - 1) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  return Number{value};
}

Token ParseName(const std::string& input, size_t& pos) {
  std::string name;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isalpha(symbol)) {
    name.push_back(symbol);
    if (pos == input.size() - 1) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  
  if (name == "max") {
    return Max{};
  } else if (name == "sqr") {
    return Sqr{};
  } else {
    return UnknownToken{name};
  }
}

std::vector<Token> Tokenize(const std::string& input) {
  std::vector<Token> tokens;
  const size_t size = input.size();
  size_t pos = 0;
  while (pos < size) {
    const auto symbol = static_cast<unsigned char>(input[pos]);
    if (std::isspace(symbol)) {
      ++pos;
    } else if (std::isdigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    } else if (std::isalpha(symbol)) {
      tokens.emplace_back(ParseName(input, pos));
    } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
      tokens.emplace_back(it->second);
      ++pos;
    }
    ++pos;
  }
  return tokens;
}

#endif //FAST_TRACK_TELECOM_PARSERS_HPP
