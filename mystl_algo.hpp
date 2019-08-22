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
            //! can this function performs only read only operations(InputIterator only)? standard stl seems so.
            //! should there be some code to protect it?
            for (; first != last; ++first)
            {
                f(*first);  /* 仿函数f，返回值被忽略 */
            }
            return f;
        }



}

#endif

