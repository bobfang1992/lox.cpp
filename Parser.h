//
// Created by Bob Fang on 3/31/24.
//

#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include <vector>

#include "Expr.h"
#include "Lox.h"
#include "Token.h"

namespace Lox {
struct ParserError : public std::exception {
  [[nodiscard]] const char *what() const noexcept override {
    return "Parser error";
  }
};

class Parser {
  std::vector<Token> tokens;
  int current = 0;

  std::shared_ptr<Expr> expression() { return equality(); }

  std::shared_ptr<Expr> equality() {
    auto expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
      auto op = previous();
      auto right = comparison();
      expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
  }

  std::shared_ptr<Expr> comparison() {
    auto expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
      auto op = previous();
      auto right = term();
      expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
  }

  std::shared_ptr<Expr> term() {
    auto expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
      auto op = previous();
      auto right = factor();
      expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
  }

  std::shared_ptr<Expr> factor() {
    auto expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
      auto op = previous();
      auto right = unary();
      expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
  }

  std::shared_ptr<Expr> unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
      auto op = previous();
      auto right = unary();
      return std::make_shared<Unary>(op, right);
    }
    return primary();
  }

  std::shared_ptr<Expr> primary() {
    if (match({TokenType::FALSE}))
      return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE}))
      return std::make_shared<Literal>(true);
    if (match({TokenType::NIL}))
      return std::make_shared<Literal>(nullptr);
    if (match({TokenType::NUMBER})) {
      return std::make_shared<Literal>(std::stod(previous().getLexeme()));
    }
    if (match({TokenType::STRING})) {
      return std::make_shared<Literal>(previous().getLexeme());
    }
    if (match({TokenType::LEFT_PAREN})) {
      auto expr = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_shared<Grouping>(expr);
    }
    throw error(peek(), "Expect expression.");
  }

  bool match(const std::vector<TokenType> &types) {
    for (const auto &type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }
    return false;
  }

  bool check(TokenType type) {
    if (isAtEnd())
      return false;
    return peek().getType() == type;
  }

  Token advance() {
    if (!isAtEnd())
      current++;
    return previous();
  }

  bool isAtEnd() { return peek().getType() == TokenType::EoF; }

  Token peek() { return tokens[current]; }

  Token previous() { return tokens[current - 1]; }

  Token consume(TokenType type, const char *message) {
    if (check(type))
      return advance();
    throw error(peek(), message);
  }

  static ParserError error(const Token &token, const char *message) {
    Lox::Lox::report(token.getLine(), "", message);
    return {};
  }

  void synchronize() {
    advance();
    while (!isAtEnd()) {
      if (previous().getType() == TokenType::SEMICOLON)
        return;
      switch (peek().getType()) {
      case TokenType::CLASS:
      case TokenType::FUN:
      case TokenType::VAR:
      case TokenType::FOR:
      case TokenType::IF:
      case TokenType::WHILE:
      case TokenType::PRINT:
      case TokenType::RETURN:
        return;
      default:
        break;
      }
      advance();
    }
  }

public:
  explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

  std::shared_ptr<Expr> parse() {
    try {
      return expression();
    } catch (const ParserError &) {
      synchronize();
      return nullptr;
    }
  }
};
} // namespace Lox

#endif // LOX_PARSER_H
