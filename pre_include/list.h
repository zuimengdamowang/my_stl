////
//// Created by boyuan on 2022/5/2.
////
//
//#ifndef TINYSTL_LIST_H
//#define TINYSTL_LIST_H
//
//#include "allocator.h"
//#include "iterator.h"
//#include "algorithm.h"
//#include "construct.h"
//#include "utility.h"
//
//
//namespace tt {
//
//    // list node
//    template<class T>
//    struct node{
//        T data;
//        struct node<T> *next;
//        struct node<T> *prev;
//        node(): data(T()), next(nullptr), prev(nullptr) {}
//        node(const T &_data): data(_data), next(nullptr), prev(nullptr) {}
//        node(const T &_data, struct _node *_next, struct _node *_prev): data(_data), next(_next), prev(_prev) {}
//    };
//
//
//
//
//    // list iterator
//    template<class T, class Ref, class Ptr>
//    struct list_iterator: public iterator<bidirectional_iterator_tag, T> {  // 继承一个迭代器（双向迭代器）
//        using typename iterator<bidirectional_iterator_tag, T>::pointer;
//        using typename iterator<bidirectional_iterator_tag, T>::reference;
//        using typename iterator<bidirectional_iterator_tag, T>::iterator_category;
//        using typename iterator<bidirectional_iterator_tag, T>::value_type;
//        using typename iterator<bidirectional_iterator_tag, T>::difference_type;
//
//        using node_ptr = node<T> *;
//        using self     = list_iterator<T, Ref, Ptr>;
//
//        node_ptr node;   // 指向当前节点
//
//        // 构造函数
//        list_iterator(): node(nullptr) {}
//        list_iterator(const node_ptr &_node_ptr): node(_node_ptr) {}
//        list_iterator(const list_iterator & lt): node(lt.node) {}
//
//        // 重载操作符
//
//        // ==
//        bool operator==(const self &x) const { return node == x.node; }
//
//        // !=
//        bool operator!=(const self &x) const { return node != x.node;}
//
//        // *
//        reference operator*() const {return (*node).data;}
//
//        // ->
//        pointer operator->() const {return &(operator*());}
//
//        // ++i
//        self & operator++() {
//            node = node->next;
//            return *this;
//        }
//
//        // i++
//        self operator++(int) {
//            self tmp = *this;
//            ++*this;
//            return tmp;
//        }
//
//        // --i
//        self & operator--() {
//            node = node->prev;
//            return *this;
//        }
//        // i--
//        self operator--(int) {
//            self tmp = *this;
//            --*this;
//            return tmp;
//        }
//
//    };
//
//
//    // list
//    template<class T, class Alloc = allocator<node<T> > >
//    class list{
//    public:
//        // 内嵌式定义
//        using value_type     = T;
//        using reference      = T&;
//        using size_type      = size_t;
//        using iterator       = list_iterator<T, T&, T*>;
//        using const_iterator = list_iterator<T, const T&, const T*>;
//        using link_type      = node<T> *;
//
//    private:
//        // 定义空间配置器
//        using dataAllocator = Alloc;
//
//    private:
//        // 定义list的表达形式(一个指针)
//        link_type head;
//
//    public:
//        // 定义成员方法
//
//        // 构造函数
//        list();
//        list(size_type n);
//        list(size_type n, const value_type &val);
//        list(iterator first, iterator last);
//        list(const list &_list);
//        list(const std::initializer_list<T> & _list);
//
//        // 析构
//        ~list();
//
//        // operator=()
//        list& operator=(const list &_list);
//
//        // 迭代器相关
//        iterator begin() {return iterator(head->next);}
//        const_iterator cbegin() const {return const_iterator(head->next);}
//        iterator end() {return iterator(head);}
//        const_iterator cend() const {return const_iterator(head);}
//
//        // 访问相关
//        reference front() {return *(begin());}
//        reference back()  {return *(--end());}
//
//        // 容量相关
//        bool empty() const {return head->next == head;}
//        size_type size() const { return (size_type)distance(begin(), end()); }
//        void resize(size_type n, value_type val=value_type());
//
//        // 增 删 查 改
//        void push_back(const value_type &val);
//        void push_front(const value_type &val);
//        void pop_back();
//        void pop_front();
//
//        iterator insert(const_iterator pos, const value_type &val);
//        iterator insert(const_iterator pos, const size_type n, const value_type &val);
//        template<typename InputIterator>
//        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
//
//        iterator erase(const_iterator pos);
//        iterator erase(const_iterator first, const_iterator last);
//
//        // 交换
//        void swap(list &_list);
//
//
//        // clear
//        void clear();
//
//        // remove
//        void remove(const value_type &val);
//
//        // unique
//        void unique();
//
//        // splice
//        void splice(iterator pos, list &_list);
//        void splice(iterator pos, iterator i);
//        void splice(iterator pos, iterator first, iterator last);
//
//        // merge
//        void merge(list &_list);
//
//        // 排序
//        void sort();
//        template<class Comp>
//        void sort(Comp comp);
//
//        // 逆置
//        void reserve();
//
//
//
//    private:
//        // 工具类方法
//
//        // transfer
//        void transfer(iterator pos, iterator first, iterator last);
//
//
//        // get_node()
//        iterator get_node();
//        void put_node(iterator pos);
//        iterator create_node(const value_type &val);
//        void destroy_node(iterator pos);
//
//        iterator insert_aux(const_iterator pos, const T &val);
//
//        void empty_init();
//
//        void fill_init(iterator pos, const size_type n, const T &val);
//
//        template<class InputIterator>
//        void copy_init(iterator pos, InputIterator first, InputIterator last);
//
//
//
//    };
//
//    // **************** 工具方法 ****************
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::get_node() {
//        return dataAllocator::allocate(1);
//    }
//
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::put_node(list::iterator pos) {
//        dataAllocator::deallocate(pos.node);
//    }
//
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::create_node(const value_type &val) {
//        iterator pos = iterator(get_node());
//        construct(pos.node, val);
//        return pos;
//    }
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::destroy_node(list::iterator pos) {
//        destroy(pos.node);
//        put_node(pos);
//    }
//
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert_aux(const_iterator pos, const T &val) {
//        iterator tmp = create_node(val);
//        tmp.node->next = pos.node;
//        tmp.node->prev = pos.node->prev;
//        pos.node->prev->next = tmp.node;
//        pos.node->prev = tmp.node;
//        return tmp;
//    }
//
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::empty_init() {
//        head = dataAllocator::allocate(1);
//        head->prev = head;
//        head->next = head;
//    }
//
//
//
//
//    // **************** 一般方法 ****************
//
//    // 构造函数
//
//    template<class T, class Alloc>
//    list<T, Alloc>::list() {
//        empty_init();
//    }
//
//    template<class T, class Alloc>
//    list<T, Alloc>::list(list::size_type n) {
//        empty_init();
//        insert(end(), n, value_type());
//    }
//
//    template<class T, class Alloc>
//    list<T, Alloc>::list(list::size_type n, const value_type &val) {
//        empty_init();
//        insert(end(), n, val);
//    }
//
//    template<class T, class Alloc>
//    list<T, Alloc>::list(iterator first, iterator last) {
//        empty_init();
//        insert(end(), first, last);
//
//    }
//
//    template <class T, class Alloc>
//    list<T, Alloc>::list(const list &_list) {
//        empty_init();
//
//        //insert(end(), _list.cbegin(), _list.cend());  // 错误
//
//        iterator first(_list.head.node->next);
//        iterator last(_list.head.node);
//        insert(end(), first, last);
//    }
//
//    template<class T, class Alloc>
//    list<T, Alloc>::list(const std::initializer_list<T> &_list) {
//        empty_init();
//        insert(end(), _list.begin(), _list.end());
//    }
//
//
//    template<class T, class Alloc>
//    list<T, Alloc>::~list() {
//        clear();
//    }
//
//    // operator=()
//    template<class T, class Alloc>
//    list<T, Alloc> &
//    list<T, Alloc>::operator=(const list &_list){
//        clear();
//        empty_init();
//        insert(end(), _list.cbegin(), _list.cend());
//
////        iterator first(_list.head.node->next);
////        iterator last(_list.head.node);
////        insert(end(), first, last);
//    }
//
//    // insert
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert(list::const_iterator pos, const value_type &val) {
//        return insert_aux(pos, val);
//    }
//
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert(list::const_iterator pos, const size_type n, const value_type &val) {
//        size_type cnt = n;
//        while (cnt-- > 0) {
//            pos = insert(pos, val);
//        }
//        return pos;
//    }
//
//    template<class T, class Alloc>
//    template<class InputIterator>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert(list::const_iterator pos, InputIterator first, InputIterator last) {
//        iterator ret = pos;
//        while (first != last) {
//            ret = insert(pos, *first);
//            ++first;
//        }
//        return ret;
//    }
//
//
//    // erase
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::erase(list::const_iterator pos) {
//        auto ret = (iterator)pos.node->next;
//        pos.node->prev->next = pos.node->next;
//        pos.node->next->prev = pos.node->prev;
//        destroy_node(pos);
//        return ret;
//    }
//
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::erase(list::const_iterator first, list::const_iterator last) {
//        auto ret = (iterator)last.node;
//        first.node->prev->next = last.node;
//        last.node->prev = first.node->prev;
//        for (; first != last; ) {
//            iterator next = iterator(first.node->next);
//            iterator tmp(first.node);
//            destroy_node(tmp);
//            first = next;
//        }
//        return ret;
//
////        for (; first != last; ) {
////            first = erase(first);
////        }
////        return iterator(first.node);
//    }
//
//    // resize
//    template<class T, class Alloc>
//    void list<T, Alloc>::resize(list::size_type n, value_type val) {
//        size_type sz = size();
//        if (sz <= n) {
//            size_type cnt = sz - n;
//            insert(end(), cnt, val);
//        }else {  // sz > n
//            size_type cnt = sz - n;
//            while (cnt--) {
//                erase(--end());
//            }
//        }
//    }
//
//    // push_back
//    template <class T, class Alloc>
//    void
//    list<T, Alloc>::push_back(const value_type &val) {
//        insert(end(), val);
//    }
//
//    // push_front
//    template <class T, class Alloc>
//    void
//    list<T, Alloc>::push_front(const value_type &val) {
//        insert(begin(), val);
//    }
//
//    // pop_back
//    template <class T, class Alloc>
//    void
//    list<T, Alloc>::pop_back() {
//        erase(--end());
//    }
//
//    // pop_back
//    template <class T, class Alloc>
//    void
//    list<T, Alloc>::pop_front() {
//        erase(begin());
//    }
//
//    // swap
//    template<class T, class Alloc>
//    void list<T, Alloc>::swap(list &_list) {
//        tt::swap(head, _list.head);
//    }
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::clear() {
//        erase(cbegin(), cend());
//        erase(cbegin());
//    }
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::remove(const value_type &val) {
//        iterator first = begin();
//        iterator last = end();
//        while (first != last) {
//            iterator nt = first;
//            ++nt;  // next
//            if (*first == val) {
//                erase(first);
//            }
//            first = nt;
//        }
//    }
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::unique() {
//        iterator first = begin();
//        iterator last = end();
//        if (first == last) return;
//        iterator next = first;
//        while (++next != last) {
//            if (*first == *next) erase(next);
//            else                 first = next;
//
//            next = first;
//        }
//    }
//
//    template<class T, class Alloc>
//    void list<T, Alloc>::reserve() {  // 交换所有节点的前后指针
//        iterator first = begin();
//        iterator last = end();
//        while (first != last) {
//            tt::swap(first.node->prev, first.node->next);
//            first = first.node->prev;
//        }
//        tt::swap(first.node->prev, first.node->next);
//    }
//
//
//
//
//}  // namespace tt
//
//
//#endif //TINYSTL_LIST_H
//
//
//
///*
// *
// * //
//// Created by boyuan on 2022/5/5.
////
//
//#ifndef TINYSTL_LIST_H
//#define TINYSTL_LIST_H
//
//#include <cstddef>
//#include <initializer_list>
//#include <limits>
//
//#include "iterator.h"
//#include "allocator.h"
//#include "construct.h"
//#include "type_traits.h"
//#include "algorithm.h"
//
//
//namespace tt {
//
//    template<class T>
//    struct list_node {
//        list_node<T>* pre;
//        list_node<T>* next;
//        T data;
//        list_node() : pre(nullptr), next(nullptr), data(T()) {}
//        list_node(list_node<T>* p,list_node<T>* n,const T& d) : pre(p), next(n), data(d) {}
//    };
//
//    template<class T, class Ref, class Ptr>
//    struct list_iterator : public iterator_base<bidirectional_iterator_tag, T>{
//        typedef T                           value_type;
//        typedef T*                          pointer;
//        typedef T&                          reference;
//        typedef ptrdiff_t                   difference_type;
//        typedef size_t                      size_type;
//        typedef bidirectional_iterator_tag  iterator_category;
//        typedef list_node<T>*               nodePtr;
//
//        nodePtr node;
//
//        list_iterator() {}
//        list_iterator(const list_iterator &x) : node(x.node) {}
//        list_iterator(const nodePtr n) : node(n) {}
//        bool operator==(const list_iterator& x) const { return node == x.node; }
//        bool operator!=(const list_iterator& x) const { return node != x.node; }
//        reference operator*() const { return (*node).data; }
//        pointer operator->() const { return &(operator*()); }
//        list_iterator& operator++() { node = (*node).next; return *this; }
//        list_iterator operator++(int) { nodePtr tmp = node; ++*this; return tmp; }
//        list_iterator& operator--() { node = (*node).pre; return *this; }
//        list_iterator operator--(int) { nodePtr tmp = node; --*this; return tmp; }
//    };
//
//
//
//
//    template<class T,class Alloc= allocator<list_node<T>>>
//    class list {
//    public:
//        typedef T                       value_type;
//        typedef size_t                  size_type;
//        typedef T&                      reference;
//        typedef T*                      pointer;
//        typedef const T&                const_reference;
//        typedef const T*                const_pointer;
//
//        typedef list_node<T>*                           nodePtr;
//        typedef list_iterator<T, T&, T*>                iterator;
//        typedef list_iterator<T, const T&, const T*>    const_iterator;
//
//
//        typedef Alloc                   list_allocator;
//
//    private:
//        nodePtr node;
//    public:
//        //构造，拷贝，赋值，析构
//        list() { empty_init(); }
//        list(size_type n, const value_type& x=value_type());
//        template<class InputIterator>
//        list(InputIterator first, InputIterator last);
//        list(const list& l);
//        list& operator=(const list& l);
//        ~list();
//
//        iterator begin() { return node->next; } //node->next是list_node<T>*,可以用改返回值初始化iterator
//        iterator end() { return node; }
//        const_iterator begin() const {return node->next;}//const this 使用
//        const_iterator end() const {return node;}
//        bool  empty() { return node->next == node; }
//        size_type size();
//
//        iterator push_back(const value_type& x);
//        iterator push_front(const value_type& x);
//        iterator insert(iterator position, const size_type& n,const value_type& x);
//        template<class InputIterator>
//        iterator insert(iterator position, InputIterator first, InputIterator last);
//        iterator erase(iterator iter);
//        iterator erase(iterator first, iterator last);
//        void pop_front();
//        void pop_back();
//        void clear();
//        void remove(const value_type& x);
//        void unique(); //只能删除连续的重复元素
//        void splice(iterator position, list& x); //不能连接list本身,position不能在first到last之间，下同
//        void splice(iterator position, iterator first, iterator last);
//        void splice(iterator position, list& l, iterator i);
//        void reverse();
//        void merge(list&); //两个list必须先递增排序
//        void swap(list& l);
//        void sort();
//
//        bool operator==(const list &l) const {return node == l.node;}
//        bool operator!=(const list &l) const {return node != l.node;}
//
//    private:
//        //分配空间并初始化
//        nodePtr create_node(const value_type& x);
//        void destroy_node(nodePtr p);
//        void empty_init();
//        iterator insert_aux(iterator position, const size_type& n, const value_type& x);
//        template<class InputIterator>
//        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::true_type);
//        template<class InputIterator>
//        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::false_type);
//        void transfer(iterator position, iterator first, iterator last);
//
//    };
//
//    //构造，拷贝，赋值，析构
//    template<class T,class Alloc>
//    list<T,Alloc>::list(size_type n, const value_type& x) {
//        empty_init();
//        insert(end(), n, x);
//    }
//    template<class T, class Alloc>
//    template<class InputIterator>
//    list<T,Alloc>::list(InputIterator first, InputIterator last) {
//        empty_init();
//        insert(end(), first, last);
//
//    }
//    template<class T, class Alloc>
//    list<T,Alloc>::list(const list& l) { //必须要定begin()和end()的const版本，传入const list使用const this调用成员函数,无法找到合适的const begin().
//        empty_init();
//
//        insert(end(), l.begin(), l.end());
//
//    }
//    template<class T, class Alloc>
//    list<T, Alloc>& list<T, Alloc>::operator=(const list& l) {
//        if (*this != l) {
//            clear();
//            insert(end(), l.begin(), l.end());
//        }
//        return *this;
//    }
//    template<class T, class Alloc>
//    list<T, Alloc>::~list() {
//        auto curr = node->next;
//        while (curr != node) {
//            auto tmp = curr;
//            curr = curr->next;
//            destroy_node(tmp);
//        }
//        destroy_node(node);
//    }
//    //容器操作
//    template<class T, class Alloc>
//    typename list<T, Alloc>::size_type list<T,Alloc>::size() {
//        auto first = begin();
//        size_type n = 0;
//        for (; first != end(); ++first, ++n);
//        return n;
//    }
//
////    template<class T, class Alloc>
////    typename list<T, Alloc>::const_iterator
////    list<T,Alloc>::begin() const {
////        auto tmp = (list* const)this;
////        iterator iter(tmp->node->next);
////        return  convert_iterator_to_const(iter);
////    }
////    template<class T, class Alloc>
////    typename list<T, Alloc>::const_iterator
////    list<T, Alloc>::end() const {
////        auto tmp = (list* const)this;
////        iterator iter(tmp->node);
////        return convert_iterator_to_const(iter);
////    }
//
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator list<T, Alloc>::push_back(const value_type& x) {
//        return insert_aux(end(), 1, x);
//    }
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator list<T, Alloc>::push_front(const value_type& x) {
//        return insert_aux(begin(), 1, x);
//    }
//    template<class T, class Alloc>
//    typename list<T,Alloc>::iterator
//    list<T,Alloc>::insert(iterator position, const size_type& n,const value_type& x) {
//        return insert_aux(position, n, x);
//    }
//    template<class T, class Alloc>
//    template<class InputIterator>
//    typename list<T,Alloc>::iterator
//    list<T,Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
//        return insert_range_aux(position, first, last, typename tt::is_integral<InputIterator>::type());
//    }
//
//    template<class T, class Alloc>
//    typename list<T,Alloc>::iterator list<T, Alloc>::erase(iterator iter) {
//        nodePtr next_node = iter.node->next;
//        iter.node->pre->next = iter.node->next;
//        iter.node->next->pre = iter.node->pre;
//        destroy_node(iter.node);
//        return next_node;
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::pop_front() {
//        erase(begin());
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::pop_back() {
//        auto tmp = end();
//        erase(--tmp);
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::clear() {
//        auto curr = node->next;
//        while (curr != node) {
//            auto tmp = curr;
//            curr = curr->next;
//            destroy_node(tmp);
//        }
//        node->pre = node;
//        node->next = node;
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::remove(const value_type& x) {
//        auto curr=begin();
//        auto last = end();
//        while (curr != last) {
//            if (*curr == x)
//                erase(curr++);
//            else
//                ++curr;
//        }
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::unique() {
//        auto first = begin();
//        auto next = first;
//        ++next;
//        if (first == end() || next == end()) return;
//        while (next != end()) {
//            if (*first == *next) {
//                erase(next++); //next失效
//            }
//            else {
//                first = next;
//                ++next;
//            }
//        }
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::splice(iterator position, list& x) {
//        transfer(position, x.begin(), x.end());
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::splice(iterator position, iterator first, iterator last) {
//        if(first!=last)
//            transfer(position, first, last);
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::splice(iterator position, list& l, iterator i) {
//        iterator j = i;
//        ++j;
//        if (position == i || position == j)  return;
//        transfer(position, i, j);
//    }
//    template<class T, class Alloc>
//    void list<T,Alloc>::reverse() {
//        auto first = begin();
//        auto curr = first;
//        ++curr;
//        auto last = end();
//        if (first == last || ++curr = last) return;
//        while (begin() != last) {
//            transfer(begin(), curr, ++curr);
//        }
//    }
//    template<class T, class Alloc>
//    void list<T, Alloc>::merge(list& l){
//        auto first1 = begin();
//        auto last1 = end();
//        auto first2 = l.begin();
//        auto last2 = l.end();
//        while (first1 != last1 && first2 != last2) {
//            if (*first2 < *first1) {
//                iterator next = first2;
//                transfer(first1, first2, ++next);
//                first2 = next;
//                //transfer(first1, first2, ++first2); //不能使用，运行时先进行加法运算，之后才对三个参数进行拷贝构造
//            }
//            else
//                ++first1;
//        }
//        if (first2 != last2)
//            transfer(last1, first2, last2);
//    }
//    template<class T, class Alloc>
//    void list<T, Alloc>::swap(list& l) {
//        tt::swap(node, l.node);
//    }
//    template<class T, class Alloc>
//    void list<T, Alloc>::sort() {
//        if (node->next == node || node->next->next == node) return;
//        list carry;
//        list counter[64];
//        int fill = 0;
//        while (!empty()) {
//            carry.splice(carry.begin(), *this, begin());
//            int i = 0;
//            while (i < fill && !counter[i].empty()) {
//                counter[i].merge(carry);
//                carry.swap(counter[i++]);
//            }
//            carry.swap(counter[i]);
//            if (i == fill) ++fill;
//        }
//        for (int i = 1; i < fill; ++i)
//            counter[i].merge(counter[i - 1]);
//        swap(counter[fill - 1]);
//    }
//    //private函数
//    template<class T, class Alloc>
//    typename list<T, Alloc>::nodePtr list<T, Alloc>::create_node(const value_type& x) {
//        nodePtr position= list_allocator::allocate();
//        list_allocator::construct(position, list_node<T>(nullptr, nullptr, x));
//        return position;
//    }
//    template<class T, class Alloc>
//    void list<T, Alloc>::destroy_node(nodePtr p) {
//        list_allocator::destroy(p);
//        list_allocator::deallocate(p);
//    }
//    template<class T, class Alloc>
//    void list<T, Alloc>::empty_init() {
//        node = list_allocator::allocate();
//        node->next = node;
//        node->pre = node;
//    }
//    template<class T, class Alloc>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert_aux(iterator position,const size_type& n, const value_type& x) {
//        auto i = n;
//        for (; i > 0; --i) {
//            auto new_node = create_node(x); //iterator是调用node的接口，一般在参数以及返回当中
//            new_node->next = position.node;
//            new_node->pre = position.node->pre;
//            position.node->pre->next = new_node;
//            position.node->pre = new_node;
//        }
//        return position.node->pre;
//    }
//    template<class T, class Alloc>
//    template<class InputIterator>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::true_type) {
//        return insert_aux(position, size_type(first), last);
//    }
//    template<class T, class Alloc>
//    template<class InputIterator>
//    typename list<T, Alloc>::iterator
//    list<T, Alloc>::insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::false_type) {
//        if (first == last)
//            std::cout << "fuck" << std::endl;
//        for (; first != last; ++first) {
//            insert_aux(position, 1, *first);
//        }
//        return position.node->pre;
//    }
//    template<class T, class Alloc>
//    void  list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
//        if (position != last) {
//            position.node->pre->next = first.node;
//            last.node->pre->next = position.node;
//            first.node->pre->next = last.node;
//            nodePtr tmp = first.node->pre;
//            first.node->pre = position.node->pre;
//            position.node->pre = last.node->pre;
//            last.node->pre = tmp;
//        }
//    }
//
//
//
////    template<class T, class Alloc>
////    typename list<T, Alloc>::const_iterator
////    list<T, Alloc>::convert_iterator_to_const(const iterator& it) const {
////        using nodeP = list_node<const T>*;
////        nodePtr ptr = it.node;
////        list_node<const T> newnode((nodeP)(ptr->pre), (nodeP)(ptr->next), ptr->data);
////        return const_iterator(&newnode); //先前由于list_iterator构造函数的参数缺少const出错
////    }
//
//
//
//
//
//} // namespace tt
//
//
//
//
//
//
//
//#endif //TINYSTL_LIST_H
//
// *
// *
// * */
//
//
//
//
//
