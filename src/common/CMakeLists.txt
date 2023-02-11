add_library(common "")

target_sources(common
    PRIVATE
        Log.cpp
        Utils.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Log.h
        ${CMAKE_CURRENT_LIST_DIR}/Utils.h
        ${CMAKE_CURRENT_LIST_DIR}/Result.h
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

