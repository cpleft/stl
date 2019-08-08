/* file		: mystl_list.hpp 
 * by		: waterlemon	429426523@qq.com
 * date		: 2019年 08月 07日 星期三 14:38:05 CST
 * last update	: 
 * 
 * description	: list
 * list本身和list结点是不同的结构，是分开设计的。
 */

#ifndef	    _MYSTL_LIST_
#define	    _MYSTL_LIST_

#include "mystl_alloc.hpp"          /* alloc{}, simple_alloc{} */
#include "mystl_iterator.hpp"       /* iterator_traits, distance() */
#include "mystl_construct.hpp"      /* construct() */

#include <cstddef>      /* ptrdiff_t */

namespace mystl
{

/* list结点: __list_node */
template <typename T>
struct __list_node {
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

/* list迭代器: __list_iterator */
template <typename T, typename Ref, typename Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*>      iterator;
    typedef __list_iterator<T, Ref, Ptr>    self;

    typedef bidirectional_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef Ptr                             pointer;
    typedef Ref                             reference;
    typedef ptrdiff_t                       difference_type;

    typedef __list_node<T>*                 link_type;
    typedef size_t                          size_type;

    link_type node;     /* 迭代器内部的一个指针，指向list结点 */

    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator& x) : node(x.node) {}

    bool operator== (const self& x) const { return node == x.node; }
    bool operator!= (const self& x) const { return node != x.node; }

    reference operator* () const { return (*node).data; }

    pointer operator-> () const { return &(operator*()); }

    self& operator++ ()     /* 前缀 */
    {
        node = (link_type) ((*node).next);
        return *this;
    }
    self operator++ (int)   /* 后缀 */
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator-- ()
    {
        node = (link_type) ((*node).prev);
        return *this;
    }
    self operator-- (int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

/* list 是一个双向链表，也是一个环形链表 */
template <typename T, typename Alloc = alloc>
class list {
protected:
    typedef __list_node<T>  list_node;
    typedef list_node*      link_type;
    typedef simple_alloc<list_node, Alloc>  list_node_allocator;

public:
    typedef __list_iterator<T, T&, T*>      iterator;

public:
    /* 以下定义仅仅用于本容器，与iterator_traits无关。*/
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef typename iterator_traits<iterator>::difference_type  difference_type;
    typedef typename iterator::size_type    size_type;

protected:
    /* 空间置配 */
    link_type get_node() { return list_node_allocator::allocate(); }
    void put_node(link_type p) { return list_node_allocator::deallocate(p); }

    /* 构造 */
    link_type create_node(const_reference x)
    {
        link_type p = get_node();
        construct(&p->data, x);            /* mystl_construct.hpp */
        return p;
    }
    void destroy_node(link_type p)
    {
        destroy(&p->data);
        put_node(p);
    }

protected:
    link_type head;

public:
    iterator begin() const { return (link_type) (head->next);  }
    iterator end() const { return head; }
    bool empty() const { return head->next == head; }
    size_type size() const 
    {
        size_type result = 0;
        result = distance(begin(), end());
        return result;
    }
    reference front() const { return *(begin()); }
    reference back() const { return *(--end()); }

public:
    list() { empty_initialize(); }      /* 产生一个空链表 */
    /* fill */
    explicit list(size_type n)
    {
        empty_initialize();
        while (n--)
            push_back(value_type());
    }
    list(size_type n, const_reference val)
    {
        empty_initialize();
        while (n--)
            push_back(val);
    }
    /* copy */
    list(const list& x)
    {
        empty_initialize();
        //!
        typename list<value_type>::iterator xit = x.begin();
        for (; xit != x.end(); ++xit) {
            insert(end(), *(xit));
        }
    }
    ~list()
    {
        clear();
        destroy_node(head);
    }

protected:
    void empty_initialize() 
    {
        head = get_node();
        head->next = head;
        head->prev = head;
    }

public:
    void push_back(const_reference x) { insert(end(), x); }
    void push_front(const_reference x) { insert(begin(), x); }

    iterator insert(iterator position, const_reference x) /* 在position之前插入 */
    {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        (link_type (position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }

    iterator erase(iterator position)
    {
        if (position == end())
            return position;
        link_type next_node = link_type (position.node->next);
        link_type prev_node = link_type (position.node->prev);
        prev_node->next = next_node;
        next_node->prev = prev_node;

        destroy_node(position.node);
        return iterator (next_node);
    }

    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    void clear()
    {
        link_type cur = (link_type) head->next;
        while (cur != head) {
            link_type tmp = cur;
            cur = (link_type) cur->next;
            destroy_node(tmp);
        }
        head->next = head;
        head->prev = head;
    }

};

}

#endif
