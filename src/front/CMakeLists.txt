add_library(front "")

target_sources(front
    PRIVATE
        AST.cpp
        Lexer.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/AST.h
        ${CMAKE_CURRENT_LIST_DIR}/Lexer.h
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(front PRIVATE common back PUBLIC ${llvm_libs})

target_include_directories(front
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common
        ${CMAKE_CURRENT_SOURCE_DIR}/src/back
)

