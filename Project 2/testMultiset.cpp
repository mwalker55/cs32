#include "Multiset.h"
#include <iostream>
using namespace std;

void test()
{
    Multiset ms1;
    assert(ms1.empty()); // ensure its empty
    assert(ms1.erase("swg") == 0);
    string x;
    assert(ms1.get(0, x) == 0); // make sure get doesn't return anything because multiset is empty
    assert(x==""); // ensure get didn't change the value passed in
    assert(ms1.insert("swg")); // fill up the multiset in next few lines
    assert(ms1.insert("nnn"));
    assert(ms1.insert("swg"));
    assert(ms1.insert("fff"));
    assert(ms1.insert("mark"));
    assert(ms1.insert("nick"));
    assert(ms1.insert("barack"));
    assert(ms1.insert("obama"));
    assert(ms1.insert("mark"));
    assert(ms1.erase("mark")); // erase 1 of the 2 instances of mark
    assert(ms1.contains("mark")); // ensure the other one is still present
    assert(ms1.eraseAll("barack")); // erase all instances of barack
    assert(!ms1.contains("barack")); // ensure barack is no longer present
    assert(ms1.size() == 7); // should still be 7 items present
    assert(ms1.get(0, x) == 2); // 2 instances of swg present
    assert(x == "swg"); // x should have been turned into swg
    Multiset ms2 = ms1; // tests copy constructor
    assert(ms2.contains("swg")); // check if copied multiset also has that value
    assert(ms2.size() == 7); //ensure size is the same
    assert(ms2.uniqueSize() == 6); // ensures all separate values got brought over
    Multiset ms3; // will be used in testing combine and subtract
    combine(ms1, ms2, ms3); // this also tests swap
    assert(ms3.size() == 14); // ms1 and ms2 were copies of each other so combined multiset should have x2 size
    assert(ms3.uniqueSize() == 6); // they contained same values so uniquesize should not be changed
    subtract(ms1, ms2, ms3); // this tests swap within it
    assert(ms3.size() == 0 && ms3.uniqueSize() == 0 && ms3.empty()); // since ms1 and ms2 were equal, ms3 should be totally empty
    ms2.insert("rip lyl"); // make ms2 distinct from ms1
    ms1.swap(ms2); // swap the multisets
    assert(ms1.size() == 8 && ms2.size() == 7); // ms1 should now be the bigger one post-swap
}

int main()
{
	test();
	cout << "All tests worked" << endl;
}
