#include "provided.h"
#include "http.h"
#include <string>
#include <iostream> 
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host) // takes in a web page's HTML and a message and hides the message in the page's HTML
{
	string text; 
	if (HTTP().get(url, text)) // ensures the website coud be connected to
	{
		return Steg::hide(text, msg, host); // if it could, return results of hide; host will now hold HTML with hidden message
	}
	return false; // couldn't connect to site, so return false
}

bool WebSteg::revealMessageInPage(const string& url, string& msg) // takes HTML with possibly encoded message and finds and decodes it
{
	string hostIn;
	if (HTTP().get(url, hostIn)) // ensures website could be connected to
		return Steg::reveal(hostIn, msg); // reveals message in HTML if one is there
	return false; // couldn't connect so return false
}