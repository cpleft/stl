/* file		: mystl_numeric.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: Wed 21 Aug 2019 05:22:43 PM CST
 * last update	: 
 * 
 * description	: numeric algorithm
 */

#ifndef	    _MYSTL_NUMERIC_
#define	    _MYSTL_NUMERIC_

#include "mystl_iterator.hpp"   /* iterator_category(), distance_type, Distance */
#include "mystl_type_traits.hpp"/* __type_traits<>{}, __true_type{}, __false_type{} */

#include <cstddef>      /* ptrdiff_t */

namespace mystl
{

    template <typename InputIterator, typename T>
        T accumulate(InputIterator first, InputIterator last, T init)
        {
            for (; first != last; ++first) {
                init += *first;
            }
            return init;
        }
    template <typename InputIterator, typename T, typename BinaryOperation>
        T accumulate(InputIterator first, InputIterator last, T init,
                BinaryOperation binary_op)
        {
            for (; first != last; ++first) {
                init = binary_op(init, *first);
            }
            return init;
        }


    
}

#endif
