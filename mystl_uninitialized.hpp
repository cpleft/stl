/* file		: mystl_uninitialized.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 05日 星期一 12:04:03 CST
 * last update	: 
 * 
 * description	: uninitialized_copy(), uninitialized_fill(), uninitialized_fill_n()
 * 与 construct(), destroy() 一样，作用于已经申请而没有初始化的空间上。
 */

#ifndef     _MYSTL_UNINITIALIZED_
#define     _MYSTL_UNINITIALIZED_

#include "mystl_type_traits.hpp"    /* __true_type{}, __false_type __type_traits<>{} */
#include "mystl_iterator.hpp"       /* value_type() */
#include "mystl_construct.hpp"      /* construct() */
#include "mystl_algobase.hpp"       /* copy(), fill(), fill_n() */ //!

#include <cstring>          /* memmove() */

namespace mystl
{

/* (1) uninitialized_copy()
 * first 指向输入端（欲复制的空间的）起始处 
 * last  指向输入端（欲复制的空间的）结束处（前闭后开区间）
 * result指向输出端（欲初始化空间的）起始处
 * 先利用 iterator_traits 萃取出迭代器 result 的 value_type, 然后判断是否为 POD 型别 */
template <typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, 
        ForwardIterator result, T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator 
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    __uninitialized_copy(first, last, result, value_type(result));
}

/* 如果型别是 POD 类型，则采取 mystl 算法 copy() 最有效率
 * 如果不是，则需要对每个迭代器指向的元素 复制构造*/
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, 
        ForwardIterator result, __true_type)
{
    return copy(first, last, result);   /* mystl::copy() 算法 */ 
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, 
        ForwardIterator result, __false_type)
{
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur)
        construct(&*cur, *first);       /* mystl_construct.hpp::construct() */
    return cur;         /* return 'last' of initialized space */
}

/* 针对 char*, wchar_t* 两种型别可以采用最有效率的做法: memmove() 执行复制  */
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}
inline wchar_t* uninitialized_copy(const wchar_t* first, 
        const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof (wchar_t) * (last - first));
    return result + (last - first);
}


/* (2) uninitialized_fill() 
 * first 指向输出端（欲初始化空间的）起始处 
 * last  指向输出端（欲初始化空间的）结束处（前闭后开区间）
 * x 为指定的初值 
 * 先利用 iterator_traits 萃取出迭代器 first 的 value_type, 然后判断是否为 POD 型别 */
template <typename ForwardIterator, typename T, typename T2>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, 
        const T& x, T2*)
{
    typedef typename __type_traits<T2>::is_POD_type is_POD;
    __uninitialized_fill_aux(first, last, x, is_POD());
}

template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    __uninitialized_fill(first, last, x, value_type(first));
}

/* 如果型别是 POD 类型，则采取 mystl 算法 fill() 最有效率
 * 如果不是，则需要对每个迭代器指向的元素 复制构造*/
template <typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, 
        const T& x, __true_type)
{
    fill(first, last, x);       /* mystl 算法 fill */
}

template <typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, 
        const T& x, __false_type)
{
    ForwardIterator cur = first;
    //! 异常处理
    for (; cur != last; ++cur)
        construct(&*cur, x);
}


/* (3) uninitialized_fill_n() 
 * first 指向输出端（欲初始化空间的）起始处 
 * n 表示欲初始化空间的大小
 * x 为指定的初值 
 * 先利用 iterator_traits 萃取出迭代器 first 的 value_type, 然后判断是否为 POD 型别 */
template <typename ForwardIterator, typename Size, typename T, typename T2>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T2*)
{
    typedef typename __type_traits<T2>::is_POD_type is_POD;
    __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

/* 如果型别是 POD 类型，则采取 mystl 算法 fill_n() 最有效率
 * 如果不是，则需要对每个迭代器指向的元素 复制构造*/
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
    return fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
    ForwardIterator cur = first;
    for (; n > 0; n--, ++cur)
        construct(&*cur, x);
    return cur;
}

}

#endif
