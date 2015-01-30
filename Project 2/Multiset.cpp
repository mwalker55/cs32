#include "Multiset.h"

Multiset::Multiset() // default constructor
{
    numNodes = 0; // no nodes added yet
    first = nullptr; // no head node so make it null
    last = nullptr; // no tail node so make it null
}

Multiset::Multiset(const Multiset& other) // copy constructor
{
    if (other.first == nullptr) // if other multiset is empty, no need to do anything beyond default
    {
        numNodes = 0;
        first = nullptr;
        last = nullptr;
        return;
    }
    numNodes = 0;
    first = nullptr;
    last = nullptr;

    Multiset* newSet = new Multiset; // create a new placeholder Multiset
    for (int i = 0; i < other.uniqueSize(); i++) // add each value from ms1 to the temp Multiset, adding it count times
    {
        ItemType y;
        int x = other.get(i, y);
        for (int z = 0; z < x; z++)
            newSet->insert(y);
    }
    this->swap(*newSet);
    delete newSet;
}

Multiset& Multiset::operator=(const Multiset& swg) // assignment operator
{
    if (&swg == this) // alias case
        return (*this);
    this->~Multiset(); // destruct current multiset so no memory leak
    Multiset* newd = new Multiset(swg); // create a temporary copy of what's being assigned from
    this->swap(*newd); // swap our current values with the temp values
    delete newd; // just in case, delete the swapped out temp's values
    return(*this); // for chained = case
}

Multiset::~Multiset() // destructor
{
    if (first == nullptr) // if Multiset is empty, nothing to do
    {
        return;
    }
    Node* temp = first; // start at the head
    Node* zzz = nullptr; // will hold reference to next Node
    do
    {
        zzz = temp->next; // refer to next Node
        delete temp; // delete current node
        temp = zzz; // set temp to the next Node
    } while (temp != nullptr); // if the next Node wasn't there, stop deleting
    first = nullptr; // eliminate dangling pointer risk
    last = nullptr;
}

bool Multiset::empty() const // returns true if Multiset is empty
{
    return (first == nullptr); // if the head pointer is null, it's empty
}

int Multiset::size() const // returns total number of items; if an item appears twice, it will count 2 toward total
{
    if (first == nullptr) // if empty, size is 0
        return 0;
    Node* temp = first; // refer to first Node
    int x = 0; // total is 0
    do // goes through all Nodes and adds the counts while Nodes are still around
    {
        x+=temp->count;
        temp = temp->next;
    } while (temp != nullptr);
    return x; // return the sum of all counts
}

int Multiset::uniqueSize() const // returns number of unique items in list
{
    return numNodes;
}

bool Multiset::insert(const ItemType& value) // inserts new item into Linked List
{
    if(first == nullptr) // if list is currently empty, add it to the top
    {
        Node* temp = new Node; // create new node and initialize values
        temp->value = value;
        temp->count = 1;
        temp->before = nullptr; // only thing in list so nothing before or after
        temp->next = nullptr;
        first = temp; // head and tail point to this
        last = temp;
        numNodes++; // new node added
        return true;
    }
    else // already something in liked list
    {
        Node* temp = first;
        do // go through and check if value is already in list; if it is, simply increase the count at that node
        {
            if(temp->value == value)
            {
                temp->count++;
                return true;
            }
            temp = temp->next;
        } while (temp != nullptr);

        temp = last; // since it wasn't there, insert i at the end
        Node* newOne = new Node; // create new Node and initialize values
        newOne->value = value;
        newOne->count = 1;
        newOne->next = nullptr; // nothing after it
        newOne->before = temp; // what was at end is equal to it
        last = newOne; // tail pointer now equals it
        temp->next = newOne;
        numNodes++; // added a node
        return true;
    }
}


int Multiset::erase(const ItemType& value) // decreases count of Value by 1 if in list
{
    if (first == nullptr) // if list is empty, can't erase it
        return 0;
    Node* temp = first;
    do // go through list; if Node holding value is found, decrease value by 1
    {
        if (temp->value == value)
        {
            temp->count--;
            if (temp->count == 0) // if lowering count by 1 reduced it to 0, take Node out of the list
            {
                obliterate(temp);
            }
            return 1;
        }
        temp = temp->next;
    } while (temp != nullptr);
    return 0; // Value not in list, so nothing erased
}

int Multiset::eraseAll(const ItemType& value) // eliminates Node holding value in the list
{
    if (first == nullptr) // if list is empty, nothing to eliminate
        return 0;
    Node* temp = first;
    do // go through list to find desired value
    {
        if (temp->value == value)
        {
            return obliterate(temp); // if value was found, eliminate it and return the count that it had had
        }
        temp = temp->next;
    } while (temp!= nullptr);
    return 0; // value wasn't found, so return 0
}

int Multiset::obliterate(Node* temp) // deletes the Node from list and returns the Node's count
{
    if(temp == first) // if deleting at the beginning
    {
        if (temp == last) // only one item in list
        {
            temp->next = nullptr; // handle all dangling pointers
            temp->before = nullptr;
            first = nullptr;
            last = nullptr;
            numNodes = 0; // no nodes anymore
            int retValue = temp->count; // will return count
            delete temp; // deallocate memory
            return retValue;
        }
        Node* temp1 = temp->next; // fix all pointers so that next item is now the head
        temp->next = nullptr;
        temp->before = nullptr;
        temp1->before = nullptr;
        first = temp1;
        numNodes--; // 1 less node
        int retValue = temp->count;  // set up return value
        delete temp; // deallocate memory
        return retValue;
    }
    else if (temp == last) // if deleting at the end
    {
        Node* temp1 = temp->before; // handle all pointers so that what was before is now the tail
        temp->next = nullptr;
        temp->before = nullptr;
        temp1->next = nullptr;
        last = temp1;
        numNodes--; // one less node
        int retValue = temp->count; // set up return value
        delete temp; // deallocate memory
        return retValue;
    }
    else // Node is in the middle somewhere
    {
        Node* temp1 = temp->before; // handle pointers to link the next and before Nodes
        Node* temp2 = temp->next;
        temp->before = nullptr;
        temp->next = nullptr;
        temp1->next = temp2;
        temp2->before = temp;
        numNodes--; // one less node
        int retValue = temp->count; // set up return value
        delete temp; // deallocate memory
        return retValue;
    }
}

bool Multiset::contains(const ItemType& value) const // returns true if value is held by a Node in the list
{
    if (first == nullptr) // if list is empty, impossible for it to be present
        return false;
    Node* temp = first;
    do // go through list; if a Node has value, return true
    {
        if (temp->value == value)
            return true;
        temp = temp->next;
    } while(temp != nullptr);
    return false; // Value never found in list so return false
}

int Multiset::count(const ItemType& value) const // return a value's count in the list if present
{
    if (first == nullptr) // list is empty, no need to search through
        return 0;
    Node* temp = first;
    do // go through all Nodes
    {
        if (temp->value == value)
            return temp->count; // if a Node had desired value, return the count at that Node
        temp = temp->next;
    } while (temp!= nullptr);

    return 0; // value was not held by a Node so return 0
}

int Multiset::get(int i, ItemType& value) const // if i is between 0 and uniqueSize, set value equal to an item in the list and return the count of that value
{
    if (i < 0 || i > numNodes || first == nullptr) // if list is empty or i is too big
        return 0;
    Node* temp = first;
    for(int j = 0; j != i; j++) // keep going for however many i was passed in
        temp = temp->next;
    value = temp->value; // set value equal to the Node's value
    return temp->count; // return the count at that Node
}


void Multiset::swap(Multiset& other) // swaps the 2 multisets
{
    Node* tempFirst = other.first; // create temp values representing the other Multiset's 3 private variables
    Node* tempLast = other.last;
    int tempNumNodes = other.numNodes;
    other.first = first; // set other Multiset's private variables equal to current class' privates
    other.last = last;
    other.numNodes = numNodes;
    first = tempFirst; // set current class' privates equal to those from other multiset originally
    last = tempLast;
    numNodes = tempNumNodes;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result) // combines the 2 Multisets; adds the values by amount count from each Multiset
{
    Multiset* newSet = new Multiset(ms1); // creates a new Multiset holding all values from ms1
    for (int i = 0; i < ms2.uniqueSize(); i++) // add each value from ms2 to the temp Multiset, adding it count times
    {
        ItemType y;
        int x = ms2.get(i, y);
        for (int z = 0; z < x; z++)
            newSet->insert(y);
    }
	result.swap(*newSet);
	delete newSet;
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)  // subtracts Multiset2 from Multiset1
{
    Multiset* newSet = new Multiset(ms1); // creates a new Multiset holding all the values from ms1
    for (int i = 0; i < ms2.uniqueSize(); i++) // erase each value from ms2 from temp Multiset count times, erasing it count times; if its no longer in ms1
    {
        ItemType y;
        int x = ms2.get(i, y);
        for (int z = 0; z < x; z++)
            newSet->erase(y);
    }
	result.swap(*newSet);
	delete newSet;
}