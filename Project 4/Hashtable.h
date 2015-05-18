using namespace std;

/*	Each Node represents one relationship in the HashTable.
	It has a pointer to what came before it in its bucket and what came after, if the exist.
	Also has a pointer to what came before and after it in the History linked list.*/

template <typename KeyType, typename ValueType>
struct Node{ 
	KeyType key;
	ValueType value;
	bool permanent;
	Node<KeyType, ValueType>* next_deletion;
	Node<KeyType, ValueType>* next_in_bucket;
	Node<KeyType, ValueType>* prev_deletion;
	Node<KeyType, ValueType>* prev_in_bucket;
};

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity)
	{
		arr = new Node<KeyType, ValueType>*[numBuckets]; // creates Node pointer array and initializes all to nullptr
		for (unsigned int i = 0; i < numBuckets; i++)
			arr[i] = nullptr;
		m_bucketCount = numBuckets; // holds bucket count
		oldest = nullptr; // nothing added, so no least recent or newest
		newest = nullptr;
		m_capacity = capacity;
		numAdded = 0; // no nodes have been added
	}
	~HashTable()
	{
		for (unsigned int i = 0; i < m_bucketCount; i++) // goes through each bucket and deletes all Nodes at the bucket
		{
			Node<KeyType, ValueType>* p = arr[i];
			while (p != nullptr)
			{
				Node<KeyType, ValueType>* temp = p->next_in_bucket;
				delete p;
				p = temp;
			}
		}
	}
	bool isFull()	const // returns true if the HashTable is full
	{
		return (m_capacity == numAdded);
	}
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false) // creates a new Node to hold passed in relationship or updates old relationship with new value 
	{
		unsigned int computeHash(KeyType); // prototypes function
		unsigned int buck = computeHash(key) % m_bucketCount; // computes the desired bucket (index in array) for passed in Key
		Node<KeyType, ValueType>* ifNeedToDelete = arr[buck]; // look at the top of that bucket
		while (ifNeedToDelete != nullptr) // if there was something there
		{
			if (ifNeedToDelete->key == key) // see if that key was found
			{
				if (ifNeedToDelete->permanent) // if it was permanent, only update the value
				{
					ifNeedToDelete->value = value;
					return true;
				}
				KeyType x;
				ValueType y;
				handleDeletion(x, y, ifNeedToDelete); // if it wasn't permanent, delete it; a new one will be added next
				break;
			}
			ifNeedToDelete = ifNeedToDelete->next_in_bucket;
		}
		if (isFull()) // if table is full, can't add a new Node
			return false;
		Node<KeyType, ValueType>* temp = new Node<KeyType, ValueType>; // dynamically allocate a new Node
		temp->key = key; // set relationship
		temp->value = value;
		temp->next_in_bucket = arr[buck]; // added to the top
		if (arr[buck] != nullptr)
		{
			arr[buck]->prev_in_bucket = temp; // set previous top's previous to the new top if one was there
		}
		temp->prev_in_bucket = nullptr; // nothing before it
		if (permanent) // if it was permanent, indicate so in the Node
			temp->permanent = true;
		else // otherwise, indicate in the Node and make it Newest using handlePrecedence function
		{
			temp->permanent = false;
			handlePrecedence(temp);
		}
		numAdded++; // one new Node added
		arr[buck] = temp; // make it the top at that index
		return true; 
	}
	bool get(const KeyType& key, ValueType& value)	const
	{
	    unsigned int computeHash(KeyType); // prototype
		unsigned int buck = computeHash(key) % m_bucketCount; // determine the proper bucket for passed in Key
		Node<KeyType, ValueType>* temp = arr[buck]; // get a pointer to top at that bucke
		while (temp != nullptr) // while there is something in the bucket
		{
			if (temp->key == key) // if key's match, set value and return
			{
				value = temp->value;
				return true;
			}
			temp = temp->next_in_bucket; // otherwise, look at next
		}
		return false; // since key never matched, return false
	}
	bool touch(const KeyType& key)
	{
	    unsigned int computeHash(KeyType); // prototype
		unsigned int buck = computeHash(key) % m_bucketCount; // get proper bucket for passed in key
		Node<KeyType, ValueType>* temp = arr[buck]; // get pointer to that bucket
		while (temp != nullptr && temp->key != key) // keep upping temp in the bucket until key is found or run out of room
			temp = temp->next_in_bucket;
		if (temp == nullptr) // if proper key wasn't found, return false
			return false;
		if (temp->permanent) // if relationship is permanent, return false
			return false;
		if (temp == oldest) // if it's the oldest thing in the History
		{
			oldest = temp->next_deletion; // update it to what comes after
			if (oldest != nullptr) // if there was something after, update its previous node
			{
				oldest->prev_deletion = nullptr;
			}
			if (oldest == nullptr) // otherwise, want to do nothing
			{
				oldest = temp;
				return true;
			}
		}
		if (temp->prev_deletion != nullptr) // handle connections before and after, if present
			temp->prev_deletion->next_deletion = temp->next_deletion;
		if (temp->next_deletion != nullptr)
			temp->next_deletion->prev_deletion = temp->prev_deletion;
		newest->next_deletion = temp; // temp now comes after the previous newest
		temp->prev_deletion = newest; // links it to Newest
		temp->next_deletion = nullptr; // nothing after the newest
		newest = temp; // update newest
		return true;
	}
	bool discard(KeyType& key, ValueType& value)
	{
		if (oldest == nullptr) // if oldest isn't there, do nothing
			return false;
		else
		{
			handleDeletion(key, value, oldest); // otherwise, delete oldest and handle pointers; update key and value values
			return true;
		}
	}
private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
	Node<KeyType, ValueType>** arr;
	Node<KeyType, ValueType>* oldest;
	Node<KeyType, ValueType>* newest;
	unsigned int m_capacity, m_bucketCount, numAdded;
	void handlePrecedence(Node<KeyType, ValueType>* temp)
	{
		if (oldest == nullptr) // if first added 
		{
			oldest = temp; // it's oldest and newest
			newest = temp;
			temp->next_deletion = nullptr; // nothing before or after it in history
			temp->prev_deletion = nullptr;
			return;
		}
		newest->next_deletion = temp; // newest's next  is now temp
		temp->next_deletion = nullptr; // nothing after temp
		temp->prev_deletion = newest; // newest comes before temp
		newest = temp; // update newest
	}
	void handleDeletion(KeyType& key, ValueType& val, Node<KeyType,ValueType>* temp) // removes passed in Node; updates key and val
	{
		key = temp->key; // update passed in key and value
		val = temp->value;
		numAdded--; // one fewer node in the table
		if (temp->next_deletion != nullptr) // handle before and after connections
			temp->next_deletion->prev_deletion = temp->prev_deletion;
		if (temp->prev_deletion != nullptr)
			temp->prev_deletion->next_deletion = temp->next_deletion;
		if (temp == oldest) // update oldest
		{
			if (temp == newest)
			{
				newest = oldest = nullptr;
			}
			else
				oldest = temp->next_deletion;
		}
		if (temp->prev_in_bucket == nullptr) // if top of its bucket, update what the top of the bucket is properly
		{
		    unsigned int computeHash(KeyType); // prototype
			unsigned int buck = computeHash(temp->key) % m_bucketCount;
			arr[buck] = temp->next_in_bucket;
			if (arr[buck] != nullptr)
				arr[buck]->prev_in_bucket = nullptr;
			delete temp;
			return;
		}
		else if (temp->next_in_bucket == nullptr) // if it's at the end of its bucket, update it properly
		{
			if (temp->prev_in_bucket != nullptr)
				temp->prev_in_bucket->next_in_bucket = nullptr;
			delete temp;
			return;
		}
		//handle adjacent bucket connections and return
		temp->prev_in_bucket->next_in_bucket = temp->next_in_bucket;
		temp->next_in_bucket->prev_in_bucket = temp->prev_in_bucket;
		delete temp;
		return;
	}
};