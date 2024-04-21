#ifndef FAST_TRACK_TELECOM_TOKENS_HPP
#define FAST_TRACK_TELECOM_TOKENS_HPP

#include <string>

struct OpeningBracket {};
struct ClosingBracket {};
struct Number { int value; };
struct UnknownToken { std::string value; };
struct MinToken {};
struct AbsToken {};
struct Plus {};
struct Minus {};
struct Multiply {};
struct Modulo {};
struct Divide {};
struct Max {};
struct Sqr {};

#endif //FAST_TRACK_TELECOM_TOKENS_HPP
