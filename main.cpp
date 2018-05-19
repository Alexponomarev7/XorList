#include <list>
#include <iostream>
#include <random>

#include "xor_list.h"

#include "StackAllocator.h"

using namespace std;

int main() {
    XorList<int, StackAllocator<int>> x;
    x.push_back(5);
    x.push_front(3);
    x.push_front(7);

    const int &a = 11;

    x.push_back(a);

    for (auto &y : x) {
        cout << y.get() << endl;
    }
    return 0;
}