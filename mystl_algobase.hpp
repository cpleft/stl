/* file		: mystl_algorithm.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 06日 星期二 13:03:39 CST
 * last update	: 
 * 
 * description	: copy(), fill(), fill_n()
 */

#ifndef	    _MYSTL_ALGORITHM_
#define	    _MYSTL_ALGORITHM_

#include "mystl_iterator.hpp"   /* iterator_category(), distance_type, Distance */
#include "mystl_type_traits.hpp"/* __type_traits<>{}, __true_type{}, __false_type{} */

#include <cstring>      /* memmove() */
#include <cstddef>      /* ptrdiff_t */

namespace mystl
{

/* fill() */
template <typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first)
        *first = value;
}

/* fill_n() */
template <typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
    for (; n > 0; n--, ++first)
        *first = value;
    return first;
}


/* __copy_dispatch<>{} 完全泛化版本 */
template <typename InputIterator, typename OutputIterator>
struct __copy_dispatch {
    OutputIterator operator() (InputIterator first, 
            InputIterator last, OutputIterator result)
    {
        return __copy(first, last, result, iterator_category(first));
    }
};

/* copy() */
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

/* 重载 copy (1) 针对 char* */
inline char* copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}
/* 重载 copy (2) 针对 wchar_t* */
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof (wchar_t) * (last - first));
    return result + (last - first);
}

/* __copy_dispatch<>{} 偏特化版本 (1) */
template <typename T>
struct __copy_dispatch<T*, T*> {
    T* operator() (T* first, T* last, T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

/* __copy_dispatch<>{} 偏特化版本 (2) */
template <typename T>
struct __copy_dispatch<const T*, T*> {
    T* operator() (const T* first, const T* last, T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};


/* 接着 __copy_dispatch<>{} 完全泛化版本，针对不同的迭代器类型 */
// for InputIterator
template <typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
        OutputIterator result, input_iterator_tag)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

// for RandomAccessIterator
template <typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator
__copy(RandomAccessIterator first, RandomAccessIterator last, 
        OutputIterator result, random_access_iterator_tag)
{
    return __copy_d(first, last, result, distance_type(first));
}

template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last, 
        OutputIterator result, Distance*)
{
    /* 以 n 来控制循环，速度更快 */
    for (Distance n = last - first; n > 0; n--, ++result, ++first)
        *result = *first;
    return result;
}

/* 接着 __copy_dispatch<>{} 偏特化版本，针对是否 has_trivial_assignment_operator */
template <typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
    memmove(result, first, sizeof (T) * (last - first));
    return result + (last - first);
}

template <typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
    /* 原生指针也是一种 RandomAccessIterator */
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}


}

#endif