#include "provided.h"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

void strip(vector<string>& holder) // strips trailing tabs and spaces from the strings
{
	for (size_t m = 0; m < holder.size(); m++) // go through each string in the vector
	{
		if (holder[m].empty()) // if already empty, move on
			continue;
		int index = holder[m].size(); // start the final end at original end
		for (int t = holder[m].size()-1; t >= 0; t--)
		{
			if (holder[m][t] == ' ' || holder[m][t] == '\t') // if space or tab, decrease final end by one
				index--;
			else // otherwise, no more trailing tabs or spaces so get our
				break;
		}
		if (index == 0) // if was all tabs and spaces, should be empty string
			holder[m] = "";
		else
			holder[m] = holder[m].substr(0, index); // otherwise, substring up to index length
	}
}

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.size() == 0) // if passed in HTML was empty, nothing to do
		return false;
	vector<string> lines; // will hold each HTML line
	string temp = "";
	for (size_t x = 0; x < hostIn.size(); x++) // harvests the line
	{
		if (hostIn[x] == '\n') // if it was an endline character, add everything added before and reset temp holder
		{
			lines.push_back(temp);
			temp = "";
		}
		else if (hostIn[x] != '\r' || (hostIn[x] == '\r' && x+1 < hostIn.size() && hostIn[x+1] == '\n')) // if it wasn't an endline character or register char followed by an endline, add it temp holder
		{
			temp += hostIn[x];
		}
	}
	if (temp != "") // in case it didn't end with an endline, final temp wasn't added
	{
		lines.push_back(temp);
	}
	strip(lines); // strip all trailing tabs and spaces
	vector<unsigned short> x; // will hold compression 
	Compressor::compress(msg, x); // compresses message into the vector of chars
	string encoded = BinaryConverter::encode(x); // encodes the vector
	string ret = "";
	vector<string> toBeAddedToLines; // will hold needed to be added chars
	temp = "";
	int index = 0, currSize = ((encoded.size() / lines.size()) + 1), midPoint = encoded.size() % lines.size(); // holds values: current line we're adding for binary, current size of binary we're adding, where we need to change
	// size of binary to be encoded
	for (int i = 0; i < encoded.size(); i++) // go to end of encoded string
	{ 
		temp += encoded[i]; // add char to temp holder
		if (temp.size() == currSize) // if we've reached the size currently going for
		{
			toBeAddedToLines.push_back(temp); // add it to binary holders
			index++; // increase number of binary sections
			if (index == midPoint) // if we've reached L % N lines added, decrease size of what's being added by one
			{
				currSize--;
			}
			temp = ""; // reset holder
		}
	}
	if (temp != "") // in case something odd happened, push on remaining binary to end
	{
		toBeAddedToLines.push_back(temp);
	}
	int i;
	for (i = 0; i < lines.size() && i < toBeAddedToLines.size(); i++) // go through and add binary to end of first lines that need it
	{
		ret += (lines[i] + toBeAddedToLines[i] + '\n');
	}
	for (; i < lines.size(); i++) // if there were more lines than the size of the encoded, lots will not have binary added; add these to final HTML as well
	{
		ret += (lines[i] + '\n');
	}
	hostOut = ret; // change hostOut to processed string
	return true;
}

bool Steg::reveal(const string& hostIn, string& msg) // takes in HTML with possibly hidden message and sets msg equal to decoded, if one existed
{
	if (hostIn == "") // if no HTML to decode, there can't be a message
		return false;
	string y;
	string holder = "";
	for (size_t x = 0; x < hostIn.size(); x++) // go through the message and collect trailing binary
	{
		if (hostIn[x] == '\n')
		{ 
			string xxx;
			for (int z = x - 1; z >= 0 && hostIn[z] != '\n'; z--) // when an endline is found, collect backward the trailing binary
			{
				if (hostIn[z] == ' ' || hostIn[z] == '\t')
				{
					xxx.push_back(hostIn[z]);
				}
				else
					break;
			}
			reverse(xxx.begin(), xxx.end()); // reverse the trailing binary (since it was added backward)
			holder += xxx; // add it to final holder
		}
	}
	if (holder.size() == 0) // if there was no trailing binary, nothing to decode
		return false;
	vector<unsigned short> yy;
	
	if (!BinaryConverter::decode(holder, yy)) // if couldn't properly decode the trailing binary, return false
		return false;
	if (!Compressor::decompress(yy, y)) // if decompressing caused an issue, return false
		return false;
	msg = y; // message was decompressed and decoded, so set it equal
	return true;
}
