#include "provided.h"
#include "Hashtable.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

unsigned int computeHash(string x) // hash computations for string and unsigned shorts
{
	int hash = 0;
	const int HASH_FACTOR = 251;
	for (size_t i = 0; i < x.size(); i++) {
		hash = hash * HASH_FACTOR + x[i];
	}
	return hash;
}

unsigned int computeHash(unsigned short x)
{
	return x;
}

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	int C = s.size() / 2 + 512; // set capacity equal to size over 2 + 512
	if (C > 16384) // if that was too big, cap capacity at 16384
		C = 16384;
	HashTable<string, unsigned short> H(C / .5, C); // bucket count is 2* capacity to satisfy load requirement
	for (int i = 0; i < 256; i++) // push in all characters from 0 to 255 with permanent associations
	{
		string s = string(1, static_cast<char>(i));
		H.set(s, i, true);
	}
	int nextFreeID = 256; 
	string runSoFar = "";
	vector<unsigned short> V;
	for (int i = 0; i < s.size(); i++) // go through each character in passed in string
	{
		char c = s[i];
		string expandedRun = runSoFar + c;
		unsigned short temp;
		if (H.get(expandedRun, temp)) // if expanded run was found in the hashtable already
		{
			runSoFar = expandedRun; // set runSoFar to expandedRun and iterate again
			continue;
		}
		H.get(runSoFar, temp); // otherwise, get runSoFar's value
		V.push_back(temp); // add that value to vector of unsigned shorts
		H.touch(runSoFar); // touch what runSoFar's value is so it's not next discard target
		runSoFar = ""; // reset runSoFar
		unsigned short cv;
		H.get(string(1,c), cv); // get value for the current character
		V.push_back(cv);
		if (!H.isFull()) // if hashtable isn't full
		{
			H.set(expandedRun, nextFreeID); // create new association
			nextFreeID++; // increase nextFreeID
		}
		else
		{
			unsigned short x; // discard oldest value
			string y;
			H.discard(y, x);
			H.set(expandedRun, x); // set expandedRun to nextFreeID of what was discarded
		}
	}
	if (runSoFar != "") // adds runSoFar's value from the hashTable to the vector
	{
		unsigned short x;
		H.get(runSoFar, x);
		V.push_back(x);
	}
	V.push_back(C); // push capacity onto the vector
	numbers = V; // set the vectors equal
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	HashTable<unsigned short, string> H(numbers[numbers.size()-1] / .5, numbers[numbers.size()-1]); // create a new HashTable, using capacity from end of passed in Vector and bucket count satisfying load of .5
	for (int i = 0; i < 256; i++) // create associations between the first 255 characters and their respective unsigned shorts
	{
		string s = string(1, static_cast<char>(i));
		H.set(i, s, true);
	}
	int nextFreeID = 256; 
	string runSoFar = "";
	string output = "";
	for (int p = 0; p < numbers.size() - 1; p++) // go through all unsigned shorts in numbers except for the last one, holding capacity
	{
		unsigned short z = numbers[p]; // get the short at current index
		if (z <= 255) // if it's a single char
		{
			string x;
			H.get(z, x); // get its corresponding strng
			output += x; // add it to final string
			if (runSoFar == "") // append to runSoFar if its empty at the moment
			{
				runSoFar += x;
				continue;
			}
			string expandedRun = runSoFar + x; 
			if (!H.isFull())
			{
				H.set(nextFreeID, expandedRun); // if not full, create assocation between expandedRun and current nextFreeID
				nextFreeID++; // increment nextFreeID
			}
			else
			{
				unsigned short me; // otherwise, discard oldest and map expandedRun to resultant unsigned short from discard
				string zz;
				H.discard(me, zz);
				H.set(me, expandedRun);
			}
			runSoFar = "";
			continue;
		}
		else // if it was a multichar string
		{
			string x = ""; // must be in the hashtable, so get it
			if (!H.get(z, x))
			{
				return false; // fatal error, so return false
			}
			H.touch(z); // touch it
			output += x; // add it to final string
			runSoFar = x; // update runSoFar
			continue;
		}
	}
	s = output; // update final decompressed message
	return true;
}