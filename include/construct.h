//
// Created by boyuan on 2022/5/5.
//

#ifndef TINYSTL_CONSTRUCT_H
#define TINYSTL_CONSTRUCT_H

#include <new>
#include "type_traits.h"
#include "iterator.h"

namespace tt{

    template <class T1, class T2>
    inline void construct(T1 *ptr, const T2 &value) {
        // TODO 学习 new 的高级用法
        // placement new
        // 在 ptr 指向的空间上调用 T 的构造函数
        ::new(ptr) T1(value);
    }

    template <class T>
    inline void construct(T *ptr) {
        ::new(ptr) T();
    }

    /**
     * 析构 ptr 所指的对象
     * @tparam T 被析构的对象类型
     * @param ptr 被析构的对象的指针
     */
    template <class T>
    inline void destory(T *ptr) {
        ptr->~T(); // 调用对象的析构函数
    }


    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, true_type){}

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, false_type){
        for (; first != last; ++first){
            destroy(&*first);
        }
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){
        using valueType = typename iterator_traits<ForwardIterator>::value_type;
        using tag       = typename __type_traits<valueType>::has_trivial_destructor;
        _destroy(first, last, tag());
    }









}  // namespace tt






#endif //TINYSTL_CONSTRUCT_H
