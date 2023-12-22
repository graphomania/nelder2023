cmake_minimum_required(VERSION 3.18)
project(neldermead)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} cmd/main.cpp internal/common.h internal/method_nelder_mead.h
        internal/method_nelder_mead.cpp
        internal/method_random_walk.h
        internal/point.h
        internal/area.h
        internal/trace.h
        internal/method_random_walk.cpp
        internal/method.h
        cmd/args.h
        internal/test_functions.h)
