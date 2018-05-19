#include "iterator.h"

template<typename T>
bool XorListIterator<T>::operator==(const XorListIterator<T>& other) {
    return _current == other._current && _neighboursXor == other._neighboursXor;
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
    typename Node<T>::pointer_type _next = _current ^ _previous;
    _previous = _current;
    _current = _next;

    return (XorListIterator<T> &) this;
}