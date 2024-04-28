cmake_minimum_required(VERSION 3.10)
project(guess)

include(cmake/pkg.cmake)

add_library(guess src/guesser.cc)
target_compile_features(guess PUBLIC cxx_std_14)
target_include_directories(guess PUBLIC include)
target_link_libraries(guess PRIVATE utf8proc)

add_executable(guess-example EXCLUDE_FROM_ALL example/main.cc)
target_link_libraries(guess-example guess)
