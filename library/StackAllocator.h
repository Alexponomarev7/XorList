#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include <iostream>
#include <cassert>

class AllocatorNode {
private:
    static size_t _M_SIZE;

    size_t _size;
    char* _current_memory;
public:
    std::shared_ptr<AllocatorNode> next;

    AllocatorNode();
    ~AllocatorNode();

    char* alloc(size_t required_memory);
    size_t empty() const;
};

template <class T>
class StackAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<class U>
    struct rebind {
        typedef StackAllocator<U> other;
    };
public:
    std::shared_ptr<AllocatorNode>  _data;

    StackAllocator();
    ~StackAllocator();

    template<class U>
    StackAllocator(const StackAllocator<U>& other);

    pointer allocate(size_type n);
    void deallocate(pointer m, size_type n);
};

size_t AllocatorNode::_M_SIZE = 1e7;

template<class T>
template<class U>
StackAllocator<T>::StackAllocator(const StackAllocator<U>& other) :
        _data(other._data) {}

template<class T>
StackAllocator<T>::StackAllocator() : _data(std::make_shared<AllocatorNode>()) {}

template<class T>
StackAllocator<T>::~StackAllocator() {
    while (_data != nullptr) {
        std::shared_ptr<AllocatorNode> next = _data;
        _data = _data->next;

        next.reset();
    }
}

template<class T>
typename StackAllocator<T>::pointer StackAllocator<T>::allocate(StackAllocator<T>::size_type n) {
    StackAllocator<T>::size_type required_memory = sizeof(T) * n;


    if (_data->empty() < required_memory) {
        auto new_data = std::make_shared<AllocatorNode>();
        new_data->next = _data;
        _data = new_data;
    }

    return reinterpret_cast<pointer>(_data->alloc(required_memory));
}

template<class T>
void StackAllocator<T>::deallocate(StackAllocator<T>::pointer m, StackAllocator<T>::size_type n) {}

AllocatorNode::AllocatorNode() : _current_memory(new char[_M_SIZE]), _size(0), next(nullptr) {}

AllocatorNode::~AllocatorNode() {
    delete[] (_current_memory - _size);
}

char* AllocatorNode::alloc(size_t required_memory) {
    char* res = _current_memory;
    _current_memory += required_memory;
    _size += required_memory;
    return res;
}

size_t AllocatorNode::empty() const {
    return _M_SIZE - _size;
}

#endif //STACKALLOCATOR_H