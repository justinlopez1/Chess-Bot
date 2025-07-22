cmake_minimum_required(VERSION 3.12)

project(chessbot C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Gather all sources
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(chessbot ${SOURCES})
