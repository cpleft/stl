/* file		: mystl_iterator.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 05日 星期一 14:32:50 CST
 * last update	: 
 * 
 * description	: 设计适当的相应型别，是迭代器的责任。设计适当的迭代器，则是容器的责任。
 *      input_iterator_tag{}, output_iterator_tag{}, 
 *      forward_iterator_tag{}, bidirectional_iterator_tag{},
 *      random_access_iterator_tag{}
 *      iterator{}          供自行设计迭代器时继承
 *      iterator_traits{}
 *      iterator_category(), difference_type(), value_type()
 */

#ifndef     _MYSTL_ITERATOR_
#define     _MYSTL_ITERATOR_

#include <cstddef>   /* ptrdiff_t */

namespace mystl
{

/* classes */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename Category, typename T,
         typename Distance = ptrdiff_t,
         typename Pointer = T*,
         typename Reference = T&>
struct iterator {
    typedef Category        iterator_category;
    typedef T               value_type;
    typedef Distance        difference_type;
    typedef Pointer         pointer;
    typedef Reference       reference;
};

template <typename Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

/* iterator_traits partial specialization for native pointer */
template <typename T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

/* iterator_traits partial specialization for native pointer */
template <typename T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
};

/* functions */
/* return Iterator's iterator_category */
template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category   category;
    return category();
}

/* return Iterator's difference_type* */
template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

/* return Iterator's value_type* */
template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}


/* distance() */
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first; ++n;
    }
    return n;
}

template <typename BidirectionalIterator>
inline typename iterator_traits<BidirectionalIterator>::difference_type
__distance(BidirectionalIterator first, BidirectionalIterator last,
        bidirectional_iterator_tag)
{
    typename iterator_traits<BidirectionalIterator>::difference_type n = 0;
    while (first != last) {
        ++first; ++n;
    }
    return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
        random_access_iterator_tag)
{
    return last - first;
}

}


#endif
