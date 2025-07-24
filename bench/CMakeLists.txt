cmake_minimum_required(VERSION 3.22)
project(Multiprecision LANGUAGES CXX)

option(MP_SOURCE_DIRECTORY "MP location" "${PROJECT_SOURCE_DIR}/../multiprecision")

SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_COMPILER /bin/clang++)
set(CMAKE_C_COMPILER /bin/clang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wextra -O2)
add_link_options(-Wextra -O2)

find_package(GTest REQUIRED)
include_directories(.)
include_directories(${MP_SOURCE_DIRECTORY}/src/include)
include_directories(${MP_SOURCE_DIRECTORY}/include)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_executable(mp_bench
        mp/integer/arithmetic.cpp)

add_test(mp_bench ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(mp_bench GTest::gtest_main)
target_link_libraries(mp_bench Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

install (
        TARGETS mp_bench
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

include(GoogleTest)
gtest_discover_tests(mp_bench)