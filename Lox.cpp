//
// Created by Bob Fang on 3/27/24.
//

#include <iostream>

#include "Lox.h"

bool Lox::Lox::hadError = false;

void Lox::Lox::error(int line, const char *message) {
  report(line, "", message);
  hadError = true;
}

void Lox::Lox::report(int line, const char *where, const char *message) {
  std::cerr << "[line " << line << "] Error" << where << ": " << message
            << std::endl;
}
