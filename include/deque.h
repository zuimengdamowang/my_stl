//
// Created by boyuan on 2022/5/6.
//

#ifndef TINYSTL_DEQUE_H
#define TINYSTL_DEQUE_H

#include <cstddef>

#include "algorithm.h"
#include "memory_aux.h"
#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"


namespace tt {

    template<class T, class Ref, class Ptr>
    struct deque_iterator: public iterator_base<random_access_iterator_tag, T> {
        template< T,  Ref,  Ptr>
        friend
        typename deque_iterator<T, Ref, Ptr>::difference_type
        operator-(const deque_iterator<T, Ref, Ptr> & it1, const deque_iterator<T, Ref, Ptr> & it2);

    public:
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using reference         = Ref;
        using pointer           = Ptr;

        using map_pointer       = T **;
        using self              = deque_iterator<T, Ref, Ptr>;

        static constexpr size_t buffer_size_ = 8;
    public:
        pointer     cur_;     // 迭代器所指的元素
        pointer     first_;   // 缓冲区的头
        pointer     last_;    // 缓冲区的尾
        map_pointer node_;    // 迭代器所在的缓冲区

    public:
        deque_iterator(pointer _cur=nullptr,
                       pointer _first=nullptr,
                       pointer _last= nullptr,
                       map_pointer _node= nullptr
                       ) : cur_(_cur), first_(_first), last_(_last), node_(_node) {}
        deque_iterator(const deque_iterator<T, Ref, Ptr> &other): cur_(other.cur_), first_(other.first_), last_(other.last_), node_(other.node_) {}

        reference operator*() {return *cur_;}
        pointer operator->() {return &(operator*());}
        self &operator++();
        self operator++(int);
        self &operator--();
        self operator--(int);
        self &operator+=(difference_type n);
        self &operator-=(difference_type n);
        self operator+(difference_type n);
        self operator-(difference_type n);
        bool operator==(const self & other) const;
        bool operator!=(const self & other) const;

    public:
        void set_node(map_pointer new_node);

    };  // class deque_iterator


    // 双端队列，支持随机访问。
    // 接口功能见：https://zh.cppreference.com/w/cpp/container/vector
    //
    // deque 的元素是分段存储的，不是想 vector 一样，相接存储。
    // 与 vector 的不同：
    // - deque 没有容量（capacity）的概念， 因为 deque 的元素是分段存储的，
    //   它的存储是按需自动扩容及收缩。
    // - deque 没有 reserve 功能。
    //
    // deque 上常见操作的复杂度（效率）如下：
    // - 随机访问——常数 O(1)
    // - 在结尾或起始插入或移除元素——常数 O(1)
    // - 插入或移除元素——线性 O(n)
    //
    // 虽然 deque 支持随机访问，但它的迭代器不是指针，随机访问是 deque 迭代器经过处理后结果。
    // 所以，使用的时候，尽量选择 vector 而不是 deque。
    //
    // 注意：迭代器失效完全看实现，不同的实现可能有不同的结果。

    template<class T, class Alloc=allocator<T>>
    class deque {
    public:
        using value_type            = T;               // 数据类型
        using pointer               = T*;              // 指针
        using const_pointer         = const T*;        // 指向常量的指针
        using reference             = T&;              // 引用
        using const_reference       = const T&;        // 常量引用
        using size_type             = std::size_t;     // 数量类型
        using difference_type       = std::ptrdiff_t;  // 用来保存两个指针（迭代器）的距离


        using iterator              = deque_iterator<T, T&, T*>;
        using const_iterator        = deque_iterator<T, const T&, const T*>;
        using self                  = deque<T, Alloc>;

    private:
        using map_allocator     = typename Alloc::template rebind<T*>::other;   // 分配中控器map_
        using node_allocator    = Alloc;                                        // 给buffer vector分配元素

        using map_pointer       = pointer *;

        static constexpr size_type buffer_size_     = 8;   // buffer 的大小
        // static constexpr size_type min_map_size_    = 8;   // map 的最小容量

        // [start, finish)
        iterator    start_;      // 起始迭代器
        iterator    finish_;     // 末尾迭代器
        map_pointer map_;        // 中控器(数组)
        size_type   map_size_;   // buffer vector数量

    public:
        deque() { empty_initialize(); }
        deque(const size_type& n, const value_type& x = value_type()) { fill_initialize(n, x);}
        template<class InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque & other);
        deque& operator=(const deque &other);
        ~deque() { delete_deque();}

        bool operator==(const deque &other);
        bool operator!=(const deque &other) {return !(*this == other);}


        size_type size() const {return size_type(finish_ - start_);}
        bool empty() const { return start_== finish_; }
        iterator begin() { return start_; }
        const_iterator begin() const {return start_;}       //const迭代器的转化
        iterator end() { return finish_; }
        const_iterator end() const {return finish_;}
        const_iterator cbegin() const {return start_;}
        const_iterator cend() const {return finish_;}
        reference front() { return *start_; }
        reference back() { return *(finish_ - 1); }
        const_reference front() const {return *start_;}
        const_reference back() const {return *(finish_ - 1);}
        void push_back(const value_type& x);
        void push_front(const value_type& x);
        void pop_back();
        void pop_front();
        iterator insert(iterator position, const value_type& x);
        iterator  insert(iterator position,const size_type& n, const value_type& x); //返回最后一个插入的位置
        template<class InputIterator>
        iterator insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void clear();

    private:

        map_pointer allocate_map(size_type map_size) {return map_allocator::allocate(map_size);}
        pointer allocate_buffer(size_type buffer_size) {return node_allocator::allocate(buffer_size);}
        void create_map_and_nodes(size_type num = 0);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);

        void empty_initialize() { create_map_and_nodes();}        //初始化一个空 deque
        void fill_initialize(size_type n, const T &x = value_type());

        template<class InputIterator>
        void copy_initialize(InputIterator first, InputIterator last, true_type);   // InputIterator不是迭代器

        template<class InputIterator>
        void copy_initialize(InputIterator first, InputIterator last, false_type);


        void delete_deque();


        iterator insert_aux(iterator position, const size_type& n, const value_type& x);
        template<class InputIterator>
        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::true_type);
        template<class InputIterator>
        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::false_type);



    };  // class deque


    // #################################### deque方法实现 ##########################################


    template<class T, class Alloc>
    void
    deque<T, Alloc>::create_map_and_nodes(size_type num) {
        size_type buffer_vector_nums = num / buffer_size_ + 1;   // map_size_
        map_size_           = buffer_vector_nums + 2;   // 首尾各加一个保证头插和尾插
        map_                = allocate_map(map_size_);  // 给map_分配内存

        // [n_first, n_finish]
        // 只不过n_finish指向的buffer可能不会填满(finish_所在的buffer是n_finish)
        map_pointer n_first = map_ + (map_size_ - buffer_vector_nums) / 2;
        map_pointer n_last  = n_first + buffer_vector_nums - 1;
        for (auto it = n_first; it <= n_last; ++it) {   // 给
            *it = allocate_buffer(buffer_size_);
        }
        start_.set_node(n_first);
        start_.cur_  = start_.first_;
        finish_.set_node(n_last);
        finish_.cur_ = finish_.first_ + num % buffer_size_;
    }


    template<class T, class Alloc>
    void
    deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_node_num = finish_.node_ - start_.node_ + 1;
        size_type new_node_num = old_node_num + nodes_to_add;
        if (map_size_ >= 2 * new_node_num) {
            /* 现在 map 有足够的位置，所以只需要移动元素即可
             * 将新的 [start_, finish_) 放到中间
             * 如果是在前面增加缓冲区，还要将 newStart 向后移动 nodesToAdd 个位置
             * 预留出来给将要增加的缓冲区
             */
            map_pointer new_start = map_ + (map_size_ - new_node_num) / 2 + (add_at_front ? nodes_to_add : 0);
            if (new_start < start_.node_) {
                tt::copy(start_.node_, finish_.node_ + 1, new_start);
            }else {
                tt::copy_backward(start_.node_, finish_.node_, new_start + nodes_to_add);
            }

            start_.set_node(new_start);
            finish_.set_node(new_start + old_node_num - 1);

        }else {
            // 现在 map 没有足够的位置，需要重新分配
            map_size_ = 2 * new_node_num + 2;
            map_pointer new_map = map_allocator::allocate(map_size_);
            map_pointer new_start = new_map + (map_size_ - new_node_num) / 2 + (add_at_front ? nodes_to_add : 0);
            tt::copy(start_.node_, finish_.node_, new_start);
            map_allocator::deallocate(map_, map_size_);

            map_ = new_map;
            start_.set_node(new_start);
            finish_.set_node(new_start + old_node_num - 1);
        }
    }



    template<class T, class Alloc>
    void
    deque<T, Alloc>::fill_initialize(size_type n, const T &x) {
        // 开辟内存，把内部结构安排好
        create_map_and_nodes(n);

        // 填充数据
        for (auto it = start_.node_; it < finish_.node_; ++it) {   // 先填满[ start_.node_, finish_.node_ )
            uninitialized_fill_n(*it, buffer_size_, x);
        }
        uninitialized_fill(finish_.first_, finish_.cur_, x);   // 在填充最后一个buffer
    }


    template<class T, class Alloc>
    template<class InputIterator>
    void
    deque<T, Alloc>::copy_initialize(InputIterator first, InputIterator last, true_type) {
        // InputIterator 类型是 整数 ====> 构造first个last
        fill_initialize(size_type(first), last);
    }


    template<class T, class Alloc>
    template<class InputIterator>
    void
    deque<T, Alloc>::copy_initialize(InputIterator first, InputIterator last, false_type) {
        difference_type n = distance(first, last);
        // 开辟内存
        create_map_and_nodes(n);
        // input_iterator_tag 类型迭代器只能逐个遍历
        for (auto it = start_; it != finish_; ++it) {
            construct(start_.cur_, *first);
        }

        // 不能使用以下实现，因为会将first视为POD true类型，调用memcpy的时候会出错(无法发生迭代器类型向void*的类型转换)
//        for (auto it = start_.node_; it < finish_.node_; ++it) {
//            auto next_position = next(first, buffer_size_);
//            uninitialized_copy(first, next_position, *it);
//            first = next_position;
//        }
//        uninitialized_copy(first, last, finish_.first_);
    }




    template<class T, class Alloc>
    void
    deque<T, Alloc>::delete_deque() {
        for (auto it = start_.node_; it < finish_.node_; ++it) {  // 析构[start_.node_, finish_.node_)
            node_allocator::destroy(*it, *it + buffer_size_);
            node_allocator::deallocate(*it, buffer_size_);
        }
        // 析构 finish_.node_
        node_allocator::destroy(finish_.first_, finish_.cur_);     // 只调用有元素部分的析构函数
        node_allocator::deallocate(finish_.first_, buffer_size_);  // 但是回收时要回收全部

        map_allocator::destroy(map_, map_ + map_size_);   // 这行可以不写，以为map_里面的元素时指针，属于POD，就算是调用了也啥也不做
        map_allocator::deallocate(map_, map_size_);
    }





    template<class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(deque::iterator position, const deque::size_type &n, const value_type &x) {
        auto elems_before = position - start_;
        if (elems_before < (size() >> 1)) {  // 前面元素少：移动前面
            for (int i = n; n > 0; --i) {
                push_front(x);
            }
            tt::copy(start_ + n, start_ + n + elems_before, start_);
            for (auto it = start_ + elems_before; it != position; ++it) {
                *it = x;
            }
        }else {
            for (int i = n; n > 0; --i) {
                push_back(x);
            }
            tt::copy_backward(position, finish_ - n, finish_);
            for (auto it = position; it != position + n; ++it) {
                *it = x;
            }
        }
        return start_ + elems_before + n - 1;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_range_aux(deque::iterator position,
                                      InputIterator first,
                                      InputIterator last,
                                      tt::true_type) {
        return insert_aux(position, size_type(first), last);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_range_aux(deque::iterator position,
                                      InputIterator first,
                                      InputIterator last,
                                      tt::false_type) {
        for (; first != last; ++first) {
            insert_aux(position, 1, *first);
        }
        return position - 1;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    deque<T, Alloc>::deque(InputIterator first, InputIterator last) {
        copy_initialize(first, last, typename tt::is_integral<InputIterator>::type());
    }

    template<class T, class Alloc>
    deque<T, Alloc>::deque(const deque &other) {
        copy_initialize(other.start_, other.finish_, typename tt::is_integral<const_iterator>::type());
    }

    template<class T, class Alloc>
    deque<T, Alloc> &
    deque<T, Alloc>::operator=(const deque &other) {
        if (*this != other) {
            copy_initialize(other.start_, other.finish_, typename tt::is_integral<const_iterator>::type());
        }
        return *this;
    }



    template<class T, class Alloc>
    bool
    deque<T, Alloc>::operator==(const deque &other) {
        return start_    == other.start_    &&
               finish_   == other.finish_   &&
               map_      == other.map_      &&
               map_size_ == other.map_size_;
    }

    template<class T, class Alloc>
    void
    deque<T, Alloc>::push_back(const value_type &x) {
        if (finish_.cur_ != finish_.last_ - 1) {   // 备用空间 大于等于2个
            construct(finish_.cur_, x);
            ++finish_.cur_;
        }else {
             // 需要看看map的备用空间够不够
             if (size_type(finish_.node_ - map_ + 1) >= map_size_) {  // 备用空间不够：重新分配内存
                 reallocate_map(1, false);
             }
             *(finish_.node_ + 1) = node_allocator::allocate(buffer_size_);
             construct(finish_.cur_, x);
             ++finish_;
        }
    }

    template<class T, class Alloc>
    void
    deque<T, Alloc>::push_front(const value_type &x) {
        if (start_.cur_ != start_.first_) {  // 看看是否在最前面
            construct(--start_.cur_, x);
        }else {
            // 在最前面
            // 看看前面还有没有剩余buffer vector
            if (map_ - start_.node_ == 0) {  // 没有：重新分配
                reallocate_map(1, true);
            }
            *(start_.node_ - 1) = node_allocator::allocate(buffer_size_);
            --start_;
            construct(start_.cur_, x);
        }
    }

    template<class T, class Alloc>
    void
    deque<T, Alloc>::pop_back() {
        if (finish_.cur_ == finish_.first_) {  // 尾迭代器正好指向当前buffer最前面：需要更新 finish_ 的信息
            node_allocator::deallocate(finish_.first_, buffer_size_);
            finish_.set_node(finish_.node_ - 1);
            finish_.cur_ = finish_.last_ - 1;
            node_allocator::deallocate(finish_.cur_);
        }else {
            // 尾元素不在缓冲区头部：直接析构
            node_allocator::destroy(--finish_.cur_);
        }
    }

    template<class T, class Alloc>
    void
    deque<T, Alloc>::pop_front() {
        if (start_.cur_ == start_.last_ - 1) {  // 在尾部
            node_allocator::destroy(start_.cur_);
            node_allocator::deallocate(start_.first_, buffer_size_);
            start_.set_node(start_.node_ + 1);
            start_.cur_  = start_.first_;
        }else {
            node_allocator::destroy(start_.cur_);
            ++start_.cur_;
        }
    }

    template<class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(deque::iterator position, const value_type &x) {
        if (position == finish_) {
            push_back(x);
            return finish_ - 1;
        }else if (position == start_) {
            push_front(x);
            return start_;
        }else {
            return insert_aux(position, 1, x);
        }
    }

    template<class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(deque::iterator position, const deque::size_type &n, const value_type &x) {
        if (position == finish_) {
            for (int i = 0; i < n; ++i) {
                push_back(x);
            }
            return finish_ - 1;
        }else if (position == start_) {
            for (int i = 0; i < n; ++i) {
                push_front(x);
            }
            return start_ + n - 1;
        }else {
            return insert_aux(position, n, x);
        }
    }

    template<class T, class Alloc>
    template<class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(deque::iterator position, InputIterator first, InputIterator last) {
        return insert_range_aux(position, first, last, typename tt::is_integral<InputIterator>::type());
    }

    template<class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::erase(deque::iterator position) {
        auto next = position + 1;
        difference_type num = position - start_ + 1;  // 计算删除点之前的元素
        if (size_type(num) < (size() >> 1)) {   // 之前的元素比较少：就移动之前的
            tt::copy_backward(start_, position, next);
            pop_front();
        }else {
            tt::copy(next, finish_, position);
            pop_back();
        }
        return start_ + num;
    }

    template<class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::erase(deque::iterator first, deque::iterator last) {
        if (start_ == finish_ && finish_ == last) {
            clear();
            return finish_;
        }
        difference_type n            = last    - first;    // 删除元素个数
        difference_type elems_before = first   - start_;   // 删除区间前方元素个数
        difference_type elems_after  = finish_ - last;
        if (elems_before < elems_after) {  // 前面元素比较少
            tt::copy_backward(start_, first, last);
            iterator new_start = start_ + n;
            node_allocator::destroy(start_, new_start);  // 析构前面的
            // 释放掉无用的map
            for (map_pointer node = start_.node_; node < new_start.node_; ++node) {
                node_allocator::deallocate(node, buffer_size_);
            }
            start_ = new_start;
        } else {   // 后面元素比较少
            tt::copy(last, finish_, finish_);
            iterator new_finish = finish_ - n;
            node_allocator::destroy(new_finish, finish_);
            for(map_pointer node = new_finish.node_; node <= finish_.node_; ++node) {
                node_allocator::deallocate(node, buffer_size_);
            }
            finish_ = new_finish;
        }
        return start_ + elems_before;
    }

    template<class T, class Alloc>
    void
    deque<T, Alloc>::clear() {
        // 恢复空构造时的初始
        // 针对头尾以外的buffer
        for (map_pointer node = start_.node_ + 1; node < finish_.node_; ++node) {
            node_allocator::destroy(*node, *node + buffer_size_);
            node_allocator::deallocate(*node, buffer_size_);
        }
        if (start_.node_ == finish_.node_) {  // 只剩余1个缓冲区
            node_allocator::destroy(start_.cur_, finish_.cur_);
            finish_ = start_;
        }else {  // 剩余2个缓冲区
            node_allocator::destroy(start_.first_, start_.last_);
            node_allocator::destroy(finish_.first_, finish_.last_);
            node_allocator::deallcate(finish_.first_, buffer_size_);  // 释放尾buffer，保留头buffer
            finish_ = start_;
        }
    }
















    // #################################### 迭代器方法实现 ##########################################


    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::difference_type
    operator-(const deque_iterator<T, Ref, Ptr> & last, const deque_iterator<T, Ref, Ptr> & first) {
        return (last.node_ - first.node_ - 1) * deque_iterator<T, Ref, Ptr>::buffer_size_ +
                (last.cur_ - last.first_) + (first.last_ - first.cur_);
    }



    template<class T, class Ref, class Ptr>
    void
    deque_iterator<T, Ref, Ptr>::set_node(typename deque_iterator::map_pointer new_node) {
        node_   = new_node;
        first_  = *new_node;
        last_   = first_ + difference_type (buffer_size_);
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self &
    deque_iterator<T, Ref, Ptr>::operator++() {
        ++cur_;
        if (cur_ == last_) {  // 到达尾部，切换到下一个缓冲区
            set_node(node_ + 1);
            cur_ = first_;
        }
        return *this;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self
    deque_iterator<T, Ref, Ptr>::operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self &
    deque_iterator<T, Ref, Ptr>::operator--() {
       if (cur_ == first_) {  // 已经是头部，切换到上一个缓冲区
           set_node(node_ - 1);
           cur_ = last_;  // [)
       }
       --cur_;
       return *this;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self
    deque_iterator<T, Ref, Ptr>::operator--(int) {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self &
    deque_iterator<T, Ref, Ptr>::operator+=(deque_iterator::difference_type n) {  // n可能是负数
        difference_type offset = cur_ + n - first_;
        if (offset >= 0 && offset < buffer_size_) {  // 不超出当前缓冲区
            cur_ += n;
        }else {
            // 计算向左还是向右超出了多少个缓冲区。
            difference_type node_offset = offset > n ? offset / buffer_size_ : -(offset - 1) / buffer_size_ + 1;
            set_node(node_ + node_offset);
            cur_ = first_ + (offset - node_offset * buffer_size_);
        }
        return *this;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self &
    deque_iterator<T, Ref, Ptr>::operator-=(deque_iterator::difference_type n) {
        return *this += -n;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self
    deque_iterator<T, Ref, Ptr>::operator+(deque_iterator::difference_type n) {
        auto tmp = *this;
        tmp += n;
        return tmp;
    }

    template<class T, class Ref, class Ptr>
    typename deque_iterator<T, Ref, Ptr>::self
    deque_iterator<T, Ref, Ptr>::operator-(deque_iterator::difference_type n) {
        auto tmp = *this;
        tmp += -n;
        return tmp;
    }

    template<class T, class Ref, class Ptr>
    bool
    deque_iterator<T, Ref, Ptr>::operator==(const deque_iterator::self & other) const {
        return  cur_   == other.cur_ ;
    }

    template<class T, class Ref, class Ptr>
    bool
    deque_iterator<T, Ref, Ptr>::operator!=(const deque_iterator::self &other) const {
        return !(*this == other);
    }








}  // namespace tt







#endif //TINYSTL_DEQUE_H
