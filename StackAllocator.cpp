#include "StackAllocator.h"

template<typename T>
StackAllocator<T>::StackAllocator() {
    current_memory = new T[M_SIZE];
    data.push_back(current_memory);

    size = 0;
}

template<typename T>
StackAllocator<T>::~StackAllocator() {
    for (auto x : data) {
        delete[] x;
    }
}

template<typename T>
typename StackAllocator<T>::pointer StackAllocator<T>::allocate(typename StackAllocator<T>::size_type n) {
    if (size + n >= M_SIZE) {
        current_memory = new typename StackAllocator<T>::value_type[M_SIZE];
        size = 0;
        data.push_back(current_memory);
    }

    typename StackAllocator<T>::pointer res = current_memory;

    current_memory += n;
    size += n;
    return res;
}

template<typename T>
void StackAllocator<T>::deallocate(StackAllocator<T>::pointer m, StackAllocator<T>::size_type n) {
    return;
}