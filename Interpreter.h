//
// Created by Bob Fang on 4/6/24.
//

#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "Expr.h"
#include <string>
#include <variant>
namespace Lox {
using LoxValue = std::variant<double, std::string, bool, std::nullptr_t>;

class Interpreter : Expr::Visitor {

  LoxValue result;

public:
  LoxValue interpret(Expr *expr) {
    auto value = expr->accept(*this);

    try {
      return std::any_cast<LoxValue>(value);
    } catch (const std::bad_any_cast &e) {
      throw std::runtime_error("Interpreter Error: " + std::string(e.what()));
    }
  }

  std::any visitLiteral(const Literal &expr) override { return expr.value; }

  std::any visitGrouping(const Grouping &expr) override {
    return evaluate(expr.expression.get());
  }

  std::any visitUnary(const Unary &expr) override {
    auto right = std::any_cast<LoxValue>(evaluate(expr.right.get()));

    switch (expr.op.getType()) {
    case TokenType::MINUS:
      if (std::holds_alternative<double>(right)) {
        return -std::get<double>(right);
      }
      throw std::runtime_error("Unary minus must be applied to a number");
    case TokenType::BANG:
      if (std::holds_alternative<bool>(right)) {
        return !std::get<bool>(right);
      }
      throw std::runtime_error("Unary bang must be applied to a boolean");
    default:
      throw std::runtime_error("Unknown unary operator");
    }
  }

  std::any visitBinary(const Binary &expr) override {
    auto left = std::any_cast<LoxValue>(evaluate(expr.left.get()));
    auto right = std::any_cast<LoxValue>(evaluate(expr.right.get()));

    switch (expr.op.getType()) {
    case TokenType::MINUS:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) - std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::SLASH:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) / std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::STAR:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) * std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::PLUS:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      }
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
      throw std::runtime_error("Operands must be two numbers or two strings");
    case TokenType::GREATER:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) > std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::GREATER_EQUAL:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) >= std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::LESS:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) < std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::LESS_EQUAL:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) <= std::get<double>(right);
      }
      throw std::runtime_error("Operands must be numbers");
    case TokenType::BANG_EQUAL:
      return left != right;
    case TokenType::EQUAL_EQUAL:
      return left == right;
    default:
      throw std::runtime_error("Unknown binary operator");
    }
  }

  std::any evaluate(Expr *expr) { return expr->accept(*this); }
};
} // namespace Lox

#endif // LOX_INTERPRETER_H
