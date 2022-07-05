//
// Created by boyuan on 2022/5/5.
//

#ifndef TINYSTL_ALGORITHM_H
#define TINYSTL_ALGORITHM_H

#include <cstring>
#include <utility>
#include <cstddef>

#include "type_traits.h"
#include "iterator.h"

namespace tt{


    //********* [fill] ********************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value)
    {
        for (; first != last; ++first)
            *first = value;
    }
    inline void fill(char *first, char *last, const char& value)
    {
        memset(first, static_cast<unsigned char>(value), last - first);
    }
    inline void fill(wchar_t *first, wchar_t *last, const wchar_t& value)
    {
        memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
    }
    //********* [fill_n] ********************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value)
    {
        for (; n > 0; --n, ++first)
            *first = value;
        return first;
    }
    template<class Size>
    char *fill_n(char *first, Size n, const char& value)
    {
        memset(first, static_cast<unsigned char>(value), n);
        return first + n;
    }
    template<class Size>
    wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value)
    {
        memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
        return first + n;
    }
    //*********** [min] ********************
    //********* [Algorithm Complexity: O(1)] ****************
    template <class T>
    const T& min(const T& a, const T& b){
        return !(b < a) ? a : b;
    }
    template <class T, class Compare>
    const T& min(const T& a, const T& b, Compare comp){
        return !comp(b, a) ? a : b;
    }
    //*********** [max] ********************
    //********* [Algorithm Complexity: O(1)] ****************
    template <class T>
    const T& max(const T& a, const T& b){
        return (a < b) ? b : a;
    }
    template <class T, class Compare>
    const T& max(const T& a, const T& b, Compare comp){
        return (copm(a, b)) ? b : a;
    }


    //*********** [swap] ********************
    //********* [Algorithm Complexity: O(1)] ****************

    template<class T>
    void
    swap(T &x, T &y) {
         T tmp = x;
         x = y;
         y = tmp;
    }

    //*********** [for_each] ********************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class ForwardIterator, class Comp>
    void for_each(ForwardIterator first, ForwardIterator last, Comp comp) {
        for (; first != last; ++first) {
            comp(*first);
        }
    }


    //********** [copy_backward] ******************************
    //********* [Algorithm Complexity: O(N)] ******************
    template<class InputIterator, class OutputIterator, class Distance>
    inline OutputIterator __copy_backward_d(InputIterator first, InputIterator last, OutputIterator result, Distance *) {
        Distance n = last - first;
        for (; n-- > 0; --result, --last) {
            *(result - 1) = *(last - 1);
        }
        return result + n;
    }

    template<class T>
    inline T *__copy_backward_t(T *first, T *last, T *result, true_type) {
        memmove(result - (last - first), first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    template<class T>
    inline T *__copy_backward_t(T *first, T *last, T *result, false_type) {
        return __copy_backward_d(first, last, result, (ptrdiff_t *)nullptr);
    }


    template <class InputIterator, class OutputIterator>
    inline OutputIterator __copy_backward(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        int n = 0;
        for (; first != last; --result, --last) {
            *(result - 1) = *(last - 1);
            ++n;
        }
        return result + n;
    }

    template <class InputIterator, class OutputIterator>
    inline OutputIterator __copy_backward(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag) {
        return __copy_backward_d(first, last, result, distance_type(first));
    }

    template <class InputIterator, class OutputIterator>
    inline OutputIterator _copy_backward(InputIterator first, InputIterator last, OutputIterator result) {
        typedef typename tt::iterator_traits<InputIterator>::iterator_category category;
        return __copy_backward(first, last, result, category());
    }

    template<class T>
    inline T *_copy_backward(T *first, T *last, T *result) {
        typedef typename tt::__type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first, last, result, t());
    }

    template <class InputIterator, class OutputIterator>
    inline OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result) {
        return _copy_backward(first, last, result);
    }

    template <>
    inline char *copy_backward(char *first, char *last, char *result) {
        memmove(result - (last - first), first, sizeof(char) * (last - first));
        return result + (last - first);
    }

    template <>
    inline wchar_t *copy_backward(wchar_t *first, wchar_t *last, wchar_t *result) {
        memmove(result - (last - first), first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    //********** [copy] ******************************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class InputIterator, class OutputIterator, class Distance>
    inline OutputIterator __copy_d(InputIterator first, InputIterator last, OutputIterator result, Distance*) {
        Distance n = last - first;
        while (n-- > 0) {
            *result++ = *first++;
        }
        return result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag){
        for (; first != last; ) {
            *result++ = *first++;
        }
        return result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag){
        return __copy_d(first, last, result, distance_type(first));
    }

    template<class T>
    inline T* __copy_t(T *first, T *last, T *result, true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    template<class T>
    inline T* __copy_t(T *first, T *last, T *result, false_type) {
        return __copy_d(first, last, result, (ptrdiff_t*)nullptr);
    }

    // _copy的具体化模板
    template<class T>
    inline T* _copy(T *first, T *last, T *result) {
        typedef typename tt::__type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result){
        typedef typename tt::iterator_traits<InputIterator>::iterator_category category;
        return __copy(first, last, result, category());
    }

    template <class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result){
        return _copy(first, last, result);
    }


    template<>
    inline char *copy(char *first, char *last, char *result){
        auto dist = last - first;
        memmove(result, first, sizeof(char) * dist);
        return result + dist;
    }

    template<>
    inline wchar_t *copy(wchar_t *first, wchar_t *last, wchar_t *result){
        auto dist = last - first;
        memcpy(result, first, sizeof(*first) * dist);
        return result + dist;
    }






}  // namespace tt

#endif //TINYSTL_ALGORITHM_H
