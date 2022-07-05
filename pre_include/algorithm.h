//
// Created by boyuan on 2022/4/28.
//

#ifndef TINYSTL_ALGORITHM_H
#define TINYSTL_ALGORITHM_H


#include <cstring>

namespace tt {

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


    //********** [advance] ******************************
    //********* [Algorithm Complexity: O(N)] ****************
    namespace {
        template<class InputIterator, class Distance>
        void _advance(InputIterator& it, Distance n, input_iterator_tag){
            assert(n >= 0);
            while (n--){
                ++it;
            }
        }
        template<class BidirectionIterator, class Distance>
        void _advance(BidirectionIterator& it, Distance n, bidirectional_iterator_tag){
            if (n < 0){
                while (n++){
                    --it;
                }
            }else{
                while (n--){
                    ++it;
                }
            }
        }
        template<class RandomIterator, class Distance>
        void _advance(RandomIterator& it, Distance n, random_access_iterator_tag){
            if (n < 0){
                it -= (-n);
            }else{
                it += n;
            }
        }
    }
    template <class InputIterator, class Distance>
    void advance(InputIterator& it, Distance n){
        typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
        _advance(it, n, iterator_category());
    }

    //********** [distance] ******************************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type dist = 0;
        while (first++ != last){
            ++dist;
        }
        return dist;
    }
    template<class RandomIterator>
    typename iterator_traits<RandomIterator>::difference_type
    _distance(RandomIterator first, RandomIterator last, random_access_iterator_tag){
        auto dist = last - first;
        return dist;
    }
    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type
    distance(Iterator first, Iterator last){
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        return _distance(first, last, iterator_category());
    }

    //********** [copy_backward] ******************************
    //********* [Algorithm Complexity: O(N)] ****************

    template <class InputIterator, class OutputIterator>
    OutputIterator __copy_backward(InputIterator first, InputIterator last, OutputIterator result, _true_type) {
        // 内置类型
        auto dist = distance(first, last);
        memcpy(result - dist, first, sizeof(*first) * dist);
        return result;
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator __copy_backward(InputIterator first, InputIterator last, OutputIterator result, _false_type) {
        // 非内置类型
        auto old_result = result;
        for (; last != first; --last, --result) {
            *(result - 1) = *(last - 1);
        }
        return old_result;
    }

    template <class InputIterator, class OutputIterator, class T>
    OutputIterator _copy_backward(InputIterator first, InputIterator last, OutputIterator result, T*) {
        typedef typename tt::_type_traits<T>::is_POD_type is_pod;
        return __copy_backward(first, last, result, is_pod());
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result) {
        return _copy_backward(first, last, result, value_type(first));
    }

    //********** [copy] ******************************
    //********* [Algorithm Complexity: O(N)] ****************
    template<class InputIterator, class OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, _true_type){
        auto dist = distance(first, last);
        memcpy(result, first, sizeof(*first) * dist);
        advance(result, dist);
        return result;
    }
    template<class InputIterator, class OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, _false_type){
        while (first != last){
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }
    template<class InputIterator, class OutputIterator, class T>
    OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, T*){
        typedef typename tt::_type_traits<T>::is_POD_type is_pod;
        return __copy(first, last, result, is_pod());
    }
    template <class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result){
        return _copy(first, last, result, value_type(first));
    }
    template<>
    inline char *copy(char *first, char *last, char *result){
        auto dist = last - first;
        memcpy(result, first, sizeof(*first) * dist);
        return result + dist;
    }
    template<>
    inline wchar_t *copy(wchar_t *first, wchar_t *last, wchar_t *result){
        auto dist = last - first;
        memcpy(result, first, sizeof(*first) * dist);
        return result + dist;
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



    /*****************************************************************************************/
    // for_each
    // 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
    // f() 可返回一个值，但该值会被忽略
    /*****************************************************************************************/

    template<class InputIterator, class Functor>
    Functor for_each(InputIterator first, InputIterator last, Functor functor) {
        for (; first != last; ++first) {
            functor(*first);
        }
        return functor;
    }











} // namespace tt










#endif //TINYSTL_ALGORITHM_H
