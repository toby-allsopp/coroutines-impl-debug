cmake_minimum_required(VERSION 3.0)

project(coroutines-impl-debug)
add_executable(${PROJECT_NAME} "main.cpp")
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -fcoroutines-ts)
