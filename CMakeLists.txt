cmake_minimum_required(VERSION 3.0)
project(microlang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(main
    src/main.cpp
    src/AST.cpp
    src/Lexer.cpp
    src/Utils.cpp
    src/AstVisitor.cpp
    src/Log.cpp)

add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(main ${llvm_libs})

# create symlink in project root, so lsp could find it
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
