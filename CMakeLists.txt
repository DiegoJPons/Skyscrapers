cmake_minimum_required(VERSION 3.10)
project(skyscrapers LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(skyscrapers_game src/skyscrapers_game.c src/skyscrapers.c)
target_compile_options(skyscrapers_game PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(skyscrapers_game PUBLIC m)

add_executable(skyscrapers_solver src/skyscrapers_solver.c src/skyscrapers.c)
target_compile_options(skyscrapers_solver PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_link_libraries(skyscrapers_solver PUBLIC m)

add_executable(skyscrapers_solver_tests tests/unit_tests.cpp src/skyscrapers.c)
target_compile_options(skyscrapers_solver_tests PUBLIC -g -Wall -Wextra -Wshadow -Wpedantic -Wdouble-promotion -Wformat=2 -Wundef -Werror)
target_include_directories(skyscrapers_solver_tests PUBLIC include)
target_link_libraries(skyscrapers_solver_tests PUBLIC gtest gtest_main pthread m)
