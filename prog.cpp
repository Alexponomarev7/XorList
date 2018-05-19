#include <vector>
#include "prog.h"
#include "StackAllocator.h"

void f() {
    std::vector<int, StackAllocator<int>> v = {1, 2, 5};
}
