cmake_minimum_required(VERSION 3.14)
project(Paralel_Lab1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/EHsc)
else()
    add_compile_options(-pthread)
    link_libraries(pthread)
endif()

set(LAB_SOURCES
    lab1_2_1
    lab1_2_2
    lab1_2_3
    lab1_2_4
    lab1_2_5
    lab1_2_6
    lab1_2_7
)

foreach(target ${LAB_SOURCES})
    add_executable(${target} ${target}.cpp)
    if(NOT MSVC)
        target_link_libraries(${target} PRIVATE pthread)
    endif()
endforeach()
