//
// Created by boyuan on 2022/4/14.
//

#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H

#include "alloc.h"
#include "construct.h"

namespace tt {
    template<class T>
    class allocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
    public:
        static pointer allocate();
        static pointer allocate(size_type n);

        static void deallocate(pointer ptr);
        static void deallocate(pointer ptr, size_type n);

        static void construct(pointer ptr);
        static void construct(pointer ptr, const_reference val);
        static void destroy(pointer ptr);
        static void destroy(pointer first, pointer last);

    };

    template <class T>
    typename allocator<T>::pointer
    allocator<T>::allocate() {
        return static_cast<pointer>(alloc::allocate(sizeof(T)));
    }

    template <class T>
    typename allocator<T>::pointer
    allocator<T>::allocate(size_type n) {
        return static_cast<pointer>( alloc::allocate(sizeof(T) * n));
    }

    template <class T>
    void
    allocator<T>::deallocate(pointer ptr) {
        alloc::deallocate(ptr, sizeof(T));
    }

    template<class T>
    void
    allocator<T>::deallocate(pointer ptr, size_type n) {
        if (!ptr || n == 0) return;
        alloc::deallocate(ptr, sizeof(T) * n);
    }

    template<class T>
    void
    allocator<T>::construct(pointer ptr) {
        new(ptr) T();
    }

    template<class T>
    void
    allocator<T>::construct(pointer ptr, const_reference val) {
        new(ptr) T(val);
    }

    template<class T>
    void
    allocator<T>::destroy(pointer ptr) {
        ptr->~T();
    }


    template<class T>
    void allocator<T>::destroy(pointer first, pointer last) {
        while (first != last) {
            first->~T();
            ++first;
        }
    }



} // namespace tt

#endif //TINYSTL_ALLOCATOR_H
