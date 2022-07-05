//
// Created by boyuan on 2022/4/27.
//

#ifndef TINYSTL_VECTOR_H
#define TINYSTL_VECTOR_H

#include <cassert>
#include "allocator.h"
#include "iterator.h"
#include "uninitialized.h"
#include "utility.h"
#include "construct.h"


namespace tt {
    template<class T, class Alloc = allocator<T>>
    class vector {
    public:
        // 定义内嵌式类型
        typedef T             value_type;
        typedef T*            pointer;
        typedef const T*      const_pointer;
        typedef T&            reference;
        typedef const T&      const_reference;
        typedef T*            iterator;
        typedef const T*      const_iterator;
        typedef size_t        size_type;
        typedef ptrdiff_t     difference_type;

    protected:
        // 定义空间配置器类型
        typedef Alloc dataAllocator;

    private:
        // 定义vector主要表达方式（3个指针）
        iterator start;
        iterator finish;
        iterator end_of_storage;

    public:
        // 定义成员方法

        // 构造 复制 移动 析构
        vector();
        explicit vector(size_type n);
        vector(const std::initializer_list<T> & lists);
        vector(size_t n, const value_type &val);
        vector(int n, const value_type &val);
        vector(long n, const value_type &val);
        template<class InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(const vector& v);
        vector(vector && v);

        vector& operator= (const vector<T> &v);
        vector& operator= (vector<T> &&v);

        ~vector();

        // 比较操作相关
        bool operator== (const vector<T> &v);
        bool operator!= (const vector<T> &v);

        // 迭代器相关
        iterator begin() {return start;}
        const_iterator cbegin() const {return start;}
        iterator end() {return finish;}
        const_iterator cend() const {return finish;}

        // 容量相关
        difference_type size() const {return finish - start;}
        difference_type capacity() const {return end_of_storage - start;}
        bool empty() const  {return start == finish;}
        void resize(size_t n, value_type val = value_type());
        void reserve(size_t n);
        void shrink_to_fit();

        // 访问元素相关
        reference operator[] (const difference_type i) {return *(begin() + i);}
        const_reference operator[] (const difference_type i)const {return *(cbegin() + i);}
        reference at(const difference_type i) {return *(start + i);}
        const_reference at(const difference_type i)const {return *(start + i);}
        reference front() {return *begin();}
        const_reference front() const {return *start;}
        reference back() {return *(end() - 1); };
        const_reference back() const {return *(finish - 1);}
        pointer data() {return start;};
        const_pointer data() const {return start;}

        // 修改容器相关的操作
        void clear();
        void swap(vector &v);
        void push_back(const value_type &value);
        void pop_back();
        iterator insert(iterator position, const value_type &value);
        void insert(iterator position, const size_t n, const value_type &val);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);


        // 容器的空间配置器相关
        dataAllocator get_allocator() {return dataAllocator();}

    private:
        // 操作工具类方法
        void allocate_fill_initialize(size_t n, const value_type& val);

        template<class InputIterator>
        void allocate_copy_initialize(InputIterator first, InputIterator last);


        void destroy_and_deallocate_all();

        template<class InputIterator>
        void destroy_and_deallocate(InputIterator first, InputIterator last);

        void insert_aux(iterator pos, const size_type n, const value_type &val);

        size_type get_new_cap(difference_type sz);

    };


    //***********************操作工具类方法***********************

    template<class T, class Alloc>
    void vector<T, Alloc>::allocate_fill_initialize(size_type n, const value_type &val) {
        start = dataAllocator::allocate(n);
        uninitialized_fill_n(start, n, val);
        finish = start + n;
        end_of_storage = finish;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::allocate_copy_initialize(InputIterator first, InputIterator last) {
        start = dataAllocator::allocate(last - first);
        finish = uninitialized_copy(first, last, start);
        end_of_storage = finish;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::destroy_and_deallocate(InputIterator first, InputIterator last) {
        destroy(first, last);
        dataAllocator::deallocate(&*first, last - first);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::destroy_and_deallocate_all() {
        if (capacity() != 0) {
            destroy_and_deallocate(start, finish);
        }
    }

    // insert_aux
    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator pos, const size_type n, const value_type &val) {
        assert(n > 0);
        difference_type left_space = end_of_storage - finish;
        difference_type need_space = n;
        if (left_space >= need_space) {   // 剩余空间 大于等于 所需空间
            const size_type elems_after = finish - pos;
            iterator old_finish = finish;
            if (elems_after > n) {  // 后移元素个数 大于 新增元素个数
                uninitialized_copy(finish - n, finish, finish);   // 先在后面填充n个
                finish += n;
                // [pos, old_finish - n) ==> [pos + n, old_finish)
                tt::copy_backward(pos, old_finish - n, old_finish);
                // 填充新元素
                tt::fill(pos, pos + n, val);

            }else {  // 后移元素个数 小于等于 新增元素个数
                uninitialized_fill_n(finish, n - elems_after, val);
                finish += n - elems_after;
                uninitialized_copy(pos, old_finish, finish);
                finish += elems_after;
                fill(pos, old_finish, val);
            }
        } else {    //  剩余空间 小于  所需空间
            // 重新分配，并拷贝
            size_type new_capacity = get_new_cap(n);
            auto new_start = dataAllocator::allocate(new_capacity);
            auto new_finish = new_start;
            new_finish = uninitialized_copy(start, pos, new_finish);  // 复制插入点之前的元素
            new_finish = uninitialized_fill_n(new_finish, n, val);   // 添加新的内容
            new_finish = uninitialized_copy(pos, finish, new_finish);  // 复制插入点之后的内容
            // 清除旧内容
            destroy_and_deallocate_all();
            // 更新迭代器位置
            start = new_start;
            finish = new_finish;
            end_of_storage = start + new_capacity;
        }
    }





    //***********************构造，复制，析构相关***********************

    // 构造

    template<class T, class Alloc>
    vector<T, Alloc>::vector(): start(0), finish(0), end_of_storage(0) {}


    template<class T, class Alloc>
    vector<T, Alloc>::vector(size_type n) {
        allocate_fill_initialize(n, value_type());
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(const std::initializer_list<T> & lists) {
        allocate_copy_initialize(lists.begin(), lists.end());
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(size_type n, const value_type &val) {
        allocate_fill_initialize(n, val);
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(int n, const value_type &val) {
        allocate_fill_initialize(n, val);
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(long n, const value_type &val) {
        allocate_fill_initialize(n, val);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
        allocate_copy_initialize(first, last);
    }

    // 拷贝构造
    template<class T, class Alloc>
    vector<T, Alloc>::vector(const vector &v) {
        allocate_copy_initialize(v.cbegin(), v.cend());
    }

    // 析构
    template<class T, class Alloc>
    vector<T, Alloc>::~vector() {
        destroy_and_deallocate_all();
        // destroy(start, finish);   // 先调用析构函数
        // dataAllocator::deallocate(start, capacity());   // 然后释放内存
    }

    // 移动构造
    template<class T, class Alloc>
    vector<T, Alloc>::vector(vector &&v) {
        start = v.start;
        finish = v.finish;
        end_of_storage = v.end_of_storage;

        v.start = v.finish = v.end_of_storage = nullptr;
    }

    //  复制赋值运算符  =
    template<class T, class Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T> &v) {
        if (this != &v) {
            const difference_type len = v.size();
            if (len > capacity()) {   // v中变量的长度大于自己的最大长度：直接申请构造个临时变量，然后交换
                vector<T, Alloc> tmp(v.cbegin(), v.cend());
                swap(tmp);
            }else if (size() >= len){   // v的变量个数小于等于自己的size
                auto i = tt::copy(v.start, v.finish, start);
                destroy(i, end());
                finish = start + len;
            }else {   // v的变量个数大于自己的size，但小于capacity
                tt::copy(v.start, v.start + size(), start);
                uninitialized_copy(v.start + size(), v.finish, finish);
                finish = start + len;
            }
        }
        return *this;
    }

    // 移动赋值运算符  =
    template<class T, class Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(vector<T> &&v) {
        if (this != &v) {
            destroy_and_deallocate_all();
            start = v.start;
            finish = v.finish;
            end_of_storage = v.end_of_storage;
            v.start = v.finish = v.end_of_storage = nullptr;
        }
        return *this;
    }

    // swap
    template<class T, class Alloc>
    void vector<T, Alloc>::swap(vector &v) {
        tt::swap(start, v.start);
        tt::swap(finish, v.finish);
        tt::swap(end_of_storage, v.end_of_storage);
    }

    // ==
    template<class T, class Alloc>
    bool vector<T, Alloc>::operator==(const vector<T> &v) {
        if (size() != v.size()) {
            return false;
        }else {
            auto ptr1 = start;
            auto ptr2 = v.start;
            for (; ptr1 != finish && ptr2 != v.finish; ++ptr1, ++ptr2) {
                if (*ptr1 != *ptr2) {
                    return false;
                }
            }
        }
        return true;
    }

    // !=
    template<class T, class Alloc>
    bool vector<T, Alloc>::operator!=(const vector<T> &v) {
        return !(*this == v);
    }

    // resize
    template<class T, class Alloc>
    void vector<T, Alloc>::resize(size_t n, value_type val) {
        if (n < size()) {  // 新空间比当前空间小: 删除(调用析构函数 + 更新迭代器位置)后面的
            erase(begin() + n, end());
        }else {     // 新空间比当前空间大: 在后面的插入
            insert(end(), n - size(), val);
        }
    }

    // reserve
    template<class T, class Alloc>
    void vector<T, Alloc>::reserve(size_t n) {
        if (n <= size()) {  // 新的capacity小于等于旧的：啥也不做
            return;
        }
        iterator new_start = dataAllocator::allocate(n);
        iterator new_finish = uninitialized_copy(start, finish, new_start);
        destroy_and_deallocate_all();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + n;
    }

    // shrink_to_fit
    template<class T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit() {
        iterator new_start = dataAllocator::allocate(size());
        iterator new_finish = uninitialized_copy(start, finish, new_start);
        destroy_and_deallocate_all();
        start = new_start;
        finish = new_finish;
        end_of_storage = finish;
    }

    // clear
    template<class T, class Alloc>
    void vector<T, Alloc>::clear() {
        erase(begin(), end());
    }

    // push_back
    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type &value) {
        if (finish != end_of_storage) {  // 还有备用空间
            construct(finish, value);
            ++finish;
        }else {
            insert_aux(end(), 1, value);
        }
    }

    // get_new_cap
    template<class T, class Alloc>
    typename vector<T, Alloc>::size_type
    vector<T, Alloc>::get_new_cap(vector<T, Alloc>::difference_type sz) {
        return size() + tt::max(size(), sz);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::pop_back() {
        --finish;
        destroy(finish);
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(vector::iterator position, const value_type &value) {
        insert_aux(position, 1, value);
    }

    template<class T, class Alloc>
    void
    vector<T, Alloc>::insert(vector::iterator position, const size_t n, const value_type &val) {
        insert_aux(position, n, val);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void
    vector<T, Alloc>::insert(vector::iterator position, InputIterator first, InputIterator last) {
        // 还没实现
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(vector::iterator pos) {
        return erase(pos, pos + 1);
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(vector::iterator first, vector::iterator last) {
        auto i = tt::copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }




} // namespace tt







#endif //TINYSTL_VECTOR_H
