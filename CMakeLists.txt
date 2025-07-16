cmake_minimum_required(VERSION 3.16)

project(fin-proto-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(FETCHCONTENT_BASE_DIR "$ENV{HOME}/_deps" CACHE PATH "Base directory for FetchContent downloads")
include(FetchContent)


FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(bytebuf_test ./test/bytebuf_test.cpp)
target_include_directories(bytebuf_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(bytebuf_test PRIVATE gtest gtest_main)

add_executable(codec_test ./test/codec_test.cpp)
target_include_directories(codec_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(codec_test PRIVATE gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(bytebuf_test)
gtest_discover_tests(codec_test)
