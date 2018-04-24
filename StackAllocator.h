#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include <iostream>
#include <vector>

template <typename T>
class StackAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        typedef StackAllocator<U> other;
    };

    static const size_type M_SIZE = 1e7;
    size_type size = 0;

    pointer current_memory;
    std::vector<pointer> data;

    StackAllocator(StackAllocator const&) = delete;
    StackAllocator& operator=(StackAllocator const&) = delete;

    StackAllocator();
    ~StackAllocator();

    pointer allocate(size_t n);
    void deallocate(pointer m, size_type n);
};


#endif //STACKALLOCATOR_H
