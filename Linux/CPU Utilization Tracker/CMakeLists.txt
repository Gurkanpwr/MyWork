cmake_minimum_required(VERSION 3.24)
project(CUT C)

set(CMAKE_C_STANDARD 99)

add_executable(CUT main.c
        threadfunctions.h
        threadfunctions.c Prod_Cons_buffer.c)



