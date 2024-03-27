cmake_minimum_required(VERSION 3.27)
project(lox)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add your executable
add_executable(lox main.cpp
        TokeyType.cpp
        TokeyType.h
        Token.cpp
        Token.h
        Scanner.cpp
        Scanner.h
        Lox.cpp
        Lox.h
)

# Link the Readline library to your executable
target_link_libraries(lox edit)

