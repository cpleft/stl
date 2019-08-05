/* file		: mystl_alloc.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 05日 星期一 09:47:29 CST
 * last update	: 
 * 
 * description	: simple_alloc, alloc, malloc_alloc
 */

#ifndef     _MYSTL_ALLOC_
#define     _MYSTL_ALLOC_

#include <cstdlib>      /* malloc(), free(), realloc() */

#if 0
#include <new>
#define     __THROW_BAD_ALLOC       throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream>
#define     __THROW_BAD_ALLOC       std::cerr << "out of memory" << std::endl; exit(1)
#endif

namespace mystl
{

class malloc_alloc {
private:
    // oom: out of memory
    static void* oom_malloc(size_t n);
    static void* oom_realloc(void* p, size_t n);
    static void  (*__malloc_alloc_oom_handler)();

public:
    static void* allocate(size_t n)
    {
        void* result = malloc(n);
        // if cannot satisfy require
        if (result == 0)
            result = oom_malloc(n);
        return result;
    }

    static void deallocate(void* p, size_t /* n */)
    {
        free(p);
    }

    static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
    }

    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }

};

/* malloc_alloc oom handling */
void (*malloc_alloc::__malloc_alloc_oom_handler)() = 0;

void* malloc_alloc::oom_malloc(size_t n)
{
    void (*my_malloc_handler)();
    void* result;

    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == 0) 
            { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) 
            return result;
    }
}

void* malloc_alloc::oom_realloc(void* p, size_t n)
{
    void (*my_malloc_handler)();
    void* result;

    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == 0) 
            { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) 
            return result;
    }
}

typedef     malloc_alloc        alloc;

template <typename T, typename Alloc = alloc>
class simple_alloc {
public:
    static T* allocate(size_t n)
        { return n == 0 ? 0 : (T*) Alloc::allocate(n * sizeof (T)); }
    static T* allocate(void)
        { return (T*) Alloc::allocate(sizeof (T)); }
    static void deallocate(T* p, size_t n)
        { if (n != 0) Alloc::deallocate(p, n * sizeof (T)); }
    static void deallocate(T* p)
        { Alloc::deallocate(p, sizeof (T)); }
};

}

#endif
