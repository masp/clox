cmake_minimum_required(VERSION 3.12)
project(clox C)

set(CMAKE_C_STANDARD 11)

add_executable(clox main.c token_parser.c)