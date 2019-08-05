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

/* 特化 */

}

#endif
