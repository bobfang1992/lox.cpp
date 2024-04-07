//
// Created by Bob Fang on 3/27/24.
//

#ifndef LOX_TOKEN_H
#define LOX_TOKEN_H

#include <any>
#include <string>
#include <utility>
#include <vector>

#include "TokeyType.h"

namespace Lox {
class Token {
  TokenType type;
  std::string lexeme;
  std::any literal;
  int line;

public:
  Token(TokenType type, std::string lexeme, int line)
      : type(type), lexeme(std::move(lexeme)), line(line) {}

  Token(TokenType type, std::string lexeme, std::any literal, int line)
      : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)),
        line(line) {}

  [[nodiscard]] TokenType getType() const { return type; }

  [[nodiscard]] std::string getLexeme() const { return lexeme; }

  [[nodiscard]] int getLine() const { return line; }

  [[nodiscard]] double getNumber() const {
    return std::any_cast<double>(literal);
  }

  [[nodiscard]] std::string getString() const {
    return std::any_cast<std::string>(literal);
  }
};

inline std::string to_string(const Token &token) {
  return std::string("Token{type: ") + to_string(token.getType()) +
         ", lexme: " + token.getLexeme() +
         ", line: " + std::to_string(token.getLine()) + "}";
}

inline std::string to_string(const std::vector<Token> &tokens) {
  std::string result;
  for (const auto &token : tokens) {
    result += to_string(token) + "\n";
  }
  return result;
}
} // namespace Lox

#endif // LOX_TOKEN_H
