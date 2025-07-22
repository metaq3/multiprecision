cmake_minimum_required(VERSION 3.22)
project(Multiprecision LANGUAGES CXX)

option(MP_SOURCE_DIRECTORY "MP location" "${PROJECT_SOURCE_DIR}/../multiprecision")

SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wextra -fsanitize=address -fsanitize=undefined -O0 -g)
add_link_options(-Wextra -fsanitize=address -fsanitize=undefined -O0 -g)

find_package(GTest REQUIRED)
include_directories(.)
include_directories(${MP_SOURCE_DIRECTORY}/src/include)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_executable(mp_test
        mp/comparison.cpp
        mp/bitwise.cpp
        mp/arithmetic.cpp
        mp/multiplication.cpp
        mp/division.cpp)

add_test(mp_test ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(mp_test GTest::gtest_main)
target_link_libraries(mp_test Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

install (
        TARGETS mp_test
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

include(GoogleTest)
gtest_discover_tests(mp_test)