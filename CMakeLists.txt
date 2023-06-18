cmake_minimum_required(VERSION 3.22)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab5 main.cpp header/VirtualFileSystem.h source/VirtualFileSystem.cpp source/Exceptions.cpp header/Exceptions.h)
