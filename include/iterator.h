//
// Created by boyuan on 2022/5/5.
//

#ifndef TINYSTL_ITERATOR_H
#define TINYSTL_ITERATOR_H



namespace tt {

    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag :public input_iterator_tag {};
    struct bidirectional_iterator_tag :public forward_iterator_tag {};
    struct random_access_iterator_tag :public bidirectional_iterator_tag {};

    // 迭代器基类
    /**
     * 迭代器基类
     * 每个新设计的迭代器都继承自它，就能保证符合 STL 的规范，以免漏写属性。
     *
     * 设计适当的对应类型，是迭代器的职责。
     * 设计适当的迭代器，是容器的职责。因为只用容器才知道设计怎么的迭代器来遍历自己。
     *
     * @tparam Category 迭代器类型
     * @tparam T 迭代器所指对象的类型
     * @tparam Distance 迭代器之间的距离的类型
     * @tparam Pointer 迭代器所指对象的指针
     * @tparam Reference 迭代器所指对象的引用
     */
    template <class Category,
            class T,
            class Distance = std::ptrdiff_t,
            class Pointer = T*,
            class Reference = T&>
    struct iterator_base {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = Pointer;
        using reference         = Reference;
    };


    // 迭代器traits
    template <class Iterator>
    struct iterator_traits {
        using iterator_category = typename Iterator::iterator_category; // 迭代器类型
        using value_type        = typename Iterator::value_type;        // 迭代器所指的对象的类型
        using difference_type   = typename Iterator::difference_type;   // 两个迭代器之间的距离
        using pointer           = typename Iterator::pointer;           // 迭代器所指的对象的指针
        using reference         = typename Iterator::reference;         // 迭代器所指的对象的引用
    };

    // 对于普通的指针或常量指针，提供具体化模板(视为随机访问迭代器)
    template <class T>
    struct iterator_traits<T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t ;
        using pointer           = T*;
        using reference         = T&;
    };

    template <class T>
    struct iterator_traits<const T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t ;
        using pointer           = T*;
        using reference         = T&;
    };


    // 以下提供几个方法,用来获取输入迭代器的iterator_category、distance_type、value_type对应的对象

    /**
     * 获取迭代器类型
     * @tparam Itertor 迭代器
     * @return 迭代器类型对象
     */
    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(Iterator &) {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    /**
     * 获取迭代器的 difference type
     * @tparam Iterator 迭代器
     * @return difference type 指针
     */
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type *
    distance_type(const Iterator&) {
        using differenceType = typename iterator_traits<Iterator>::difference_type;
        return static_cast<differenceType*>(0);
    }

    /**
     * 获取迭代器的对象类型
     * @tparam Iterator 迭代器
     * @return 对象指针
     */
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type *
    value_type(const Iterator&) {
        using valueType = typename iterator_traits<Iterator>::value_type;
        return static_cast<valueType*>(0);
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


    template <class InputIterator>
    inline InputIterator next(InputIterator it,
                              typename iterator_traits<InputIterator>::difference_type n = 1) {
        advance(it, n);

        return it;
    }



}  // namespace







#endif //TINYSTL_ITERATOR_H
