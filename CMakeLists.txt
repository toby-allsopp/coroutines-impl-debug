cmake_minimum_required(VERSION 3.0)

project(coroutines-impl-debug)
add_executable(${PROJECT_NAME} "main.cpp")
if(MSVC)
target_compile_options(${PROJECT_NAME} PUBLIC /std:c++latest /await)
else()
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -fcoroutines-ts)
endif()
