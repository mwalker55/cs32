#include "newMultiSet.h"
#include <cassert>
#include <iostream>
using namespace std;

int main()
{
	Multiset a(1000);   // a can hold at most 1000 distinct items
	Multiset b(5);      // b can hold at most 5 distinct items
	Multiset s;         // s can hold at most DEFAULT_MAX_ITEMS distinct items
	Multiset c;
	ItemType v[6] = {2323,5555,4242,3232,5252,5636};

	// No failures inserting 5 distinct items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(v[k]));

	// Failure if we try to insert a sixth distinct item into b
	assert(!b.insert(v[5]));

	// When two Sets' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(v[5]));
	b=a;
	assert(!b.insert(v[5]));
	assert(!b.insert(1231));

	assert(s.empty()); // check to see if s set is empty
	assert(!b.empty()); // check to see if a set is not empty

	unsigned long x = 9876543;
	assert( !s.get(42,x)  &&  x == 9876543); // x unchanged by get failure
	s.insert(123456789);
	assert(s.size() == 1);
	assert(s.get(0,x)  &&  x == 123456789);
	s.erase(123456789); // erase 123456789
	assert(!s.contains(123456789)); // check if it doesn't contain 123456789
	assert(s.empty()); // check to see if the set is empty

	s.insert(123);
	assert(s.size() == 1); // check size = 1
	s.insert(321);
	assert(s.size() == 2); // check size = 2
	s.insert(222);
	assert(s.size() == 3); // check size = 3

	assert(s.contains(123) && s.contains(222) && s.contains(321));
	// check to see if all numbers are in the set

	c.insert(987);
	c.insert(789);

	s.swap(c);

	unsigned long temp = 0;
	c.get(0, temp);
	assert(temp == 123); //check to see if temp was correctly changed

	assert(!c.get(10000,temp)); // get failure

	assert(c.contains(123));
	assert(c.contains(321));
	assert(c.contains(222));
	//check for correct swap

	//assert(s.contains(789));
	assert(s.contains(987));//check for correct swap

	c.erase(321);
	assert(c.contains(123)); // check to see if the set still contains 123
	assert(!c.contains(321)); // check to see if the set doesn't contain 321
	assert(c.contains(222)); // check to see if the set still contains 222
	assert(c.size() == 2); // check the size = 2

	a = c;
	assert(a.contains(123)); // check to see if the set still contains 123
	assert(!a.contains(321)); // check to see if the set doesn't contain 321
	assert(a.contains(222)); // check to see if the set still contains 222
	assert(a.size() == 2); // check the size = 2

	cout << "Passed all tests" << endl;

}
