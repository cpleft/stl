/* file		: mystl_construct.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 05日 星期一 11:12:00 CST
 * last update	: 
 * 
 * description	: construct(), destroy()
 */

#ifndef     _MYSTL_CONSTRUCT_
#define     _MYSTL_CONSTRUCT_

#include <new>
#include "mystl_iterator.hpp"       /* value_type() */
#include "mystl_type_traits.hpp"    /* __false_type{}, __type_traits<>{} */

namespace mystl
{

template <typename T1, typename T2>
inline void construct(T1* p, const T2& value)
{
    new (p) T1(value);  // placement new
}

template <typename T>
inline void destroy(T* p)
{
    p->~T();
}

/* 判断元素的型别是否有 trivial destructor */
template <typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

/* destroy 第二版本 
 * 接收两个迭代器*/
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

/* 如果元素的型别有 non-trivial destructor */
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first != last; ++first)
        destroy(&*first);
}

/* 如果元素的型别有 trivial destructor */
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator , ForwardIterator , __true_type) {}


/* destroy() 第二版本针对迭代器为 char*, wchar_t* 的特化版 */
inline void destroy(char* , char*) {}
inline void destroy(wchar_t* , wchar_t*) {}

}

#endif
