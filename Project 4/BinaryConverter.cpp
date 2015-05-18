#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)  // takes vector of numbers and returns string of tabs and spaces representing binary for each number concatenated 
{
	string ret; // to be returned
	for (size_t x = 0; x < numbers.size(); x++) // add together the binary representation of each number
		ret += convertNumberToBitString(numbers[x]);
	for (size_t x = 0; x < ret.size(); x++) // converts '0' and '1' to ' ' and '\t' respectively
	{
		ret[x] = (ret[x] == '0') ? ' ' : '\t';
	}
	return ret;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers) // takes an encoded string of tabs and spaces and converts it back to a vector of numbers
{
	if (bitString.size() % 16 != 0) // if not a multiple of 16, cannot be an encoded variant
			return false; 
	numbers.clear(); // empties passed in vector
	string y; // will hold spaces and temps at each space
	for (size_t x = 0; x < bitString.size(); x++)
	{
		if (bitString[x] != ' ' && bitString[x] != '\t') // if not a valid character, can't decode it
			return false;
		y += bitString[x]; // adds the character
		if (x % 16 == 15) // if this is the end of a particular number, because each unsigned short is represented in 16 bits
		{
			unsigned short temp;
			for (int i = 0; i < y.size(); i++) //convert the spaces and tabs to '0' and '1'
			{
				if (y[i] == ' ')
					y[i] = '0';
				else
					y[i] = '1';
			}
			convertBitStringToNumber(y, temp); // take this 16 size bit string and convert it into the respective unsigned short
			numbers.push_back(temp); // add unsigned short to vector
			y = ""; // reset temp holding string
		}
	}
	return true; // return true since no issues
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
