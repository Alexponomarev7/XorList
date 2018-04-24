#include "StackAllocator.h"
#include <list>

using namespace std;

int main() {
    list<int, StackAllocator<int>> v = {1, 2, 5, 6};

    cout << v.back() << endl;
    return 0;
}
