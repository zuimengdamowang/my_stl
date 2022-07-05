cmake_minimum_required(VERSION 3.19)
project(TinySTL)

set(CMAKE_CXX_STANDARD 17)

add_executable(TinySTL
        main.cpp
        include/alloc.h
        include/allocator.h
        include/iterator.h
        include/type_traits.h
        include/construct.h
        include/memory_aux.h
        include/algorithm.h
        include/list.h
        include/deque.h
        )