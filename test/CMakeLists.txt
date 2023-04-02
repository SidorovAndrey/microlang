project(test)

include(FetchContent)
# use latest commit hash on main: https://github.com/google/googletest/commits/main
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/b5fd99bbd55ebe1a3488b8ea3717fba089293457.zip 
)

FetchContent_MakeAvailable(
    googletest
)

enable_testing()

add_executable(
    test 
    front/ASTTests.cpp
)

target_link_libraries(
    test
    GTest::gtest_main
    gmock_main
    common
    front
    back
)

include(GoogleTest)
gtest_discover_tests(test)

