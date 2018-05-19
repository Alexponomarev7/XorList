#ifndef XOR_LIST_H
#define XOR_LIST_H

#include <iostream>

#include "node.h"
#include "iterator.h"
#include <utility>
#include <memory>
#include <cassert>

template <class T, class Allocator = std::allocator<T>>
class XorList {
public:
    using allocator_pointer = Allocator*;
    using allocator_type = Allocator;
    using node = Node<T>;
    using node_pointer = Node<T>*;
    using node_allocator_type = typename Allocator::template rebind<node>::other;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = XorListIterator<T>;
    using reverse_iterator = std::reverse_iterator<XorListIterator<T>>;
private:
    node_pointer _start;
    node_pointer _end;
    size_t _size;
    node_allocator_type _alloc;
public:
    explicit XorList(const allocator_type& alloc = Allocator());
    XorList(size_type count, const T& value = T(), const Allocator& alloc = Allocator());

    XorList(const XorList& other);
    XorList(XorList&& other);

    ~XorList();

    size_type size() const;

    bool empty() const;

    template<typename U>
    void push_back(U&& element);

    template<typename U>
    void push_front(U&& element);

    void pop_back();
    void pop_front();

    void clear();

    XorList &operator= (const XorList& other);
    XorList &operator= (XorList&& other);

    reference back();
    const reference back() const;

    reference front();
    const reference front() const;

    iterator begin();
    iterator end();

    reverse_iterator rbegin();
    reverse_iterator rend();

    void connect(node_pointer left, node_pointer now, node_pointer right);
    void erase(iterator pos);
    template<typename U>
    iterator insert_after(iterator pos, U&& value);
    template<typename U>
    iterator insert_before(iterator pos, U&& value);
};

template<typename T, class Allocator>
XorList<T, Allocator>::XorList(size_type count, const T& value, const Allocator& alloc) : XorList(alloc)
{
    while (count--) {
        push_back(value);
    }
}

template<typename T, class Allocator>
XorList<T, Allocator>::XorList(const XorList& other) : _alloc(other._alloc)
{
    for (auto &x : other) {
        push_back(x);
    }

    _size = other._size;
}

template<typename T, class Allocator>
XorList<T, Allocator>::XorList(XorList&& other) : XorList(other._alloc)
{
    std::swap<T>(_start, other._start);
    std::swap<T>(_end, other._end);
    std::swap<T>(_size, other._size);
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::XorList& XorList<T, Allocator>::operator=(const XorList& other) {
    for (auto &x : other) {
        push_back(x);
    }
    _size = other._size;

    return *this;
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::XorList& XorList<T, Allocator>::operator= (XorList&& other) {
    std::swap<T>(_start, other._start);
    std::swap<T>(_end, other._end);
    std::swap<T>(_size, other._size);
    return *this;
}


template<typename T, class Allocator>
void XorList<T, Allocator>::clear() {
    while (_start != nullptr) {
        pop_back();
    }
};

template<typename T, class Allocator>
XorList<T, Allocator>::XorList(const allocator_type& alloc):
        _start(nullptr), _end(nullptr), _size(0), _alloc(node_allocator_type(alloc)) {}

template<typename T, class Allocator>
XorList<T, Allocator>::~
        XorList() {
    clear();
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::size_type XorList<T, Allocator>::size() const {
    return _size;
}

template<typename T, class Allocator>
bool XorList<T, Allocator>::empty() const {
    return  _size == 0;
};

template<typename T, class Allocator>
template<typename U>
void XorList<T, Allocator>::push_back(U&& el) {
    node_pointer new_node = node_allocator_traits::allocate(_alloc, 1);
    node_allocator_traits::construct(_alloc, new_node, std::forward<U>(el), _end);

    if (_end != nullptr) {
        _end->xors(reinterpret_cast<typename Node<T>::pointer_type>(new_node));
    } else {
        _start = new_node;
    }
    _end = new_node;
    _size++;
};


template<typename T, class Allocator>
template < typename U >
void XorList<T, Allocator>::push_front(U&& el) {
    node_pointer new_node = node_allocator_traits::allocate(_alloc, 1);
    node_allocator_traits::construct(_alloc, new_node, std::forward<U>(el), nullptr, _start);

    if (_start != nullptr) {
        _start->xors(reinterpret_cast<typename Node<T>::pointer_type>(new_node));
    } else {
        _end = new_node;
    }
    _start = new_node;
    _size++;
};

template<typename T, class Allocator>
void XorList<T, Allocator>::erase(iterator pos) {
    if (pos == begin()) {
        pop_front();
    } else if (pos == rbegin()) {
        pop_back();
    } else {
        node_pointer now = **pos, left = **(--pos), right = **(++pos);
        typename Node<T>::pointer_type now_xors = reinterpret_cast<typename Node<T>::pointer_type>(now);
        left->xors(reinterpret_cast<typename Node<T>::pointer_type>(right) ^ now_xors);
        right->xors(reinterpret_cast<typename Node<T>::pointer_type>(left) ^ now_xors);

        node_allocator_traits::destroy(_alloc, now);
        node_allocator_traits::deallocate(_alloc, now, 1);
        _size--;
    }
}

template<typename T, class Allocator>
void XorList<T, Allocator>::pop_front() {
    node_pointer del = _start;

    _start = reinterpret_cast<node_pointer>(_start->neighbours_xor());
    if (_size > 1)
        _start->xors(reinterpret_cast<typename Node<T>::pointer_type>(del));
    else
        _end = nullptr;

    node_allocator_traits::destroy(_alloc, del);
    node_allocator_traits::deallocate(_alloc, del, 1);
    _size--;
};

template<typename T, class Allocator>
void XorList<T, Allocator>::pop_back() {
    node_pointer del = _end;

    _end = reinterpret_cast<node_pointer>(_end->neighbours_xor());
    if (_size > 1)
        _end->xors(reinterpret_cast<typename Node<T>::pointer_type>(del));
    else
        _start = nullptr;

    node_allocator_traits::destroy(_alloc, del);
    node_allocator_traits::deallocate(_alloc, del, 1);
    _size--;
};

template<typename T, class Allocator>
typename XorList<T, Allocator>::reference XorList<T, Allocator>::front() {
    return _start->get();
};

template<typename T, class Allocator>
const typename XorList<T, Allocator>::reference XorList<T, Allocator>::front() const {
    return _start->get();
};

template<typename T, class Allocator>
typename XorList<T, Allocator>::reference XorList<T, Allocator>::back() {
    return _end->get();
};

template<typename T, class Allocator>
const typename XorList<T, Allocator>::reference XorList<T, Allocator>::back() const {
    return _end->get();
};

template<typename T, class Allocator>
typename XorList<T, Allocator>::iterator XorList<T, Allocator>::begin() {
    return iterator(nullptr, _start);
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::iterator XorList<T, Allocator>::end() {
    return iterator(_end, nullptr);
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::reverse_iterator XorList<T, Allocator>::rbegin() {
    return reverse_iterator(nullptr, _end);
}

template<typename T, class Allocator>
typename XorList<T, Allocator>::reverse_iterator XorList<T, Allocator>::rend() {
    return reverse_iterator(_start, nullptr);
}

template<typename T, class Allocator>
void XorList<T, Allocator>::connect(node_pointer left, node_pointer now, node_pointer right) {
    left->xors(reinterpret_cast<typename Node<T>::pointer_type>(right));
    left->xors(reinterpret_cast<typename Node<T>::pointer_type>(now));

    now->xors(reinterpret_cast<typename Node<T>::pointer_type>(left));
    now->xors(reinterpret_cast<typename Node<T>::pointer_type>(right));

    right->xors(reinterpret_cast<typename Node<T>::pointer_type>(now));
    right->xors(reinterpret_cast<typename Node<T>::pointer_type>(left));
}

template<typename T, class Allocator>
template<typename U>
typename XorList<T, Allocator>::iterator XorList<T, Allocator>::insert_after(iterator pos, U&& value) {
    if (pos == rbegin() || pos == --end()) {
        push_back(std::forward<U>(value));
        return rbegin();
    }

    return insert_before(++pos, std::forward<U>(value));
}

template<typename T, class Allocator>
template<typename U>
typename XorList<T, Allocator>::iterator XorList<T, Allocator>::insert_before(iterator pos, U&& value) {
    if (pos == begin() || pos == rend()) {
        push_front(std::forward<U>(value));
        return begin();
    }

    node_pointer new_v = node_allocator_traits::allocate(_alloc, 1);
    node_pointer now = **pos, left = **(pos--);

    node_allocator_traits::construct(_alloc, new_v, std::forward<U>(value), left, now);

    connect(left, new_v, now);

    ++_size;
    return iterator(left, new_v);
}

#endif //XOR_LIST_H