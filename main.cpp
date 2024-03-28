#include "argparse.h"
#include "editline/readline.h"
#include <fstream>
#include <iostream>

#include "Scanner.h"

bool global_debug_flag = false;

void run(std::string_view source) {
  if (global_debug_flag) {
    std::cout << "Running program " << source << "\n";
  }

  Lox::Scanner scanner(std::string(source.begin(), source.end()));
  std::vector<Lox::Token> tokens = scanner.scanTokens();
  if (global_debug_flag) {
    std::cout << "======== Scanner ========\n"
              << Lox::to_string(tokens) << "\n";
  }
}

void runFile(std::string_view path) {
  if (global_debug_flag) {
    std::cout << "Evaluating file " << path << "\n";
  }

  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()));
  if (content.empty()) {
    std::cerr << "Error: File is empty\n";
    std::cerr << "Exiting...\n";
    exit(1);
  }
  run(content);
}

void runPrompt() {
  const char *line;

  while ((line = readline("> ")) != nullptr) {
    if (*line) {
      add_history(line);
    }

    if (global_debug_flag) {
      std::cout << "Evaluating expression " << line << "\n";
    }

    std::string input_as_string(line);
    free((void *)line);

    if (input_as_string == "exit") {
      break;
    }

    run(input_as_string);
  }
}

int main(int argc, char **argv) {
  argparse::Parser parser;

  auto flag =
      parser.AddFlag("global_debug_flag", 'd', "Enable global_debug_flag mode");
  auto file = parser.AddArg<std::string>("file", "Path to file to run");

  parser.ParseArgs(argc, argv);
  if (*flag) {
    global_debug_flag = true;
  }
  if (file) {
    runFile(*file);
  } else {
    runPrompt();
  }
  return 0;
}
