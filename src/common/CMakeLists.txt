project(common)

add_library(${PROJECT_NAME} 
    Log.cpp
    Utils.cpp
)
add_library(common_lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

