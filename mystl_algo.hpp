/* file		: mystl_algo.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: Thu 22 Aug 2019 10:20:14 PM CST
 * last update	: 
 * 
 * description	: 
 */

#ifndef	    _MYSTL_ALGO_
#define	    _MYSTL_ALGO_

#include "mystl_iterator.hpp"   /* iterator_category(), distance_type, Distance */
#include "mystl_type_traits.hpp"/* __type_traits<>{}, __true_type{}, __false_type{} */


namespace mystl
{

    /* Non-modifying sequence operations: */

    /* for_each */
    template <typename InputIterator, typename Function>
        Function for_each(InputIterator first, InputIterator last, Function f)
        {
            //! can this function performs only read only operations(InputIterator only)? standard stl seems can.
            //! should there be some code to protect it?
            for (; first != last; ++first)
            {
                f(*first);  /* 仿函数f，返回值被忽略 */
            }
            return f;
        }

    /* find */
    /* find() */
    template <typename InputIterator, typename T>
        InputIterator find(InputIterator first, InputIterator last, const T& value)
        {
            /* match **equality** condition */
            for (; first != last; ++first) {
                if (*first == value)
                    return first;
            }
            return first;
        }
    /* find_if() */
    template <typename InputIterator, typename UnaryPredicate>
        InputIterator find_if(InputIterator first, InputIterator last,
                UnaryPredicate pred)
        {
            for (; first != last; ++first) {
                if (pred(*first))
                    return first;
            }
            return first;
        }
    /* find_if_not() */
    template <typename InputIterator, typename UnaryPredicate>
        InputIterator find_if_not(InputIterator first, InputIterator last,
                UnaryPredicate pred)
        {
            for (; first != last; ++first) {
                if (!pred(*first))
                    return first;
            }
            return first;
        }

    /* find_end() */
    /* 为了更好的性能，这个函数的实现应该区分ForwardIterator和
     * BidirectionalIterator, BidirectionalIterator可以倒着查找 */
    /* 设计：双层架构，上层为dispatch function，下层是对应不同迭代
     * 器的不同实现*/
    /* dispatch */
    template <typename ForwardIterator1, typename ForwardIterator2>
        ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator2 first2, ForwardIterator2 last2)
        {
            typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
            typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

            return __find_end(first1, last1, first2, last2, category1(), category2());
        }
    /* for ForwardIterator */
    template <typename ForwardIterator1, typename ForwardIterator2>
        ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator2 first2, ForwardIterator2 last2,
                forward_iterator_tag, forward_iterator_tag)
        {
            //! KMP should be better
            if (first2 == last2) return last1;

            ForwardIterator1 ret = last1;

            for (; first1 != last1; ++first1) {
                ForwardIterator1 it1 = first1;
                ForwardIterator2 it2 = first2;
                while (*it1 == *it2) {
                    ++it1; ++it2;
                    if (it2 == last2) { ret = first1; break; }
                    if (it1 == last1) return ret;
                }
            }
            return ret;
        }
    /* for BidirectionalIterator */
    //! 这里需要用到reverse_iterator，有关于适配器的概念，以后实现
    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
        BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                bidirectional_iterator_tag, bidirectional_iterator_tag)
        {
            return __find_end(first1, last1, first2, last2, forward_iterator_tag(), forward_iterator_tag());
        }

    /* find_end() */
    /* dispatch */
    template <typename ForwardIterator1, typename ForwardIterator2,
             typename BinaryPredicate>
        ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator2 first2, ForwardIterator2 last2,
                BinaryPredicate pred)
        {
            typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
            typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

            return __find_end(first1, last1, first2, last2, pred, category1(), category2());
        }
    /* for ForwardIterator */
    template <typename ForwardIterator1, typename ForwardIterator2,
             typename BinaryPredicate>
        ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator2 first2, ForwardIterator2 last2,
                BinaryPredicate pred,
                forward_iterator_tag, forward_iterator_tag)
        {
            if (first2 == last2) return last1;

            ForwardIterator1 ret = last1;

            for (; first1 != last1; ++first1) {
                ForwardIterator1 it1 = first1;
                ForwardIterator2 it2 = first2;
                while (pred(*it1, *it2)) {
                    ++it1; ++it2;
                    if (it2 == last2) { ret = first1; break; }
                    if (it1 == last1) return ret;
                }
            }
            return ret;
        }
    /* for BidirectionalIterator */
    //! 这里需要用到reverse_iterator，有关于适配器的概念，以后实现
    template <typename BidirectionalIterator1, typename BidirectionalIterator2,
             typename BinaryPredicate>
        BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                BinaryPredicate pred,
                bidirectional_iterator_tag, bidirectional_iterator_tag)
        {
            return __find_end(first1, last1, first2, last2, pred, forward_iterator_tag(), forward_iterator_tag());
        }

    /* find_first_of */
    /* find_first_of() */
    template <typename InputIterator, typename ForwardIterator>
        InputIterator find_first_of(InputIterator first1, InputIterator last1,
                ForwardIterator first2, ForwardIterator last2)
        {
            for (; first1 != last1; ++first1) {
                for (ForwardIterator it = first2; it != last2; ++it)
                    if (*first1 == *it) return first1;
            }
            return last1;
        }
    /* find_first_of() */
    template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
        InputIterator find_first_of(InputIterator first1, InputIterator last1,
                ForwardIterator first2, ForwardIterator last2,
                BinaryPredicate pred)
        {
            for (; first1 != last1; ++first1) {
                for (ForwardIterator it = first2; it != last2; ++it)
                    if (pred(*first1, *it)) return first1;
            }
            return last1;
        }


}

#endif

