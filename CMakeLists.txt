cmake_minimum_required(VERSION 3.0)
project(microlang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(test)

add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)

# create symlink in project root, so lsp could find it
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
