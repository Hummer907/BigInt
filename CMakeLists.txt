cmake_minimum_required(VERSION 3.26)
project(BigInteger)

set(CMAKE_CXX_STANDARD 17)

add_executable(BigInteger
        cmake-build-debug/BigInt.cpp
        cmake-build-debug/BigInt.h
        cmake-build-debug/main.cpp
        cmake-build-debug/InvalidInputException.cpp
        cmake-build-debug/InvalidInputException.h
)
