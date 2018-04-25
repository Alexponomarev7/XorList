#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include <iostream>
#include <cassert>

class AllocatorNode {
private:
    static const size_t M_SIZE = 1e7;
    size_t size;
    char* current_memory;
public:
    AllocatorNode *next;

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

    StackAllocator();
    ~StackAllocator();

    StackAllocator(StackAllocator const&) = default;
    StackAllocator& operator=(StackAllocator const&) = default;

    pointer allocate(size_type n);
    void deallocate(pointer m, size_type n);
private:
    AllocatorNode* data;
};

template<class T>
StackAllocator<T>::StackAllocator() {
    data = new AllocatorNode();
}

template<class T>
StackAllocator<T>::~StackAllocator() {
    while (data != nullptr) {
        AllocatorNode* next = data;
        data = data->next;
        delete next;
    }
}

template<class T>
typename StackAllocator<T>::pointer StackAllocator<T>::allocate(StackAllocator<T>::size_type n) {
    StackAllocator<T>::size_type required_memory = sizeof(T) * n;

    if (data->empty() < required_memory) {
        AllocatorNode* new_data = new AllocatorNode();
        new_data->next = data;
        data = new_data;
    }

    return reinterpret_cast<pointer>(data->alloc(required_memory));
}

template<class T>
void StackAllocator<T>::deallocate(StackAllocator<T>::pointer m, StackAllocator<T>::size_type n) {
    return;
}

AllocatorNode::AllocatorNode() {
    current_memory = new char[M_SIZE];
    size = 0;
}

AllocatorNode::~AllocatorNode() {
    delete[] (current_memory - size);
}

char* AllocatorNode::alloc(size_t required_memory) {
    char* res = current_memory;
    current_memory += required_memory;
    size += required_memory;
    return res;
}

size_t AllocatorNode::empty() const {
    return M_SIZE - size;
}

#endif //STACKALLOCATOR_H
