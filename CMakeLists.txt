cmake_minimum_required(VERSION 3.16)

project(fin-proto-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    folly
    GIT_REPOSITORY https://github.com/facebook/folly.git
    GIT_TAG v2025.03.31.00
)

FetchContent_MakeAvailable(folly)

add_library(bin-codec ./include/codec.h)
target_link_libraries(bin-codec private folly)