//
// Created by Bob Fang on 3/28/24.
//

#ifndef LOX_ASTPRINTER_H
#define LOX_ASTPRINTER_H

#include <type_traits>

#include "Expr.h"
#include "Lox.h"

namespace Lox {

struct ASTPrinter : public Expr::Visitor {
  std::string result;
  std::string print(Expr *expr) {
    expr->accept(*this);
    return result;
  }

  std::any visitAssign(const Assign &expr) override {
    result += "(= " + expr.name.getLexeme() + " ";
    expr.value->accept(*this);
    result += ")";
    return {};
  }

  std::any visitBinary(const Binary &expr) override {
    result += "(" + expr.op.getLexeme() + " ";
    expr.left->accept(*this);
    result += " ";
    expr.right->accept(*this);
    result += ")";
    return {};
  }

  std::any visitCall(const Call &expr) override {
    result += "(call ";
    expr.callee->accept(*this);
    for (const auto &arg : expr.arguments) {
      result += " ";
      arg->accept(*this);
    }
    result += ")";

    return {};
  }

  std::any visitGet(const Get &expr) override {
    result += "(get ";
    expr.object->accept(*this);
    result += "." + expr.name.getLexeme() + ")";

    return {};
  }

  std::any visitGrouping(const Grouping &expr) override {
    result += "(group ";
    expr.expression->accept(*this);
    result += ")";

    return {};
  }

  std::any visitLiteral(const Literal &expr) override {
    std::visit(
        [&](const auto &value) {
          if constexpr (std::is_same_v<std::string,
                                       std::decay_t<decltype(value)>>) {
            result += "(literal " + value + ")";
            return;
          }
          if constexpr (std::is_same_v<bool, std::decay_t<decltype(value)>> ||
                        std::is_same_v<double, std::decay_t<decltype(value)>>) {
            result += "(literal " + std::to_string(value) + ")";
            return;
          }

          Lox::error(1, "Unknown literal type");
        },
        expr.value);

    return {};
  }

  std::any visitLogical(const Logical &expr) override {
    result += "(" + expr.op.getLexeme() + " ";
    expr.left->accept(*this);
    result += " ";
    expr.right->accept(*this);
    result += ")";

    return {};
  }

  std::any visitSet(const Set &expr) override {
    result += "(set ";
    expr.object->accept(*this);
    result += "." + expr.name.getLexeme() + " ";
    expr.value->accept(*this);
    result += ")";

    return {};
  }

  std::any visitSuper(const Super &expr) override {
    result += "(super " + expr.method.getLexeme() + ")";

    return {};
  }

  std::any visitThis(const This &expr) override {
    result += "(this)";
    return {};
  }

  std::any visitUnary(const Unary &expr) override {
    result += "(" + expr.op.getLexeme() + " ";
    expr.right->accept(*this);
    result += ")";

    return {};
  }

  std::any visitVariable(const Variable &expr) override {
    result += "(var " + expr.name.getLexeme() + ")";

    return {};
  }
};

}; // namespace Lox
;  // namespace Lox

#endif // LOX_ASTPRINTER_H
