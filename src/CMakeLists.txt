project(main)

add_executable(main main.cpp)

add_subdirectory(common)
add_subdirectory(front)
add_subdirectory(back)

target_link_libraries(main
    PRIVATE
        common
        front
        back
)

