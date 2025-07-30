cmake_minimum_required(VERSION 3.16)

project(fin-proto-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(FETCHCONTENT_BASE_DIR "$ENV{HOME}/_deps" CACHE PATH "Base directory for FetchContent downloads")
include(FetchContent)

include_directories(${PROJECT_SOURCE_DIR}/include)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)
FetchContent_MakeAvailable(googletest)
find_package(ZLIB REQUIRED)
enable_testing()

add_executable(bytebuf_test ./test/bytebuf_test.cpp)
target_include_directories(bytebuf_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(bytebuf_test PUBLIC gtest gtest_main)

add_executable(codec_test ./test/codec_test.cpp)
target_include_directories(codec_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(codec_test PUBLIC gtest gtest_main)


add_executable(root_packet_test ./test/root_packet_test.cpp ./test/root_packet_test_1.cpp)
target_include_directories(root_packet_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(root_packet_test PUBLIC gtest gtest_main)

add_executable(sse_bin_test ./test/sse_binary_test.cpp)
target_include_directories(sse_bin_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(sse_bin_test PUBLIC gtest gtest_main ZLIB::ZLIB)

add_executable(szse_bin_test ./test/szse_binary_test.cpp)
target_include_directories(szse_bin_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(szse_bin_test PUBLIC gtest gtest_main ZLIB::ZLIB)

add_executable(rc_bin_test ./test/rc_binary_test.cpp)
target_include_directories(rc_bin_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(rc_bin_test PUBLIC gtest gtest_main)

add_executable(checksum_test ./test/checksum_test.cpp)
target_include_directories(checksum_test PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(checksum_test PUBLIC gtest gtest_main ZLIB::ZLIB)

include(GoogleTest)
gtest_discover_tests(bytebuf_test)
gtest_discover_tests(codec_test)
gtest_discover_tests(root_packet_test)
gtest_discover_tests(rc_bin_test)
gtest_discover_tests(sse_bin_test)
gtest_discover_tests(szse_bin_test)
gtest_discover_tests(checksum_test)
