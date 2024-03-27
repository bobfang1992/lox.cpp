//
// Created by Bob Fang on 3/27/24.
//

#ifndef LOX_LOX_H
#define LOX_LOX_H

namespace Lox {
struct Lox {
  static bool hadError;
  static void error(int line, const char *message);
  static void report(int line, const char *where, const char *message);
};
} // namespace Lox

#endif // LOX_LOX_H
