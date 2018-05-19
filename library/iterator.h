#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>

#include "node.h"

template<typename T>
struct XorListIterator : public std::iterator <std::bidirectional_iterator_tag, T> {
protected:
    using node = Node<T>;
    using node_pointer = Node<T>*;

    typename Node<T>::pointer_type _current;
    typename Node<T>::pointer_type _previous;
public:
    XorListIterator(const XorListIterator<T>& other) = default;
    XorListIterator& operator=(const XorListIterator<T>& other) = default;

    XorListIterator(node_pointer prev = nullptr, node_pointer cur = nullptr);

    XorListIterator& operator++();
    XorListIterator operator++(int);

    XorListIterator& operator--();
    XorListIterator operator--(int);

    node& operator*() const;

    bool operator==(const XorListIterator<T>& other);
    bool operator!=(const XorListIterator<T>& other);
};

template<typename T>
XorListIterator<T>::XorListIterator(node_pointer prev, node_pointer cur)
        : _previous(reinterpret_cast<typename Node<T>::pointer_type>(prev))
        , _current(reinterpret_cast<typename Node<T>::pointer_type>(cur)) {}


template<typename T>
bool XorListIterator<T>::operator==(const XorListIterator<T>& other) {
    return _current == other._current && _previous == other._previous;
}

template<typename T>
bool XorListIterator<T>::operator!=(const XorListIterator<T>& other) {
    return !(XorListIterator<T>::operator==(other));
}

template<typename T>
XorListIterator<T> XorListIterator<T>::operator++(int) {
    XorListIterator<T> temp = *this;
    operator++();
    return temp;
}

template<typename T>
XorListIterator<T>& XorListIterator<T>::operator++() {
    typename Node<T>::pointer_type _next = reinterpret_cast<node_pointer>(_current)->neighbours_xor() ^_previous;
    _previous = _current;
    _current = _next;

    return (XorListIterator<T> &) *this;
}

template<typename T>
typename XorListIterator<T>::node& XorListIterator<T>::operator*() const {
    return *reinterpret_cast<node_pointer>(_current);
}

#endif //ITERATOR_H
