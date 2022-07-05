//
// Created by boyuan on 2022/5/5.
//

#ifndef TINYSTL_LIST_H
#define TINYSTL_LIST_H

#include <cstddef>
#include <initializer_list>
#include <limits>

#include "iterator.h"
#include "allocator.h"
#include "construct.h"
#include "type_traits.h"
#include "algorithm.h"


namespace tt {

    template<class T>
    struct list_node {
        list_node<T>* prev;
        list_node<T>* next;
        T data;
        list_node() : prev(nullptr), next(nullptr), data(T()) {}
        list_node(list_node<T>* p, list_node<T>* n, const T& d) : prev(p), next(n), data(d) {}
    };

    template<class T, class Ref, class Ptr>
    struct list_iterator : public iterator_base<bidirectional_iterator_tag, T> {
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
        typedef size_t                      size_type;
        typedef bidirectional_iterator_tag  iterator_category;
        typedef list_node<T>*               node_ptr;

        node_ptr node;

        list_iterator() {}
        list_iterator(const node_ptr n) : node(n) {}
        list_iterator(const list_iterator & other) : node(other.node) {}
        bool operator==(const list_iterator & other) const { return node == other.node; }
        bool operator!=(const list_iterator & other) const { return node != other.node; }
        reference operator*() const { return node->data; }
        pointer operator->() const { return &(operator*()); }
        list_iterator& operator++() { node = node->next; return *this; }
        list_iterator operator++(int) { node_ptr tmp = node; ++*this; return tmp; }
        list_iterator& operator--() { node = node->prev; return *this; }
        list_iterator operator--(int) { node_ptr tmp = node; --*this; return tmp; }
    };




    template<class T, class Alloc = allocator<T>>
    class list {
    public:
        typedef T                       value_type;
        typedef size_t                  size_type;
        typedef T&                      reference;
        typedef T*                      pointer;
        typedef const T&                const_reference;
        typedef const T*                const_pointer;

        typedef list_node<T>*                           link_type;
        typedef list_iterator<T, T&, T*>                iterator;
        typedef list_iterator<T, const T&, const T*>    const_iterator;
        typedef list<T, Alloc>                          self;

    private:
        typedef typename Alloc::template rebind<list_node<T>>::other     list_allocator;

        link_type head;
    public:
        //构造，拷贝，赋值，析构
        list() { empty_init(); }
        list(size_type n, const value_type& x = value_type());
        template<class InputIterator>
        list(InputIterator first, InputIterator last);
        list(const list & other);
        list& operator=(const list & other);
        ~list();

        iterator begin() { return head->next; }  //node->next是list_node<T>*,可以用改返回值初始化iterator
        iterator end() { return head; }
        const_iterator begin() const { return head->next; } //const this 使用
        const_iterator end() const { return head; }
        bool  empty() { return head->next == head; }
        size_type size();

        void push_back(const value_type& x);
        void push_front(const value_type& x);
        iterator insert(iterator position, const size_type& n, const value_type& x);
        template<class InputIterator>
        iterator insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator iter);
        iterator erase(iterator first, iterator last);
        void pop_front();
        void pop_back();
        void clear();
        void remove(const value_type& x);
        void unique(); //只能删除连续的重复元素
        void splice(iterator position, list& x); //不能连接list本身,position不能在first到last之间，下同
        void splice(iterator position, iterator first, iterator last);
        void splice(iterator position, list& l, iterator i);
        void reverse();
        void merge(list &); //两个list必须先递增排序
        void swap(list & other);
        void sort();

        bool operator==(const list &other) const { return head == other.head; }
        bool operator!=(const list &other) const { return head != other.head; }

    private:
        //分配空间并初始化
        link_type create_node(const value_type& x);
        void destroy_node(link_type p);
        void empty_init();
        iterator insert_aux(iterator position, const size_type& n, const value_type& x);
        template<class InputIterator>
        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::true_type);
        template<class InputIterator>
        iterator insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::false_type);
        void transfer(iterator position, iterator first, iterator last);
        void delete_list();

    };

    //构造，拷贝，赋值，析构
    template<class T, class Alloc>
    list<T, Alloc>::list(size_type n, const value_type& x) {
        empty_init();
        insert(end(), n, x);
    }
    template<class T, class Alloc>
    template<class InputIterator>
    list<T, Alloc>::list(InputIterator first, InputIterator last) {
        empty_init();
        insert(end(), first, last);

    }
    template<class T, class Alloc>
    list<T, Alloc>::list(const list & other) { //必须要定begin()和end()的const版本，传入const list使用const this调用成员函数,无法找到合适的const begin().
        empty_init();
        insert(end(), other.begin(), other.end());
    }
    template<class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(const list & other) {
        if (*this != other) {
            clear();
            insert(end(), other.begin(), other.end());
        }
        return *this;
    }
    template<class T, class Alloc>
    list<T, Alloc>::~list() {
        delete_list();
    }

    //容器操作
    template<class T, class Alloc>
    typename list<T, Alloc>::size_type
    list<T, Alloc>::size() {
        return (size_type)distance(begin(), end());
    }

    //    template<class T, class Alloc>
    //    typename list<T, Alloc>::const_iterator
    //    list<T,Alloc>::begin() const {
    //        auto tmp = (list* const)this;
    //        iterator iter(tmp->node->next);
    //        return  convert_iterator_to_const(iter);
    //    }
    //    template<class T, class Alloc>
    //    typename list<T, Alloc>::const_iterator
    //    list<T, Alloc>::end() const {
    //        auto tmp = (list* const)this;
    //        iterator iter(tmp->node);
    //        return convert_iterator_to_const(iter);
    //    }

    template<class T, class Alloc>
    void
    list<T, Alloc>::push_back(const value_type& x) {
        insert_aux(end(), 1, x);
    }
    template<class T, class Alloc>
    void
    list<T, Alloc>::push_front(const value_type& x) {
        insert_aux(begin(), 1, x);
    }
    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(iterator position, const size_type& n, const value_type& x) {
        return insert_aux(position, n, x);
    }
    template<class T, class Alloc>
    template<class InputIterator>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        return insert_range_aux(position, first, last, typename tt::is_integral<InputIterator>::type());
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator iter) {
        link_type next_node = iter.node->next;
        iter.node->prev->next = iter.node->next;
        iter.node->next->prev = iter.node->prev;
        destroy_node(iter.node);
        return iterator(next_node);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(list::iterator first, list::iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return first;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_front() {
        erase(begin());
    }
    template<class T, class Alloc>
    void list<T, Alloc>::pop_back() {
        auto tmp = end();
        erase(--tmp);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::clear() {
        erase(begin(), end());
    }
    template<class T, class Alloc>
    void list<T, Alloc>::remove(const value_type& x) {
        auto curr = begin();
        auto last = end();
        while (curr != last) {
            if (*curr == x)
                erase(curr++);
            else
                ++curr;
        }
    }
    template<class T, class Alloc>
    void list<T, Alloc>::unique() {
        auto first = begin();
        auto next = first;
        ++next;
        if (first == end() || next == end()) return;
        while (next != end()) {
            if (*first == *next) {
                erase(next++); //next失效
            }
            else {
                first = next;
                ++next;
            }
        }
    }
    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& x) {
        transfer(position, x.begin(), x.end());
    }
    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, iterator first, iterator last) {
        if (first != last)
            transfer(position, first, last);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& l, iterator i) {
        iterator j = i;
        ++j;
        if (position == i || position == j)  return;
        transfer(position, i, j);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::reverse() {
        auto first = begin();
        auto curr = first;
        ++curr;
        auto last = end();
        if (first == last || ++curr = last) return;
        while (begin() != last) {
            transfer(begin(), curr, ++curr);
        }
    }
    template<class T, class Alloc>
    void list<T, Alloc>::merge(list& l) {
        auto first1 = begin();
        auto last1 = end();
        auto first2 = l.begin();
        auto last2 = l.end();
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
                //transfer(first1, first2, ++first2); //不能使用，运行时先进行加法运算，之后才对三个参数进行拷贝构造
            }
            else
                ++first1;
        }
        if (first2 != last2)
            transfer(last1, first2, last2);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::swap(list & other) {
        tt::swap(head, other.node);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::sort() {
        if (head->next == head || head->next->next == head) return;
        list carry;
        list counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i - 1]);
        swap(counter[fill - 1]);
    }
    //private函数
    template<class T, class Alloc>
    typename list<T, Alloc>::link_type
    list<T, Alloc>::create_node(const value_type& x) {
        link_type position = list_allocator::allocate();
        list_allocator::construct(position, list_node<T>(nullptr, nullptr, x));
        return position;
    }
    template<class T, class Alloc>
    void list<T, Alloc>::destroy_node(list::link_type p) {
        list_allocator::destroy(p);
        list_allocator::deallocate(p);
    }
    template<class T, class Alloc>
    void list<T, Alloc>::empty_init() {
        head = list_allocator::allocate();
        head->next = head;
        head->prev = head;
    }
    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_aux(iterator position, const size_type& n, const value_type& x) {
        auto i = n;
        for (; i > 0; --i) {
            auto new_node = create_node(x);   //iterator是调用node的接口，一般在参数以及返回当中
            new_node->next = position.node;
            new_node->prev = position.node->prev;
            position.node->prev->next = new_node;
            position.node->prev = new_node;
        }
        return position.node->prev;
    }
    template<class T, class Alloc>
    template<class InputIterator>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::true_type) {
        return insert_aux(position, size_type(first), last);
    }
    template<class T, class Alloc>
    template<class InputIterator>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_range_aux(iterator position, InputIterator first, InputIterator last, tt::false_type) {
        if (first == last) {
            std::cout << "error" << std::endl;
            return position;
        }
        for (; first != last; ++first) {
            insert_aux(position, 1, *first);
        }
        return position.node->prev;
    }
    template<class T, class Alloc>
    void  list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
        if (position != last) {
            position.node->prev->next = first.node;
            last.node->prev->next = position.node;
            first.node->prev->next = last.node;
            link_type tmp = first.node->prev;
            first.node->prev = position.node->prev;
            position.node->prev = last.node->prev;
            last.node->prev = tmp;
        }
    }

    template<class T, class Alloc>
    void
    list<T, Alloc>::delete_list() {
        erase(begin(), end());
        destroy_node(head);
    }





    //    template<class T, class Alloc>
    //    typename list<T, Alloc>::const_iterator
    //    list<T, Alloc>::convert_iterator_to_const(const iterator& it) const {
    //        using nodeP = list_node<const T>*;
    //        nodePtr ptr = it.node;
    //        list_node<const T> newnode((nodeP)(ptr->prev), (nodeP)(ptr->next), ptr->data);
    //        return const_iterator(&newnode); //先前由于list_iterator构造函数的参数缺少const出错
    //    }





} // namespace tt







#endif //TINYSTL_LIST_H

