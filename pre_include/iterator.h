//
// Created by boyuan on 2022/4/27.
//

#ifndef TINYSTL_ITERATOR_H
#define TINYSTL_ITERATOR_H

#include <cstdlib>

namespace tt {

    // 五种迭代器类型
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };

    template<class T, class Distance>
    struct output_iterator {
        typedef output_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };


    template<class T, class Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };


    template<class T, class Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };


    template<class T, class Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };


    template <class Category, class T, class Distance=ptrdiff_t, class Pointer=T*, class Reference=T&>
    struct iterator {
        typedef Category  iterator_category;
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
    };


    // 迭代器萃取器
    template<class I>
    struct iterator_traits {
        typedef typename I::iterator_category iterator_category;
        typedef typename I::value_type        value_type;
        typedef typename I::difference_type   difference_type;
        typedef typename I::pointer           pointer;
        typedef typename I::reference         reference;
    };

    // 迭代器萃取器的具体化模板（针对普通的指针
    template <class T>
    struct iterator_traits<T *> {
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef T*                            pointer;
        typedef T&                            reference;
    };

    template <class T>
    struct iterator_traits<const T *> {
        typedef random_access_iterator_tag    iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef const T*                      pointer;
        typedef const T&                      reference;
    };

    // 定义几个辅助函数来萃取出特性
    template <class I>
    inline typename iterator_traits<I>::iterator_category
    iterator_category(const I &) {
        typedef typename iterator_traits<I>::iterator_category category;
        return category();
    };

    template<class I>
    inline typename iterator_traits<I>::value_type *
    value_type(const I &) {
        return static_cast<typename iterator_traits<I>::value_type *>(nullptr);
    }


    template <class I>
    inline typename iterator_traits<I>::difference_type *
    difference_type(const I &) {
        return static_cast<typename iterator_traits<I>::difference_type *>(nullptr);
    }



}  // namespace tt




#endif //TINYSTL_ITERATOR_H
