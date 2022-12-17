cmake_minimum_required(VERSION 3.8)
project(brainfuck
        LANGUAGES C
        VERSION 2.7.3
        DESCRIPTION "Brainfuck interpreter written in C")

include_directories(include)

add_executable(brainfuck
    src/brainfuck.c
)