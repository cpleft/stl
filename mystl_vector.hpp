/* file		: mystl_vector.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 06日 星期二 10:19:46 CST
 * last update	: 
 * 
 * description	: vector
 */

#ifndef     _MYSTL_VECTOR_
#define     _MYSTL_VECTOR_

#include "mystl_alloc.hpp"      /* simple_alloc{}, alloc{} */
#include "mystl_construct.hpp"  /* destroy(), construct() */
#include "mystl_algobase.hpp"   /* copy(), copy_backward(), fill(), max() */
#include "mystl_uninitialized.hpp"  /* uninitialized_fill_n(), uninitialized_copy() */

#include <cstddef>              /* size_t, ptrdiff_t */

namespace mystl
{
template <typename T, typename Alloc = alloc>
class vector {
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef ptrdiff_t       difference_type;

    typedef size_t          size_type;
    typedef value_type*     iterator;       /* vector 的迭代器是普通指针 */
    /* operator*, operator->, operator++, operator--, operator+, operator-
     * operator+=, operator-= 这些操作普通指针天生具备 
     * 所以 vector 的迭代器是 RandomAccessIterator */

protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;             /* 目前使用空间的头 */
    iterator finish;            /* 目前使用空间的尾 */
    iterator end_of_storage;    /* 目前可用空间的尾 */

    void fill_initialize(size_type n, const T& value)
    {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    void deallocate()
    {
        if (start)
            data_allocator::deallocate(start, end_of_storage - start);
    }

public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return size_type(end() - begin()); }
    size_type capacity() { return size_type(end_of_storage - begin()); }
    bool empty() { return begin() == end(); }
    reference operator[] (size_type n) { return *(begin() + n); }

public:
    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    vector(int n, const T& value) { fill_initialize(n, value); }
    vector(long n, const T& value) { fill_initialize(n, value); }
    explicit vector(size_type n) { fill_initialize(n, T()); }
    ~vector() 
    {
        destroy(start, finish);             /* mystl_construct.hpp */
        deallocate();                       /* member fun */
    }

public:
    reference front() { return *begin(); }
    reference back() { return *(end() -1); }
    void push_back(const T& x) 
    {
        if (finish != end_of_storage) {
            construct(finish, x);           /* mystl_construct.hpp */
            ++finish;
        }
        else
            insert(end(), x);           /* member fun */
    }
    void pop_back() 
    {
        --finish;
        destroy(finish);
    }

    iterator erase(iterator position)
    {
        if (position == end())
            return position;
        if (position + 1 != end())
            copy(position + 1, finish, position);   /* 后续元素向前移动1格 */
        --finish;
        destroy(finish);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator i = copy(last, finish, first);     /* mystl_algobase.hpp */
        destroy(i, finish);                         /* mystl_construct.hpp */
        finish = finish - (last -first);
        return first;
    }
    void resize(size_type new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);    /* member fun */
    }
    void resize(size_type new_size)
    {
        resize(new_size, T());
    }
    void clear() { erase(begin(), end()); }
    void insert(iterator position, size_type n, const T& x);
    void insert(iterator position, const T& x);

protected:
    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);         /* mystl_uninitialized.hpp */
        return result;
    }

};

/* insert() */
template <typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, const T& x)
{
    if (finish != end_of_storage) {     /* 还有备用空间 */
        /* 在备用空间的起始处构造一个元素，以 vector 最后一个元素为初值 */
        construct(finish, *(finish-1));
        ++finish;
        copy_backward(position, finish-2, finish-1);    /* mystl_algobase.hpp */
        *position = x;
    }
    else {
        const size_type old_size = size();
        const size_type new_size = old_size != 0 ? 2 * old_size : 1;
        /* 配置原则: 增大2倍 */

        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish = new_start;
        try {
            /* 拷贝原 vector 内容到新的 vector 中 */
            new_finish = uninitialized_copy(start, position, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = uninitialized_copy(position, finish, new_finish);
        } catch (...) {
            /* commit or rollback */
            destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, new_size);
            throw;
        }
        /* 析构，释放 原vector */
        destroy(begin(), end());            /* mystl_construct.hpp */
        deallocate();                       /* member fun */

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

/* insert() */
template <typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
    if (n == 0) return;
    if (size_type(end_of_storage - finish) >= n) {  /* 备用空间足够 */
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n) {
            uninitialized_copy(finish-n, finish, finish);   /* mystl_algobase.hpp */
            finish += n;
            copy_backward(position, old_finish-n, old_finish);
            fill(position, position+n, x);          /* mystl_algobase.hpp */
        }
        else {
            uninitialized_fill_n(finish, n-elems_after, x); /* mystl_algobase.hpp */
            finish += n - elems_after;
            uninitialized_copy(position, old_finish, finish);
            finish += elems_after;
            fill(position, old_finish, x);
        }
    }
    else {
        const size_type old_size = size();
        const size_type new_size = old_size + max(old_size, n); /* mystl_algobase.hpp */
        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start, position, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, x);
            new_finish = uninitialized_copy(position, finish, new_finish);
        } catch(...) {
            /* commit or rollback */
            destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, new_size);
            throw;
        }
        destroy(start, finish);
        deallocate();       /* member fun */

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

}

#endif
