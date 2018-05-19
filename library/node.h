#ifndef NODE_H
#define NODE_H

#include <iostream>

template <class T>
class Node {
public:
    using value_type = T;
    using pointer_type = uintptr_t;
private:
    value_type _value;
    pointer_type _neighbours_xor;
public:
    Node(const T& value, Node<T>* prev = nullptr, Node<T>* next = nullptr);

    void xors(pointer_type p);
    pointer_type neighbours_xor() const;
    value_type& get();

    Node* next(Node* v);
};

template <class T>
Node<T>::Node(const T& value, Node<T>* prev, Node<T>* next) :
        _value(value), _neighbours_xor(reinterpret_cast<pointer_type>(prev) ^ reinterpret_cast<pointer_type>(next)) {}

template <class T>
void Node<T>::xors(pointer_type p) {
    _neighbours_xor ^= p;
}

template <class T>
typename Node<T>::pointer_type Node<T>::neighbours_xor() const {
    return _neighbours_xor;
}

template <class T>
typename Node<T>::value_type& Node<T>::get() {
    return _value;
}


#endif //NODE_H
