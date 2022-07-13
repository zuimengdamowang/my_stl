//
// Created by boyuan on 2022/5/5.
//

#ifndef TINYSTL_MEMORY_AUX_H
#define TINYSTL_MEMORY_AUX_H

#include <cstring>
#include <utility>
#include "type_traits.h"
#include "construct.h"
#include "algorithm.h"


namespace tt {


    /***************************************************************************/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, true_type);
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, false_type);

    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        typedef typename __type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
        return _uninitialized_copy_aux(first, last, result, isPODType());
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, true_type){
        memcpy(result, first, (last - first) * sizeof(*first));
        return result + (last - first);
    }
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, false_type){
        int i = 0;
        for (; first != last; ++first, ++i){
            construct((result + i), *first);
        }
        return (result + i);
    }

    /***************************************************************************/
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, true_type);
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, false_type);

    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename __type_traits<T>::is_POD_type isPODType;
        _uninitialized_fill_aux(first, last, value, isPODType());
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, true_type){
        fill(first, last, value);
    }
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                 const T& value, false_type){
        for (; first != last; ++first){
            construct(first, value);
        }
    }

    /***************************************************************************/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                              Size n, const T& x, true_type);
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                              Size n, const T& x, false_type);

    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
        typedef typename __type_traits<T>::is_POD_type isPODType;
        return _uninitialized_fill_n_aux(first, n, x, isPODType());
    }
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                              Size n, const T& x, true_type){
        return fill_n(first, n, x);
    }
    template<class ForwardIterator, class Size, class T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                              Size n, const T& x, false_type){
        int i = 0;
        for (; i != n; ++i){
            construct((T*)(first + i), x);
        }
        return (first + i);
    }



    template <class T>
    class SharedPtr {
    public:
        explicit SharedPtr(T *ptr) : ptr_(ptr), cnt_(new unsigned int(1)) {}
        SharedPtr(const SharedPtr<T> &other) : ptr_(other.ptr_), cnt_(other.cnt_) { ++(*cnt_); }
        SharedPtr<T>& operator=(const SharedPtr &other) {
            if (this != other) {
                --(*cnt_);
                if (*cnt_ == 0) {
                    delete ptr_;
                    delete cnt_;
                }
                ptr_ = other.ptr_;
                cnt_ = other.cnt_;
                ++cnt_;
            }
            return *this;
        }

        ~SharedPtr() {
            --(*cnt_);
            if (*cnt_ == 0) {
                delete cnt_;
                delete ptr_;
            }
        }


        T& operator*() {
            return *ptr_;
        }

        T* operator->() {
            return &(*this);
        }

    private:
        T               *ptr_;
        unsigned int    *cnt_;
    };






}  // namespace tt










#endif //TINYSTL_MEMORY_AUX_H
