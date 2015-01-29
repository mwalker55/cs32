#include "StudentMultiset.h"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{
    StudentMultiset ms;
    assert(ms.add(111));
    assert(ms.add(555));
    assert(ms.add(4418546));
    assert(ms.add(111));
    assert(ms.size() == 4);
}

