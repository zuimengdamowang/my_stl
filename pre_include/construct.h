//
// Created by boyuan on 2022/4/27.
//

#ifndef TINYSTL_CONSTRUCT_H
#define TINYSTL_CONSTRUCT_H
#include <new>
#include "type_traits.h"
#include "iterator.h"

namespace tt {

    template<class T1, class T2>
    inline void construct(T1 *ptr, const T2 &val) {
        new(ptr) T1(val);
    }

    template<class T>
    inline void destroy(T *ptr) {
        ptr->~T();
    }


    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) {
        while (first != last) {
            destroy(&(*first));
            ++first;
        }
    }

    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _true_type) {
        // 啥都不做
    }


    template<class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
        typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }


    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        __destroy(first, last, value_type(first));
    }


}








#endif //TINYSTL_CONSTRUCT_H
