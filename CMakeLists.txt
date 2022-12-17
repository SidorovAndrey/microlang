cmake_minimum_required(VERSION 3.0)
project(microlang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk)

add_executable(main
    src/main.cpp
    src/AST.cpp
    src/Lexer.cpp
    src/Utils.cpp)

add_compile_options(-Wall)

