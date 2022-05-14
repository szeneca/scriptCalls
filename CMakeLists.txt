cmake_minimum_required(VERSION 3.16)
project(scriptCalls C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_compile_options("-g3")
add_compile_options("-fpermissive")
add_compile_options("-Wall")


add_executable(app
    main.cpp
)